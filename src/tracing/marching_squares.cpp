#include "marching_squares.hpp"

#include <vector>

// Straight line connecting two vertices. @todo Optimize marching squares and remove this.
struct line {
    // Vertex indices.
    std::size_t vertex_indices[2];
};

// Vertex contaning position and line index.
struct vertex {
    // The postion of the vertex.
    ptg_vec2 position;

    // Line index which this vertex belongs.
    std::size_t line_index;

    // Whether vertex is assigned a contour.
    bool assigned_contour;
};

/*
 * Create vertices and add line to vectors.
 * @param v1 First vertex.
 * @param v2 Second vertex.
 * @param out_lines Vector to store resulting lines.
 * @param out_vertices Vector to store resulting vertices.
 */
static void add_line(const ptg_vec2& v1, const ptg_vec2& v2, std::vector<line>& out_lines, std::vector<vertex>& out_vertices) {
    out_lines.push_back({ out_vertices.size(), out_vertices.size() + 1 });
    out_vertices.push_back({ v1, out_lines.size() - 1, false });
    out_vertices.push_back({ v2, out_lines.size() - 1, false });
}

/*
 * Marching squares algorithm.
 * @param topLeft Whether top-left node is active in marching kernel.
 * @param topRight Whether top-right node is active in marching kernel.
 * @param bottomRight Whether bottom-right node is active in marching kernel.
 * @param bottomLeft Whether bottom-left node is active in marching kernel.
 * @param x X-coordinate (right positive) for center of the marching kernel relative to top-left coner of image in pixels.
 * @param y Y-coordinate (down positive) for center of the marching kernel relative to top-left coner of image in pixels.
 * @param out_lines Vector to store resulting lines.
 * @param out_vertices Vector to store resulting vertices.
 */
static void marching_squares(bool topLeft, bool topRight, bool bottomRight, bool bottomLeft, unsigned int x, unsigned int y, std::vector<line>& out_lines, std::vector<vertex>& out_vertices) {

    // Calculate current configuration of marching squares.
    const unsigned int square_configuration = topLeft * 8 + topRight * 4 + bottomRight * 2 + bottomLeft * 1;
    
    // Convert (x,y) from pixel space to mesh space.
    x = x * 2;
    y = y * 2;
    /* Nodes (A, B, C, D) in marching kernel for 2x2 pixels.
     * x: positive right, y: positive down.
     * # ------- #
     * |    A    |
     * | D(x,y)B |
     * |    C    |
     * # ------- # */
    const ptg_vec2 A = { x, y - 1 };
    const ptg_vec2 B = { x + 1, y };
    const ptg_vec2 C = { x, y + 1 };
    const ptg_vec2 D = { x - 1, y };

    switch (square_configuration) {
        // 0 points:    
        case 0:
            break; 

        // 1 points:
        // C -> D
        case 1:
            add_line(C, D, out_lines, out_vertices);
            break;

        // B -> C
        case 2:
            add_line(B, C, out_lines, out_vertices);
            break;

        // A -> B
        case 4:
            add_line(A, B, out_lines, out_vertices);
            break;

        // D -> A
        case 8:
            add_line(D, A, out_lines, out_vertices);
            break;

        // 2 points:
        // B -> D
        case 3:
            add_line(B, D, out_lines, out_vertices);
            break;
        
        // A -> C
        case 6:
            add_line(A, C, out_lines, out_vertices);
            break;

        // C -> A
        case 9:
            add_line(C, A, out_lines, out_vertices);
            break;

        // D -> B
        case 12:
            add_line(D, B, out_lines, out_vertices);
            break;

        // 2 points (diagonal):
        // C -> B, A -> D
        case 5:
            add_line(C, B, out_lines, out_vertices);
            add_line(A, D, out_lines, out_vertices);
            break;

        // B -> A, D -> C
        case 10:
            add_line(B, A, out_lines, out_vertices);
            add_line(D, C, out_lines, out_vertices);
            break;

        // 3 points:
        // A -> D
        case 7:
            add_line(A, D, out_lines, out_vertices);
            break;

        // B -> A
        case 11:
            add_line(B, A, out_lines, out_vertices);
            break;

        // C -> B
        case 13:
            add_line(C, B, out_lines, out_vertices);
            break;

        // D -> C
        case 14:
            add_line(D, C, out_lines, out_vertices);
            break;

        // 4 points:
        case 15:
            break;
    }
}

