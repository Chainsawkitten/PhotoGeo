#include "read_svg.hpp"

#include <vector>
#include <tinyxml2.h>
#include <string>

using namespace tinyxml2;

struct svg_layer {
    std::vector<ptg_outline> outlines;
    ptg_color color;
};

void read_svg(const char* filename, ptg_tracing_results* results, ptg_color** colors, unsigned int* width, unsigned int* height) {
    // Parse SVG file.
    XMLDocument doc;
    doc.LoadFile(filename);
    const XMLElement* root = doc.FirstChildElement("svg");

    // Parse dimensions.
    *width = root->IntAttribute("width") / 2;
    *height = root->IntAttribute("height") / 2;

    // Parse layers.
    std::vector<svg_layer> layers;
    const XMLElement* group = root->FirstChildElement("g");
    while (group != nullptr) {
        svg_layer layer;
        layer.color = {0, 0, 0};
        bool first = true;

        // Parse contours within the layer.
        const XMLElement* path = group->FirstChildElement("path");
        while (path != nullptr) {
            // Read layer color.
            if (first) {
                first = false;
                std::string style = path->Attribute("style");
                std::size_t pos = style.find("stroke:rgb(");

                // Red.
                style = style.substr(pos + 11);
                layer.color.r = std::stoi(style);

                // Green.
                style = style.substr(style.find(",") + 1);
                layer.color.g = std::stoi(style);

                // Blue.
                style = style.substr(style.find(",") + 1);
                layer.color.b = std::stoi(style);
            }

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

    // Write results.
    results->layer_count = layers.size();
    results->outline_counts = new unsigned int[layers.size()];
    results->outlines = new ptg_outline*[layers.size()];
    *colors = new ptg_color[layers.size()];
    for (unsigned int layer = 0; layer < layers.size(); ++layer) {
        (*colors)[layer] = layers[layer].color;
        results->outline_counts[layer] = layers[layer].outlines.size();
        results->outlines[layer] = new ptg_outline[results->outline_counts[layer]];

        for (unsigned int outline = 0; outline < results->outline_counts[layer]; ++outline)
            results->outlines[layer][outline] = layers[layer].outlines[outline];
    }
}

void free_svg_results(ptg_tracing_results *results, ptg_color **colors) {
    ptg_free_tracing_results(results);
    delete[] *colors;
}
