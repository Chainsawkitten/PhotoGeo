#include "douglas_peucker.hpp"

#include <iostream>

/*
 * Reduce vertex count in outline using Douglas-Peucker.
 * @param outline The outline to reduce.
 */
static void reduce_outline(ptg_outline& outline) {
    // TODO: Implement.
}

void ptgi_douglas_peucker(ptg_tracing_results* tracing_results) {
    std::cout << "ptgi_douglas_peucker has not yet been implemented." << std::endl;

    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline) {
            reduce_outline(tracing_results->outlines[layer][outline]);
        }
    }
}
