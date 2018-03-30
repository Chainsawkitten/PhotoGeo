#include "scale.hpp"

void scale_svg(ptg_tracing_results* svg, unsigned int scale) {
    for (unsigned int layer = 0; layer < svg->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < svg->outline_counts[layer]; ++outline) {
            for (unsigned int vertex = 0; vertex < svg->outlines[layer][outline].vertex_count; ++vertex) {
                svg->outlines[layer][outline].vertices[vertex].x *= scale;
                svg->outlines[layer][outline].vertices[vertex].y *= scale;
            }
        }
    }
}
