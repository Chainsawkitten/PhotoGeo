#include <photogeo.h>
#include <iostream>
#include <cstring>
#include <vector>
#include "conversion.hpp"
#include "svg.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* inputFilename = "";
    const char* outputFilename = "";
    std::vector<ptg_color> backgroundColors;
    std::vector<ptg_color> foregroundColors;
    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                inputFilename = argv[++argument];

            // Output filename.
            if (argv[argument][1] == 'o' && argc > argument + 1)
                outputFilename = argv[++argument];

            // Background color.
            if (argv[argument][1] == 'b' && argc > argument + 1)
                backgroundColors.push_back(TextToColor(argv[++argument]));

            // Foreground color.
            if (argv[argument][1] == 'f' && argc > argument + 1)
                foregroundColors.push_back(TextToColor(argv[++argument]));
        }
    }

    // Display help if no valid configuration was given.
    if (inputFilename[0] == '\0' || outputFilename[0] == '\0') {
        std::cout << "usage: PhotoGeoCmd -i inputFilename -o outputFilename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source image." << std::endl;
        std::cout << "  -o  Specify filename of result SVG." << std::endl;
        std::cout << "  -b  Specify background color." << std::endl
                  << "      Format: R:G:B" <<  std::endl;
        std::cout << "  -f  Specify foreground color." << std::endl
                  << "      Format: R:G:B" <<  std::endl;

        return 0;
    }

    // We need at least one color.
    if (backgroundColors.size() + foregroundColors.size() == 0) {
        std::cout << "You must specify at least one background or foreground color." << std::endl;
        return 0;
    }

    // Load source image.
    int components, width, height;
    unsigned char* data = stbi_load(inputFilename, &width, &height, &components, 0);

    if (data == NULL) {
        std::cerr << "Couldn't load image " << inputFilename << "." << std::endl;
        return 1;
    }

    if (components != 3) {
        std::cerr << "Image has to be RGB (3 channels)." << std::endl;
        return 1;
    }

    // Source image parameters.
    ptg_image_parameters imageParameters;
    memset(&imageParameters, 0, sizeof(ptg_image_parameters));
    imageParameters.image = reinterpret_cast<ptg_color*>(data);
    imageParameters.background_color_count = backgroundColors.size();
    imageParameters.background_colors = backgroundColors.data();
    imageParameters.color_layer_count = foregroundColors.size();
    imageParameters.color_layer_colors = foregroundColors.data();

    // Quantization parameters.
    ptg_quantization_parameters quantizationParameters;

    // Generation parameters.
    ptg_generation_parameters generationParameters;
    generationParameters.image_parameters = &imageParameters;
    generationParameters.quantization_parameters = &quantizationParameters;

    // Results.
    unsigned int* vertexCount;
    ptg_vec2** vertices;

    // Generate collision geometry.
    ptg_generate_collision_geometry(&generationParameters, &vertexCount, &vertices);

    // Free image.
    stbi_image_free(data);

    // Free results.
    ptg_free_results(vertexCount, vertices);


    // Test SVG output.
    vertexCount = new unsigned int[foregroundColors.size()];
    vertices = new ptg_vec2*[foregroundColors.size()];

    // Test data.
    for (unsigned int i=0; i < foregroundColors.size(); ++i) {
        vertexCount[i] = 2;
        vertices[i] = new ptg_vec2[vertexCount[i]];

        vertices[i][0].x = 0;
        vertices[i][0].y = i * 50;

        vertices[i][1].x = 50;
        vertices[i][1].y = i * 50;
    }

    // Output SVG file.
    WriteSVG(outputFilename, foregroundColors.size(), foregroundColors.data(), vertexCount, vertices, true);

    // Clean up test data.
    delete[] vertexCount;
    for (unsigned int i=0; i < foregroundColors.size(); ++i)
        delete[] vertices[i];
    delete[] vertices;

    return 0;
}
