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
        file << "  </g>\n";
    }

    // Write end tag.
    file << "</svg>\n";

    file.close();
}
