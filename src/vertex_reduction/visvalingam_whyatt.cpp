#include "visvalingam_whyatt.hpp"

#include <iostream>
#include <limits>

struct vertex {
    ptg_vec2 position;
    unsigned int area;
    vertex* previous;
    vertex* next;
};

/*
 * Calculate the area*2 of a vertex.
 * @param v Vertex to calculate area of.
 * @return The area of the vertex.
 */
unsigned int calculate_double_area(vertex* v) {
    // Calculate vectors from v to other points in triangle.
    const unsigned int ux = v->previous->position.x - v->position.x;
    const unsigned int uy = v->previous->position.y - v->position.y;
    const unsigned int vx = v->next->position.x - v->position.x;
    const unsigned int vy = v->next->position.y - v->position.y;

    return abs(ux * vy - uy * vx);
}

/*
 * Reduce vertex count in outline using Visvalingam-Whyatt.
 * @param outline The outline to reduce.
 */
void reduce_outline(ptg_outline& outline) {
    // Initialize vertices.
    vertex* first = nullptr;
    vertex** current = &first;
    vertex* previous = first;
    for (unsigned int i = 0; i < outline.vertex_count; ++i) {
        *current = new vertex;
        (*current)->position = outline.vertices[i];
        (*current)->previous = previous;
        (*current)->next = nullptr;

        previous = *current;
        current = &(*current)->next;
    }

    // Calculate initial areas (except for first and last vertex).
    for (vertex* node = first->next; node->next != nullptr; node = node->next)
        node->area = calculate_double_area(node);

    // Find vertex with smallest area.
    unsigned int smallest_area = std::numeric_limits<unsigned int>::max();
    vertex* smallest_vertex = nullptr;
    for (vertex* node = first->next; node->next != nullptr; node = node->next) {
        if (node->area < smallest_area) {
            smallest_area = node->area;
            smallest_vertex = node;
        }
    }

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
