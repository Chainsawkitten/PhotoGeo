#include <iostream>

#include "perturb.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
        std::cout << "usage: perturb -i input_filename -o output_filename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source image." << std::endl;
        std::cout << "  -o  Specify filename of output image." << std::endl;

        return 0;
    }

    // Load source image.
    int components, width, height;
    unsigned char* data = stbi_load(input_filename, &width, &height, &components, 0);

    if (data == NULL) {
        std::cerr << "Couldn't load image " << input_filename << "." << std::endl;
        return 1;
    }

    if (components != 3) {
        std::cerr << "Image has to be RGB (3 channels)." << std::endl;
        return 1;
    }

    // Perform various filters.
    perturb(data, width, height);

    // Write image to PNG file.
    width /= 2;
    height /= 2;
    stbi_write_png(output_filename, width, height, components, data, width * components);

    // Clean up image data.
    stbi_image_free(data);

    return 0;
}
