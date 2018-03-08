#include "read_svg.hpp"

#include <iostream>
#include <tinyxml2.h>

ptg_tracing_results read_svg(const char* filename) {
    std::cerr << "read_svg has not yet been implemented." << std::endl;

    // Parse SVG file.
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename);

    // TODO Parse layers.
    // TODO Parse contours.

    ptg_tracing_results results;
    results.layer_count = 0;
    results.outline_counts = nullptr;
    results.outlines = nullptr;

    return results;
}
