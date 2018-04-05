#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* filename[2] = { "", "" };
    const char* log_filename = "";

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == '1' && argc > argument + 1)
                filename[0] = argv[++argument];

            // Output filename.
            else if (argv[argument][1] == '2' && argc > argument + 1)
                filename[1] = argv[++argument];

            // Log filename.
            else if (argv[argument][1] == 'l' && argc > argument + 1)
                log_filename = argv[++argument];
        }
    }

    // Display help if no valid configuration was given.
    if (filename[0][0] == '\0' || filename[1][0] == '\0') {
        std::cout << "usage: compare -1 first.png -2 second.png" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -1  Specify filename of first image." << std::endl;
        std::cout << "  -2  Specify filename of second image." << std::endl;
        std::cout << "  -l  Specify filename of log file." << std::endl;

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

    // Perform comparison.
    unsigned int total_pixels = width[0] * height[0];
    unsigned int difference = 0;
    for (unsigned int i = 0; i < total_pixels; ++i) {
        if (data[0][i * 3] != data[1][i * 3] ||
            data[0][i * 3 + 1] != data[1][i * 3 + 1] ||
            data[0][i * 3 + 2] != data[1][i * 3 + 2])
            ++difference;
    }

    // Free images.
    stbi_image_free(data[0]);
    stbi_image_free(data[1]);

    // Output difference.
    difference = 100.0 - static_cast<double>(difference) / total_pixels * 100.0;
    std::cout << difference << "%" << std::endl;
    if (log_filename != "") {
        std::ofstream log(log_filename);
        if (log.is_open()) {
            log << "[" << filename[0] << " - " << filename[1] << "] : " << difference << "%" << std::endl;
            log.close();
        }
        else
            std::cout << "Unable to open log file: " << log_filename << std::endl;
    }

    return 0;
}
