#include "marching_squares.hpp"

#include <vector>

struct edge {
    unsigned int vertex_index[2];
    bool assigned;
};

static void marching_squares(bool topLeft, bool topRight, bool bottomRight, bool bottomLeft, unsigned int x, unsigned int y, std::vector<edge>& edges, std::vector<ptg_vec2>& vertices) {

    // Calculate current configuration of marching squares.
    const unsigned int configuration = topLeft * 8 + topRight * 4 + bottomRight * 2 + bottomLeft * 1;

    /* Nodes (A, B, C, D) in marching squares for 2x2 pixels.
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

    switch (configuration) {
        // 0 points:    
        case 0:
            break; 

        // 1 points:
        // C -> D
        case 1:
            vertices.push_back(C);
            vertices.push_back(D);
            break;

        // B -> C
        case 2:
            vertices.push_back(B);
            vertices.push_back(C);
            break;

        // A -> B
        case 4:
            vertices.push_back(A);
            vertices.push_back(B);
            break;

        // D -> A
        case 8:
            vertices.push_back(D);
            vertices.push_back(A);
            break;

        // 2 points:
        // B -> D
        case 3:
            vertices.push_back(B);
            vertices.push_back(D);
            break;
        
        // A -> C
        case 6:
            vertices.push_back(A);
            vertices.push_back(C);
            break;

        // C -> A
        case 9:
            vertices.push_back(C);
            vertices.push_back(A);
            break;

        // D -> B
        case 12:
            vertices.push_back(D);
            vertices.push_back(B);
            break;

        // 2 points (diagonal):
        // B -> C, D -> A
        case 5:
            vertices.push_back(B);
            vertices.push_back(C);

            vertices.push_back(D);
            vertices.push_back(A);
            break;

        // A -> B, C -> D
        case 10:
            vertices.push_back(A);
            vertices.push_back(B);

            vertices.push_back(C);
            vertices.push_back(D);
            break;

        // 3 points:
        // D -> A
        case 7:
            vertices.push_back(D);
            vertices.push_back(A);
            break;

        // A -> B
        case 11:
            vertices.push_back(A);
            vertices.push_back(B);
            break;

        // B -> C
        case 13:
            vertices.push_back(B);
            vertices.push_back(C);
            break;

        // C -> D
        case 14:
            vertices.push_back(C);
            vertices.push_back(D);
            break;

        // 4 points:
        case 15:
            break;
    }
}

void trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count) {

    std::vector<edge> edges;
    std::vector<ptg_vec2> vertices;

    // Execute marching squares on layer.
    unsigned int it = 0;
    while (it / (layer_height - 1) < layer_height) {
        if ((it + 1) % layer_width == 0)
            ++it;

        marching_squares(layer[it], layer[it + 1], layer[it + layer_width], layer[it + layer_width + 1], it % layer_width + 1, it / layer_height + 1, edges, vertices);

        ++it;
    }

    out_outline_count = static_cast<unsigned int>(vertices.size() / 2);
    out_outlines = new ptg_outline[out_outline_count];

    for (unsigned int i = 0; i < out_outline_count; ++i) {
        out_outlines[i].vertex_count = 2;
        out_outlines->vertices = new ptg_vec2[2];
        out_outlines->vertices[0] = vertices[i * 2];
        out_outlines->vertices[1] = vertices[i * 2 + 1];
    }
    
}
