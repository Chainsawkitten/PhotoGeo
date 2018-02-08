#include "svg.hpp"

#include <iostream>
#include <fstream>

void WriteSVG(const char* filename, unsigned int layers, ptg_color* colors, unsigned int* vertexCount, ptg_vec2** vertices) {
    // Open output file.
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open " << filename << "." << std::endl;
        return;
    }

    // Write header.
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
         << "<svg xmlns=\"http://www.w3.org/2000/svg\">\n";

    // Layers.
    for (unsigned int layer = 0; layer < layers; ++layer) {
        file << "  <g id=\"layer" << layer << "\">\n";

        // Vertices.
        for (unsigned int vertex = 0; vertex < vertexCount[0] - 1; ++vertex) {
            file << "    <line x1=\"" << vertices[layer][vertex].x
                 << "\" y1=\"" << vertices[layer][vertex].y
                 << "\" x2=\"" << vertices[layer][vertex + 1].x
                 << "\" y2=\"" << vertices[layer][vertex + 1].y
                 << "\" stroke-width=\"2\" stroke=\"rgb(" << (int)colors[layer].r << ", " << (int)colors[layer].g << ", " << (int)colors[layer].b << ")\" />\n";
        }

        file << "  </g>\n";
    }

    // Write end tag.
    file << "</svg>\n";

    file.close();
}
