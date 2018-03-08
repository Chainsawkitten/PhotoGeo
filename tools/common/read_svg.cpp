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

        // Parse contours within the layer.
        const XMLElement* path = group->FirstChildElement("path");
        while (path != nullptr) {
            // TODO Parse vertices.

            ptg_outline outline;
            outline.vertex_count = 0;
            outline.vertices = nullptr;
            layer.outlines.push_back(outline);

            path = path->NextSiblingElement("path");
        }

        layers.push_back(layer);

        group = group->NextSiblingElement("g");
    }

    // Return results.
    ptg_tracing_results results;
    results.layer_count = layers.size();
    results.outline_counts = new unsigned int[layers.size()];
    for (unsigned int layer = 0; layer < layers.size(); ++layer) {
        results.outline_counts[layer] = layers[layer].outlines.size();
    }
    results.outlines = nullptr;

    return results;
}
