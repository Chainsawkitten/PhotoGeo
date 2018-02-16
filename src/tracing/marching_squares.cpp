#include "marching_squares.hpp"

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
    out_vertices.push_back({ v1, out_lines.size() });
    out_vertices.push_back({ v2, out_lines.size() });
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

void ptgi_trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count) {

    std::vector<line> lines;
    std::vector<vertex> vertices;

    // Execute marching squares on layer.
    for (unsigned int it = 0; it < layer_width * layer_height; ++it) {
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


    // Create outlines.
    out_outline_count = static_cast<unsigned int>(vertices.size() / 2);
    out_outlines = new ptg_outline[out_outline_count];
    for (unsigned int outline_index = 0; outline_index < out_outline_count; ++outline_index) {
        out_outlines[outline_index].vertex_count = 2;
        out_outlines[outline_index].vertices = new ptg_vec2[2];
        out_outlines[outline_index].vertices[0] = vertices[outline_index * 2].position;
        out_outlines[outline_index].vertices[1] = vertices[outline_index * 2 + 1].position;
    }
}
