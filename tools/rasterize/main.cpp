#include <iostream>
#include "read_svg.hpp"
#include "rasterize.hpp"
#include "scale.hpp"
#include "vertex_count.hpp"
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* input_filename = "";
    const char* output_filename = "";
    unsigned int scale = 1;
    const char* vertex_count_filename = "";

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                input_filename = argv[++argument];

            // Output filename.
            else if (argv[argument][1] == 'o' && argc > argument + 1)
                output_filename = argv[++argument];

            // Scale.
            else if (argv[argument][1] == 's' && argc > argument + 1)
                scale = std::stoi(argv[++argument]);

            // Vertex count filename.
            else if (argv[argument][1] == 'v' && argc > argument + 1)
                vertex_count_filename = argv[++argument];
        }
    }

    // Display help if no valid configuration was given.
    if (input_filename[0] == '\0' || output_filename[0] == '\0') {
        std::cout << "usage: rasterize -i input_filename -o output_filename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source SVG." << std::endl;
        std::cout << "  -o  Specify filename of result PNG." << std::endl;
        std::cout << "  -s  Specify how the image should be scaled." << std::endl
                  << "      Integer values only." << std::endl;
        std::cout << "  -v  Specify filename of vertex count log file." << std::endl;

        return 0;
    }

    // Load SVG image.
    unsigned int width;
    unsigned int height;
    ptg_color* colors;
    ptg_tracing_results svg;
    read_svg(input_filename, &svg, &colors, &width, &height);

    // Log vertex count.
    if (vertex_count_filename[0] != '\0')
        log_vertex_count(vertex_count_filename, &svg);

    // Scale image.
    width *= scale;
    height *= scale;
    scale_svg(&svg, scale);

    // Allocate image data.
    ptg_color* image_data = new ptg_color[width * height];

    // Rasterize.
    rasterize(&svg, colors, width, height, image_data);

    // Free SVG results.
    free_svg_results(&svg, &colors);

    // Write image to PNG file.
    const unsigned int components = 3;
    stbi_write_png(output_filename, width, height, components, image_data, width * components);

    // Clean up image data.
    delete[] image_data;

    return 0;
}
