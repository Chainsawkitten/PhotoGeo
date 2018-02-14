#include "svg.hpp"

#include <iostream>
#include <fstream>

void WriteSVG(const char* filename, unsigned int layers, ptg_color* colors, ptg_outline** outlines, unsigned int* outline_counts, bool markers) {
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

        // Outlines.
        for (unsigned int outline_index = 0; outline_index < outline_counts[layer]; ++outline_index) {
            const ptg_outline& outline = outlines[layer][outline_index];

            // Lines.
            for (unsigned int vertex_index = 0; vertex_index < outline.vertex_count - 1; ++vertex_index) {
                file << "    <line x1=\"" << outline.vertices[vertex_index].x
                    << "\" y1=\"" << outline.vertices[vertex_index].y
                    << "\" x2=\"" << outline.vertices[vertex_index + 1].x
                    << "\" y2=\"" << outline.vertices[vertex_index + 1].y
                    << "\" stroke-width=\"2\" stroke=\"rgb(" << (int)colors[layer].r << ", " << (int)colors[layer].g << ", " << (int)colors[layer].b << ")\" />\n";
            }

            // Markers.
            if (markers) {
                for (unsigned int vertex_index = 0; vertex_index < outline.vertex_count; ++vertex_index) {
                    file << "    <rect x=\"" << (int)outline.vertices[vertex_index].x - 2
                        << "\" y=\"" << (int)outline.vertices[vertex_index].y - 2
                        << "\" width=\"4\" height=\"4\""
                        << " stroke-width=\"0\" fill=\"rgb(" << (int)colors[layer].r << ", " << (int)colors[layer].g << ", " << (int)colors[layer].b << ")\" />\n";
                }
            }
        }

        file << "  </g>\n";
    }

    // Write end tag.
    file << "</svg>\n";

    file.close();
}
