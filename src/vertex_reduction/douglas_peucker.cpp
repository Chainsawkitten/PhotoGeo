#include "douglas_peucker.hpp"

#include <iostream>
#include <cstring>

/*
 * Reduce vertex count in outline using Douglas-Peucker.
 * @param outline The outline to reduce.
 */
static void reduce_outline(ptg_outline& outline) {
    // Find the two points farthest from each other.
    unsigned int max_first_point = 0;
    unsigned int max_last_point = 0;
    double max_distance = 0.0;
    for (unsigned int first_point = 0; first_point < outline.vertex_count - 1; ++first_point) {
        for (unsigned int last_point = first_point + 1; last_point < outline.vertex_count; ++last_point) {
            const ptg_vec2 p1 = outline.vertices[first_point];
            const ptg_vec2 p2 = outline.vertices[last_point];
            const double delta_x = p2.x - p1.x;
            const double delta_y = p2.y - p1.y;
            const double distance = delta_x * delta_x + delta_y * delta_y;
            if (distance > max_distance) {
                max_distance = distance;
                max_first_point = first_point;
                max_last_point = last_point;
            }
        }
    }

    // Allocate buffer for whether points should be kept.
    bool* keep = new bool[outline.vertex_count];
    memset(keep, 1, outline.vertex_count);

    // TODO: Apply Douglas-Peucker recursively on both lines.
    // TODO: Generate final outline.

    delete[] keep;
}

void ptgi_douglas_peucker(ptg_tracing_results* tracing_results) {
    std::cout << "ptgi_douglas_peucker has not yet been implemented." << std::endl;

    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline) {
            reduce_outline(tracing_results->outlines[layer][outline]);
        }
    }
}
