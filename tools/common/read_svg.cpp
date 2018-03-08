#include "read_svg.hpp"

#include <iostream>
#include <vector>
#include <tinyxml2.h>

using namespace tinyxml2;

struct svg_layer {
    std::vector<ptg_outline> outlines;
};

ptg_tracing_results read_svg(const char* filename) {
    std::cerr << "read_svg has not yet been implemented." << std::endl;

    // Parse SVG file.
    XMLDocument doc;
    doc.LoadFile(filename);
    const XMLElement* root = doc.FirstChildElement("svg");

    // Parse layers.
    std::vector<svg_layer> layers;
    const XMLElement* group = root->FirstChildElement("g");
    while (group != nullptr) {
        svg_layer layer;

        // TODO Parse contours within the layer.

        layers.push_back(layer);

        group = group->NextSiblingElement("g");
    }

    ptg_tracing_results results;
    results.layer_count = layers.size();
    results.outline_counts = new unsigned int[layers.size()];
    for (unsigned int layer = 0; layer < layers.size(); ++layer) {
        results.outline_counts[layer] = 0;
    }
    results.outlines = nullptr;

    return results;
}
