#include "svg.hpp"

#include <iostream>
#include <fstream>

void write_svg(const char* filename, const ptg_image_parameters* image_parameters, ptg_outline** outlines, unsigned int* outline_counts, bool markers) {
    // Open output file.
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open " << filename << "." << std::endl;
        return;
    }

    // Write header.
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
         << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
         << "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
         << "   width=\"" << image_parameters->width * 2 << "\""
         << "   height=\"" << image_parameters->height * 2 << "\""
         << ">\n";

    // Layers.
    for (unsigned int layer = 0; layer < image_parameters->color_layer_count; ++layer) {
        file << "  <g id=\"layer" << layer << "\">\n";
        const ptg_color color = image_parameters->color_layer_colors[layer];

        // Outlines.
        for (unsigned int outline_index = 0; outline_index < outline_counts[layer]; ++outline_index) {
            const ptg_outline& outline = outlines[layer][outline_index];

            // Path.
            file << "    <path\n"
                 << "       style=\"fill:none;stroke:rgb(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ");stroke-width:2px\"\n"
                 << "       inkscape:connector-curvature=\"0\"\n"
                 << "       d=\"M";
            for (unsigned int vertex_index = 0; vertex_index < outline.vertex_count; ++vertex_index)
                file << " " << outline.vertices[vertex_index].x << "," << outline.vertices[vertex_index].y;
            file << " Z\" />\n";
        
            // Markers.
            if (markers) {
                for (unsigned int vertex_index = 0; vertex_index < outline.vertex_count; ++vertex_index) {
                    file << "    <rect x=\"" << (int)outline.vertices[vertex_index].x - 2
                         << "\" y=\"" << (int)outline.vertices[vertex_index].y - 2
                         << "\" width=\"4\" height=\"4\""
                         << " stroke-width=\"0\" fill=\"rgb(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ")\" />\n";
                }
            }
        }
        file << "  </g>\n";
    }

    // Write end tag.
    file << "</svg>\n";

    file.close();
}
