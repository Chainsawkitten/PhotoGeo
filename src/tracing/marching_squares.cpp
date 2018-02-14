#include "marching_squares.hpp"

#include <vector>

// Contains two vertices creating a line.
struct line {
    // The vertices.
    ptg_vec2* vertices[2];
};

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
static void marching_squares(bool topLeft, bool topRight, bool bottomRight, bool bottomLeft, unsigned int x, unsigned int y, std::vector<line>& out_lines, std::vector<ptg_vec2>& out_vertices) {

    // Calculate current configuration of marching squares.
    const unsigned int square_configuration = topLeft * 8 + topRight * 4 + bottomRight * 2 + bottomLeft * 1;

    /* Nodes (A, B, C, D) in marching kernel for 2x2 pixels.
     * x: positive right, y: positive down.
     * # -- A -- #
     * |    |    |
     * D--(x,y)--B
     * |    |    |
     * # -- C -- # */
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
            out_vertices.push_back(C);
            out_vertices.push_back(D);
            break;

        // B -> C
        case 2:
            out_vertices.push_back(B);
            out_vertices.push_back(C);
            break;

        // A -> B
        case 4:
            out_vertices.push_back(A);
            out_vertices.push_back(B);
            break;

        // D -> A
        case 8:
            out_vertices.push_back(D);
            out_vertices.push_back(A);
            break;

        // 2 points:
        // B -> D
        case 3:
            out_vertices.push_back(B);
            out_vertices.push_back(D);
            break;
        
        // A -> C
        case 6:
            out_vertices.push_back(A);
            out_vertices.push_back(C);
            break;

        // C -> A
        case 9:
            out_vertices.push_back(C);
            out_vertices.push_back(A);
            break;

        // D -> B
        case 12:
            out_vertices.push_back(D);
            out_vertices.push_back(B);
            break;

        // 2 points (diagonal):
        // B -> C, D -> A
        case 5:
            out_vertices.push_back(B);
            out_vertices.push_back(C);

            out_vertices.push_back(D);
            out_vertices.push_back(A);
            break;

        // A -> B, C -> D
        case 10:
            out_vertices.push_back(A);
            out_vertices.push_back(B);

            out_vertices.push_back(C);
            out_vertices.push_back(D);
            break;

        // 3 points:
        // D -> A
        case 7:
            out_vertices.push_back(D);
            out_vertices.push_back(A);
            break;

        // A -> B
        case 11:
            out_vertices.push_back(A);
            out_vertices.push_back(B);
            break;

        // B -> C
        case 13:
            out_vertices.push_back(B);
            out_vertices.push_back(C);
            break;

        // C -> D
        case 14:
            out_vertices.push_back(C);
            out_vertices.push_back(D);
            break;

        // 4 points:
        case 15:
            break;
    }
}

void trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count) {

    std::vector<line> lines;
    std::vector<ptg_vec2> vertices;

    // Execute marching squares on layer.
    for (unsigned int it = 0; it < layer_width * (layer_height - 1); ++it)
        if ((it + 1) % layer_width != 0)
            marching_squares(layer[it], layer[it + 1], layer[it + layer_width], layer[it + layer_width + 1], it % layer_width + 1, it / layer_width + 1, lines, vertices);

    // Create outlines.
    out_outline_count = static_cast<unsigned int>(vertices.size() / 2);
    out_outlines = new ptg_outline[out_outline_count];
    for (unsigned int outline_index = 0; outline_index < out_outline_count; ++outline_index) {
        out_outlines[outline_index].vertex_count = 2;
        out_outlines[outline_index].vertices = new ptg_vec2[2];
        out_outlines[outline_index].vertices[0] = vertices[outline_index * 2];
        out_outlines[outline_index].vertices[1] = vertices[outline_index * 2 + 1];
    }
}