/*
 * Find other vertex with same position.
 * @param this_index Vertex index of vertex to find other vertex with same position.
 * @param vertices The vertices.
 * @param out_found_index Variable to store vertex index of the found vertex.
 * @return Whether an index was found.
 * @todo Optimize using lookup table.
 */
static bool find_equal_vertex(const std::size_t this_index, const std::vector<vertex>& vertices, std::size_t& out_found_index) {
    const vertex& this_vertex = vertices[this_index];
    for (std::size_t vertex_index = 0; vertex_index < vertices.size(); ++vertex_index) {
        const vertex& vertex = vertices[vertex_index];
        if (this_index != vertex_index && vertex.position.x == this_vertex.position.x && this_vertex.position.y == vertex.position.y) {
            out_found_index = vertex_index;
            return true;
        }
    }
    return false;
}

void ptgi_trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count) {

    std::vector<line> lines;
    std::vector<vertex> vertices;

    // Execute marching squares on layer.
    for (unsigned int it = 0; it < (layer_width * layer_height); ++it) {
        bool topEdge = it < layer_width;
        bool bottomEdge = it >= (layer_width * (layer_height - 1));
        bool leftEdge = it % layer_width == 0;
        bool rightEdge = it % layer_width == (layer_width - 1);

        bool topLeft = topEdge || leftEdge ? false : layer[it - layer_width - 1];
        bool topRight = topEdge || rightEdge ? false : layer[it - layer_width];
        bool bottomRight = bottomEdge || rightEdge ? false : layer[it];
        bool bottomLeft = bottomEdge || leftEdge ? false : layer[it - 1];

        marching_squares(topLeft, topRight, bottomRight, bottomLeft, it % layer_width, it / layer_width, lines, vertices);
    }

    // Create contours.
    std::vector<std::vector<std::size_t>> contours;
    for (std::size_t vertex_index = 0; vertex_index < vertices.size(); ++vertex_index) {
        if (!vertices[vertex_index].assigned_contour) {

            // Create new contour.
            contours.push_back(std::vector<std::size_t>());
            std::vector<std::size_t>& contour = contours.back();

            // Trace contour (assign connected vertex indices to contour).
            std::size_t it_vertex_index = vertex_index;
            do {
                // Add vertex to contour.
                contour.push_back(it_vertex_index);

                // Find connected vertex on line.
                const line& line = lines[vertices[it_vertex_index].line_index];
                const std::size_t connected_vertex_index = line.vertex_indices[1];

                // Set vertices as assigned to contour.
                vertices[it_vertex_index].assigned_contour = true;
                vertices[connected_vertex_index].assigned_contour = true;

                // Find other vertex starting at connected vertex.
                find_equal_vertex(connected_vertex_index, vertices, it_vertex_index);

            } while (!vertices[it_vertex_index].assigned_contour);

            // Finish contour by connecting tail and root.
            contour.push_back(contour.front());
        }
    }

    // Create outlines.
    out_outline_count = (unsigned int)contours.size();
    out_outlines = new ptg_outline[out_outline_count];
    for (std::size_t countour_index = 0; countour_index < contours.size(); ++countour_index) {
        std::vector<std::size_t>& contour = contours[countour_index];
        ptg_outline& out_outline = out_outlines[(unsigned int)countour_index];
        out_outline.vertex_count = (unsigned int)contour.size();
        out_outline.vertices = new ptg_vec2[out_outline.vertex_count];
        for (std::size_t vertex_index = 0; vertex_index < contour.size(); ++vertex_index) {
            out_outline.vertices[(unsigned int)vertex_index] = vertices[contour[vertex_index]].position;
        }
    }
}
