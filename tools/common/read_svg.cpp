#include "read_svg.hpp"

#include <vector>
#include <tinyxml2.h>

using namespace tinyxml2;

struct svg_layer {
    std::vector<ptg_outline> outlines;
};

ptg_tracing_results read_svg(const char* filename) {
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
            // Parse vertices.
            std::vector<unsigned int> numbers;
            const char* d = path->Attribute("d");

            // Get all numbers in the string.
            while (*d != '\0') {
                // Get next number in the string.
                for (; !(*d >= '0' && *d <= '9') && *d != '\0'; ++d);

                // Parse number.
                unsigned int n = 0;
                for (; *d >= '0' && *d <= '9'; ++d) {
                    n *= 10;
                    n += *d - '0';
                }
                numbers.push_back(n);

                // Discard decimals.
                for (; (*d >= '0' && *d <= '9') || *d == '.'; ++d);
            }

            ptg_outline outline;
            outline.vertex_count = numbers.size() / 2;
            outline.vertices = new ptg_vec2[numbers.size() / 2];
            for (std::size_t i = 0; i < numbers.size() / 2; ++i) {
                outline.vertices[i].x = numbers[i * 2];
                outline.vertices[i].y = numbers[i * 2 + 1];
            }

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
    results.outlines = new ptg_outline*[layers.size()];
    for (unsigned int layer = 0; layer < layers.size(); ++layer) {
        results.outline_counts[layer] = layers[layer].outlines.size();
        results.outlines[layer] = new ptg_outline[results.outline_counts[layer]];

        for (unsigned int outline = 0; outline < results.outline_counts[layer]; ++outline)
            results.outlines[layer][outline] = layers[layer].outlines[outline];
    }

    return results;
}
