#include <iostream>
#include <read_svg.hpp>
#include "rasterize.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* input_filename = "";
    const char* output_filename = "";

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                input_filename = argv[++argument];

            // Output filename.
            if (argv[argument][1] == 'o' && argc > argument + 1)
                output_filename = argv[++argument];
        }
    }

    // Display help if no valid configuration was given.
    if (input_filename[0] == '\0' || output_filename[0] == '\0') {
        std::cout << "usage: rasterize -i input_filename -o output_filename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source SVG." << std::endl;
        std::cout << "  -o  Specify filename of result PNG." << std::endl;

        return 0;
    }

    // Load SVG image.
    ptg_tracing_results svg;
    read_svg(input_filename, &svg);

    std::cerr << "rasterization tool not yet implemented." << std::endl;

    // Allocate image data.
    // TODO: Allow user to configure size (or read it from SVG file).
    unsigned int width = 2000;
    unsigned int height = 1415;
    ptg_color* image_data = new ptg_color[width * height];

    // Rasterize.
    rasterize(&svg, width, height, image_data);

    // Free tracing results.
    ptg_free_tracing_results(&svg);

    // Write image to PNG file.
    const unsigned int components = 3;
    stbi_write_png(output_filename, width, height, components, image_data, width * components);

    // Clean up image data.
    delete[] image_data;

    return 0;
}
