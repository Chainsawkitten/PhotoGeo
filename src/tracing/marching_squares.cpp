#include "marching_squares.hpp"

#include <assert.h>
#include <vector>

// Forward declarations.
struct line;
struct vertex;

// Straight line between two vertices.
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
        // B -> C, D -> A
        case 5:
            add_line(B, C, out_lines, out_vertices);
            add_line(D, A, out_lines, out_vertices);
            break;

        // A -> B, C -> D
        case 10:
            add_line(A, B, out_lines, out_vertices);
            add_line(C, D, out_lines, out_vertices);

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
 * @param out_found_index Variable to store vertex index of the found vertex.
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

/*
 * Tracing along contour to find all vertices of an outline.
 * @param vertex_index Vertex index of vertex to find other vertex with same position.
 * @param vertices The vertices generated from marching squares.
 * @param lines The lines generated from marching squares.
 * @param out_contour Vector to store vertices.
 */
static void trace_contour(const std::size_t vertex_index, std::vector<vertex>& vertices, const std::vector<line>& lines, std::vector<std::size_t>& out_contour) {
    // Add vertex to contour.
    out_contour.push_back(vertex_index);

    // Find connected vertex on line.
    const line& line = lines[vertices[vertex_index].line_index];
    std::size_t connected_vertex_index = line.vertex_indices[0] == vertex_index ? line.vertex_indices[1] : line.vertex_indices[0];
    
    // Set vertices as assigned to contour.
    vertices[vertex_index].assigned_contour = true;
    vertices[connected_vertex_index].assigned_contour = true;

    // Find other vertex starting at connected vertex.
    std::size_t found_vertex_index;
    find_equal_vertex(connected_vertex_index, vertices, found_vertex_index);

    // Check whether found vertex is assigned.
    if (!vertices[found_vertex_index].assigned_contour) {
        // Continue to trace contour.
        trace_contour(found_vertex_index, vertices, lines, out_contour);
    } else {
        // Finnish contour by connecting tail and root.
        out_contour.push_back(out_contour.front());
    }
}

void trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count) {

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
            // Trace contour (assign connected vertex indices to contour).
            trace_contour(vertex_index, vertices, lines, contours.back());
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
    //out_outline_count = static_cast<unsigned int>(vertices.size() / 2);
    //out_outlines = new ptg_outline[out_outline_count];
    //for (unsigned int outline_index = 0; outline_index < out_outline_count; ++outline_index) {
    //    out_outlines[outline_index].vertex_count = 2;
    //    out_outlines[outline_index].vertices = new ptg_vec2[2];
    //    out_outlines[outline_index].vertices[0] = vertices[outline_index * 2].position;
    //    out_outlines[outline_index].vertices[1] = vertices[outline_index * 2 + 1].position;
    //}
}
