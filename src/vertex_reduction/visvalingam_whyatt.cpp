#include "visvalingam_whyatt.hpp"

#include <cmath>
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
static unsigned int calculate_double_area(vertex* v) {
    // Calculate vectors from v to other points in triangle.
    const long ux = (long)v->previous->position.x - v->position.x;
    const long uy = (long)v->previous->position.y - v->position.y;
    const long vx = (long)v->next->position.x - v->position.x;
    const long vy = (long)v->next->position.y - v->position.y;

    return std::abs(ux * vy - uy * vx);
}

/*
 * Reduce vertex count in outline using Visvalingam-Whyatt.
 * @param outline The outline to reduce.
 */
static void reduce_outline(ptg_outline& outline) {
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

    while (true) {
        // Find vertex with smallest area.
        unsigned int smallest_area = std::numeric_limits<unsigned int>::max();
        vertex* smallest_vertex = nullptr;
        for (vertex* node = first->next; node->next != nullptr; node = node->next) {
            if (node->area < smallest_area) {
                smallest_area = node->area;
                smallest_vertex = node;
            }
        }

        // Remove it if smaller than certain value.
        // TODO: Make threshold configurable.
        const unsigned int threshold = 20 * 2;
        if (smallest_area <= threshold) {
            smallest_vertex->previous->next = smallest_vertex->next;
            smallest_vertex->next->previous = smallest_vertex->previous;

            // Recalculate area of neighbor vertices.
            if (smallest_vertex->previous->previous != nullptr)
                smallest_vertex->previous->area = calculate_double_area(smallest_vertex->previous);

            if (smallest_vertex->next->next != nullptr)
                smallest_vertex->next->area = calculate_double_area(smallest_vertex->next);

            delete smallest_vertex;
        } else {
            break;
        }
    }

    // Store output vertices.
    outline.vertex_count = 0;
    for (vertex* node = first; node != nullptr; node = node->next)
        outline.vertices[outline.vertex_count++] = node->position;

    // Cleanup.
    while (first->next != nullptr) {
        vertex* next = first->next;
        delete first;
        first = next;
    }
}

void ptgi_visvalingam_whyatt(ptg_tracing_results* tracing_results) {
    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline) {
            reduce_outline(tracing_results->outlines[layer][outline]);
        }

        // Remove outlines that have been reduced down to a single point.
        unsigned int outline_count = 0;
        for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline) {
            if (tracing_results->outlines[layer][outline].vertex_count > 2) {
                tracing_results->outlines[layer][outline_count].vertex_count = tracing_results->outlines[layer][outline].vertex_count;
                tracing_results->outlines[layer][outline_count].vertices = tracing_results->outlines[layer][outline].vertices;
                ++outline_count;
            } else {
                delete[] tracing_results->outlines[layer][outline].vertices;
            }
        }

        tracing_results->outline_counts[layer] = outline_count;
    }
}
