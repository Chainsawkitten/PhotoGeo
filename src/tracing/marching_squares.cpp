#include "marching_squares.hpp"

#include <assert.h>
#include <limits>
#include <vector>

// Node is used in marching squares and contour tracing.
struct node {
    // Configuration id.
    int configuration;

    // Whether node is assigned a contour.
    bool assigned;
};

// Direction describes which direction the contour is heading during contour tracing.
enum  direction {
    NONE, //< No direction.
    UP, //< Up direction.
    DOWN, //< Down direction.
    LEFT, //< Left direction.
    RIGHT //< Right direction.
};

/*
 * Calculating the nodes vertices and direction.
 * @param x X-coordinate in pixels (positive right).
 * @param y Y-coordinate in pixels (positive down).
 * @param direction Current direction of the contour.
 * @param node The node to decode.
 * @param out_v0 The first vertex.
 * @param out_v1 The second vertex.
 * @return Which direction the contour is heading.
 */
static direction calculate_node(unsigned int x, unsigned int y, direction direction, const node& node, ptg_vec2& out_v0, ptg_vec2& out_v1) {

    // Convert (x,y) from pixel space to mesh space.
    x = x * 2 + 1;
    y = y * 2 + 1;
    /* Nodes (A, B, C, D) in marching kernel for 2x2 pixels.
     * x: positive right, y: positive down.
     * Lines are created in clockwise winding order.
     * # ------- #
     * |    A    |
     * | D(x,y)B |
     * |    C    |
     * # ------- # */
    const ptg_vec2 A = { x, y - 1 };
    const ptg_vec2 B = { x + 1, y };
    const ptg_vec2 C = { x, y + 1 };
    const ptg_vec2 D = { x - 1, y };

    switch (node.configuration) {
        // 0 points:    
        case 0:
            return NONE;

        // 1 points:
        // D -> C
        case 1:
            out_v0 = D;
            out_v1 = C;
            return DOWN;

        // C -> B
        case 2:
            out_v0 = C;
            out_v1 = B;
            return RIGHT;

        // B -> A
        case 4:
            out_v0 = B;
            out_v1 = A;
            return UP;

        // A -> D
        case 8:
            out_v0 = A;
            out_v1 = D;
            return LEFT;

        // 2 points:
        // D -> B
        case 3:
            out_v0 = D;
            out_v1 = B;
            return RIGHT;
        
        // C -> A
        case 6:
            out_v0 = C;
            out_v1 = A;
            return UP;

        // A -> C
        case 9:
            out_v0 = A;
            out_v1 = C;
            return DOWN;

        // B -> D
        case 12:
            out_v0 = B;
            out_v1 = D;
            return LEFT;

        // 2 points (diagonal):
        // D -> C, B -> A
        case 5:
            if (direction == RIGHT) {
                out_v0 = D;
                out_v1 = C;
                return DOWN;
            } else if (direction == LEFT) {
                out_v0 = B;
                out_v1 = A;
                return UP;
            }
            return NONE;

        // C -> B, A -> D
        case 10:
            if (direction == UP) {
                out_v0 = C;
                out_v1 = B;
                return RIGHT;
            } else if (direction == DOWN) {
                out_v0 = A;
                out_v1 = D;
                return LEFT;
            }
            return NONE;

        // 3 points:
        // D -> A
        case 7:
            out_v0 = D;
            out_v1 = A;
            return UP;

        // A -> B
        case 11:
            out_v0 = A;
            out_v1 = B;
            return RIGHT;

        // B -> C
        case 13:
            out_v0 = B;
            out_v1 = C;
            return DOWN;

        // C -> D
        case 14:
            out_v0 = C;
            out_v1 = D;
            return LEFT;

        // 4 points:
        case 15:
            return NONE;

        default:
            return NONE;
    }
}

void ptgi_trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count) {

    // Allocate nodes.
    node* nodes = new node[(layer_width + 1) * (layer_height + 1)];
    std::vector<unsigned int> root_indices;

    // Execute marching squares on layer.
    for (unsigned int it = 0; it < ((layer_width + 1) * (layer_height + 1)); ++it) {
        
        unsigned int it_x = it % (layer_width + 1);
        unsigned int it_y = it / (layer_width + 1);

        bool top_edge = it_y == 0;
        bool bottom_edge = it_y == layer_height;
        bool left_edge = it_x == 0;
        bool right_edge = it_x == layer_width;

        bool top_left = top_edge || left_edge ? false : layer[it_x - 1 + (it_y - 1) * layer_width];
        bool top_right = top_edge || right_edge ? false : layer[it_x + (it_y - 1) * layer_width];
        bool bottom_right = bottom_edge || right_edge ? false : layer[it_x + it_y * layer_width];
        bool bottom_left = bottom_edge || left_edge ? false : layer[it_x - 1 + it_y * layer_width];

        int configuration = top_left * 8 + top_right * 4 + bottom_right * 2 + bottom_left * 1;

        // Calculate current configuration of marching squares.
        nodes[it] = { configuration, false };
        // Add root if configuration is 2 or 10
        if ((configuration == 2) || (configuration == 10))
            root_indices.push_back(it);
    }

    // Create contours.
    std::vector<std::vector<ptg_vec2>> contours;
    for (int root_index : root_indices) {
        node* node_root = &nodes[root_index];
        if (!node_root->assigned) {

            // Create new contour.
            contours.push_back(std::vector<ptg_vec2>());
            std::vector<ptg_vec2>& contour = contours.back();
    
            unsigned int it = root_index;
            unsigned int x = it % (layer_width + 1);
            unsigned int y = it / (layer_width + 1);
            direction direction = node_root->configuration == 10 ? UP : RIGHT;
            node* node_it = node_root;
            ptg_vec2 v0, v1;

            // Trace contour.
            do {
                // Calculating the nodes vertices and direction.
                direction = calculate_node(x, y, direction, *node_it, v0, v1);
                assert(direction != NONE);

                // Add vertex to contour.
                contour.push_back(v0);

                // Set node as assigned to contour expect when tracing configuration 10 and direction is left.
                if (node_it->configuration != 10 || direction != LEFT)
                    node_it->assigned = true;

                // Find connected node.
                x = x + (direction == RIGHT) - (direction == LEFT);
                y = y + (direction == DOWN) - (direction == UP);
                it = x + y * (layer_width + 1);
                node_it = &nodes[it];
            } while (node_it != node_root || direction == DOWN);

            // Finish contour by connecting tail and root.
            contour.push_back(contour.front());
        }
    }

    // Create outlines.
    out_outline_count = (unsigned int)contours.size();
    out_outlines = new ptg_outline[out_outline_count];
    for (std::size_t countour_index = 0; countour_index < contours.size(); ++countour_index) {
        std::vector<ptg_vec2>& contour = contours[countour_index];
        ptg_outline& out_outline = out_outlines[(unsigned int)countour_index];
        out_outline.vertex_count = (unsigned int)contour.size();
        out_outline.vertices = new ptg_vec2[out_outline.vertex_count];
        memcpy(out_outline.vertices, contour.data(), sizeof(ptg_vec2) * out_outline.vertex_count);
    }

    // Delete nodes.
    delete[] nodes;
}
