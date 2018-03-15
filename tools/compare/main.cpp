#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* filename[2] = { "", "" };

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == '1' && argc > argument + 1)
                filename[0] = argv[++argument];

            // Output filename.
            if (argv[argument][1] == '2' && argc > argument + 1)
                filename[1] = argv[++argument];
        }
    }

    // Display help if no valid configuration was given.
    if (filename[0][0] == '\0' || filename[1][0] == '\0') {
        std::cout << "usage: compare -1 first.png -2 second.png" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -1  Specify filename of first image." << std::endl;
        std::cout << "  -2  Specify filename of second image." << std::endl;

        return 0;
    }

    // Load images.
    int components[2], width[2], height[2];
    unsigned char* data[2];

    for (int i = 0; i < 2; ++i) {
        data[i] = stbi_load(filename[i], &width[i], &height[i], &components[i], 0);

        if (data[i] == NULL) {
            std::cerr << "Couldn't load image " << filename[i] << "." << std::endl;
            return 1;
        }

        if (components[i] != 3) {
            std::cerr << "Image has to be RGB (3 channels)." << std::endl;
            return 1;
        }
    }

    if (width[0] != width[1] || height[0] != height[1]) {
        std::cerr << "Images have to have the same dimensions." << std::endl;
        return 1;
    }

    // TODO: Perform comparison.
    std::cout << "Compare tool not yet implemented." << std::endl;

    // Free images.
    stbi_image_free(data[0]);
    stbi_image_free(data[1]);

    return 0;
}
