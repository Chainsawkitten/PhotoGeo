#include "visvalingam_whyatt.hpp"

#include <iostream>

struct vertex {
    ptg_vec2 position;
    double area;
    vertex* previous;
    vertex* next;
};

/*
 * Calculate the area of a vertex.
 * @param v Vertex to calculate area of.
 * @return The area of the vertex.
 */
double calculate_area(vertex* v) {
    // TODO: Implement.
    return 0.0;
}

/*
 * Reduce vertex count in outline using Visvalingam-Whyatt.
 * @param outline The outline to reduce.
 */
void reduce_outline(ptg_outline& outline) {
    // Initialize vertices.
    vertex* first = nullptr;
    vertex** current = &first;
    for (unsigned int i = 0; i < outline.vertex_count; ++i) {
        vertex* previous = *current;
        *current = new vertex;
        (*current)->position = outline.vertices[i];
        (*current)->previous = previous;
        (*current)->next = nullptr;
        current = &(*current)->next;
    }

    // TODO: Calculate initial areas.
    // TODO: Find lowest area vertex.
    // TODO: Remove it if smaller than certain value.
    // TODO: Recalculate area of neighbor vertices.
    // TODO: Repeat.

    // TODO: Store output vertices.

    // Cleanup.
    while (first->next != nullptr) {
        vertex* next = first->next;
        delete first;
        first = next;
    }
}

void ptgi_visvalingam_whyatt(ptg_tracing_results* tracing_results) {
    std::cerr << "ptg_visvalingam_whyatt has not yet been implemented." << std::endl;

    unsigned int vertex_count_before = 0;
    unsigned int vertex_count_after = 0;

    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline) {
            vertex_count_before += tracing_results->outlines[layer][outline].vertex_count;
            reduce_outline(tracing_results->outlines[layer][outline]);
            vertex_count_after += tracing_results->outlines[layer][outline].vertex_count;
        }
    }

    std::cout << "Vertex count (before): " << vertex_count_before << std::endl;
    std::cout << "Vertex count (after): " << vertex_count_after << std::endl;
}
