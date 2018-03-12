#include "marching_squares.hpp"

#include <assert.h>
#include <limits>
#include <vector>


// Straight line connecting two vertices. @todo Optimize marching squares and remove this.
struct line {
    // Vertex indices.
    std::size_t vertex_indices[2];

    // Whether line is assigned a contour.
    bool assigned_contour;
};

// Vertex contaning position and line index.
struct vertex {
    // The postion of the vertex.
    ptg_vec2 position;

    // Line index which this vertex belongs.
    std::size_t line_index;
};

/*
 * Create vertices and add line to vectors.
 * @param v1 First vertex.
 * @param v2 Second vertex.
 * @param out_lines Vector to store resulting lines.
 * @param out_vertices Vector to store resulting vertices.
 */
static void add_line(const ptg_vec2& v1, const ptg_vec2& v2, std::vector<line>& out_lines, std::vector<vertex>& out_vertices) {
    out_lines.push_back({ out_vertices.size(), out_vertices.size() + 1 , false });
    out_vertices.push_back({ v1, out_lines.size() - 1 });
    out_vertices.push_back({ v2, out_lines.size() - 1 });
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
        // D -> C
        case 1:
            add_line(D, C, out_lines, out_vertices);
            break;

        // C -> B
        case 2:
            add_line(C, B, out_lines, out_vertices);
            break;

        // B -> A
        case 4:
            add_line(B, A, out_lines, out_vertices);
            break;

        // A -> D
        case 8:
            add_line(A, D, out_lines, out_vertices);
            break;

        // 2 points:
        // D -> B
        case 3:
            add_line(D, B, out_lines, out_vertices);
            break;
        
        // C -> A
        case 6:
            add_line(C, A, out_lines, out_vertices);
            break;

        // A -> C
        case 9:
            add_line(A, C, out_lines, out_vertices);
            break;

        // B -> D
        case 12:
            add_line(B, D, out_lines, out_vertices);
            break;

        // 2 points (diagonal):
        // B -> A, D -> C
        case 5:
            add_line(B, A, out_lines, out_vertices);
            add_line(D, C, out_lines, out_vertices);
            break;

        // A -> D, C -> B
        case 10:
            add_line(A, D, out_lines, out_vertices);
            add_line(C, B, out_lines, out_vertices);
            break;

        // 3 points:
        // D -> A
        case 7:
            add_line(D, A, out_lines, out_vertices);
            break;

        // A -> B
        case 11:
            add_line(A, B, out_lines, out_vertices);
            break;

        // B -> C
        case 13:
            add_line(B, C, out_lines, out_vertices);
            break;

        // C -> D
        case 14:
            add_line(C, D, out_lines, out_vertices);
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
 * @return The vertex index of the found vertex.
 * @todo Optimize using lookup table.
 */
static std::size_t find_equal_vertex(const std::size_t this_index, const std::vector<vertex>& vertices) {
    const vertex& this_vertex = vertices[this_index];
    for (std::size_t vertex_index = 0; vertex_index < vertices.size(); ++vertex_index) {
        const vertex& vertex = vertices[vertex_index];
        if ((this_index != vertex_index) && (vertex.position.x == this_vertex.position.x) && (this_vertex.position.y == vertex.position.y))
            return vertex_index;
    }
    return std::numeric_limits<std::size_t>::max();
}

void ptgi_trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count) {

    std::vector<line> lines;
    std::vector<vertex> vertices;

    // Execute marching squares on layer.
    for (unsigned int it = 0; it < ((layer_width + 1) * (layer_height + 1)); ++it) {
        
        unsigned int it_x = it % (layer_width + 1);
        unsigned int it_y = it / (layer_width + 1);

        bool topEdge = it_y == 0;
        bool bottomEdge = it_y == layer_height;
        bool leftEdge = it_x == 0;
        bool rightEdge = it_x == layer_width;

        bool topLeft = topEdge || leftEdge ? false : layer[it_x - 1 + (it_y - 1) * layer_width];
        bool topRight = topEdge || rightEdge ? false : layer[it_x + (it_y - 1) * layer_width];
        bool bottomRight = bottomEdge || rightEdge ? false : layer[it_x + it_y * layer_width];
        bool bottomLeft = bottomEdge || leftEdge ? false : layer[it_x - 1 + it_y * layer_width];

        marching_squares(topLeft, topRight, bottomRight, bottomLeft, it_x, it_y, lines, vertices);
    }

    // Create contours.
    std::vector<std::vector<std::size_t>> contours;
    for (std::size_t line_index = 0; line_index < lines.size(); ++line_index) {
        if (!lines[line_index].assigned_contour) {

            // Create new contour.
            contours.push_back(std::vector<std::size_t>());
            std::vector<std::size_t>& contour = contours.back();

            // Trace contour (assign connected vertex indices to contour).
            line* line = &lines[line_index];
            do {
                // Add vertex index to contour.
                contour.push_back(line->vertex_indices[0]);

                // Set line as assigned to contour.
                line->assigned_contour = true;

                const vertex& this_vertex = vertices[line->vertex_indices[1]];
                const vertex& other_vertex = vertices[find_equal_vertex(line->vertex_indices[1], vertices)];
                assert((other_vertex.position.x == this_vertex.position.x) && (other_vertex.position.y == this_vertex.position.y));
                assert(this_vertex.line_index != other_vertex.line_index);

                // Find connected line.
                line = &lines[vertices[find_equal_vertex(line->vertex_indices[1], vertices)].line_index];
            } while (!line->assigned_contour);

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
