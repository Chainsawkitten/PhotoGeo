#include "vertex_count.hpp"

#include <fstream>

void log_vertex_count(const char* filename, const ptg_tracing_results* svg) {
    unsigned int vertex_count = 0;
    for (unsigned int layer = 0; layer < svg->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < svg->outline_counts[layer]; ++outline) {
            vertex_count += svg->outlines[layer][outline].vertex_count;
        }
    }

    std::ofstream file(filename);
    file << vertex_count;
    file.close();
}
