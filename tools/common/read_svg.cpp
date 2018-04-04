#include "read_svg.hpp"

#include <cstring>
#include <math.h>
#include <vector>
#include <tinyxml2.h>
#include <string>

using namespace tinyxml2;

struct svg_layer {
    std::vector<ptg_outline> outlines;
    ptg_color color;
};

/*
 * Parse number from string.
 * @param c The string to parse.
 * @return Rounded value.
 */
static long parse_number(const char*& c) {
    // Check sign.
    int sign = 1;
    if (*c == '-') {
        sign = -1;
        ++c;
    }
    
    // Numbers.
    double n = 0.0;
    for (; *c >= '0' && *c <= '9'; ++c) {
        n *= 10;
        n += *c - '0';
    }

    // Discard dot.
    if (*c == '.')
        ++c;

    // Decimals.
    double d = 1.0;
    for (; (*c >= '0' && *c <= '9'); ++c) {
        d *= 10;
        n += (*c - '0') / d;
    }
    
    // Return rounded value.
    return static_cast<long>(sign * round(n));
}

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
                if (pos != std::string::npos) {
                    // Red.
                    style = style.substr(pos + 11);
                    layer.color.r = std::stoi(style);

                    // Green.
                    style = style.substr(style.find(",") + 1);
                    layer.color.g = std::stoi(style);

                    // Blue.
                    style = style.substr(style.find(",") + 1);
                    layer.color.b = std::stoi(style);
                } else {
                    const char* d = &style[style.find("stroke:#") + 8];
                    const int A = 'a' + 10;

                    // Red.
                    layer.color.r = ((d[0] >= '0' && d[0] <= '9') ? d[0] - '0' : d[0] - A) * 16 + ((d[1] >= '0' && d[1] <= '9') ? d[1] - '0' : d[1] - A);
                    d += 2;

                    // Green.
                    layer.color.g = ((d[0] >= '0' && d[0] <= '9') ? d[0] - '0' : d[0] - A) * 16 + ((d[1] >= '0' && d[1] <= '9') ? d[1] - '0' : d[1] - A);
                    d += 2;

                    // Blue.
                    layer.color.b = ((d[0] >= '0' && d[0] <= '9') ? d[0] - '0' : d[0] - A) * 16 + ((d[1] >= '0' && d[1] <= '9') ? d[1] - '0' : d[1] - A);
                    d += 2;
                }
            }

            // Parse vertices.
            std::vector<ptg_vec2> vertices;
            const char* d = path->Attribute("d");
            unsigned int x = 0;
            unsigned int y = 0;
            char state;
            // Get all numbers in the string.
            for (; *d != '\0'; ++d) {
                // Discard spaces.
                while (*d == ' ')
                    ++d;
 
                // Update state if not a number.
                if (!((*d >= '0' && *d <= '9') || *d == '-'))
                    state = *d;
                else {
                    switch (state) {
                        case 'M':
                        case 'L':
                            // Parse absolute number pair.
                            x = parse_number(d);
                            ++d;
                            y = parse_number(d);
                            break;
                        case 'm':
                        case 'l':
                            // Parse relative number pair.
                            x = x + parse_number(d);
                            ++d;
                            y = y + parse_number(d);
                            break;
                        case 'H':
                            // Parse absolute horizontal number.
                            x = parse_number(d);
                            break;
                        case 'h':
                            // Parse relative horizontal number.
                            x = x + parse_number(d);
                            break;
                        case 'V':
                            // Parse absolute vertical number.
                            y = parse_number(d);
                            break;
                        case 'v':
                            // Parse relative vertical number.
                            y = y + parse_number(d);
                            break;
                    }
                    vertices.push_back({ x , y });
                }
            }

            // Loop.
            if (state == 'Z' || state == 'z')
                vertices.push_back(vertices[0]);

            ptg_outline outline;
            outline.vertex_count = vertices.size();
            outline.vertices = new ptg_vec2[vertices.size()];
            memcpy(outline.vertices, vertices.data(), vertices.size() * sizeof(ptg_vec2));

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
