#include <photogeo.h>
#include <iostream>
#include <cstring>
#include "svg.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* inputFilename = "";
    const char* outputFilename = "";
    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                inputFilename = argv[++argument];

            // Output filename.
            if (argv[argument][1] == 'o' && argc > argument + 1)
                outputFilename = argv[++argument];
        }
    }

    // Display help if no valid configuration was given.
    if (inputFilename[0] == '\0' || outputFilename[0] == '\0') {
        std::cout << "usage: PhotoGeoCmd -i inputFilename -o outputFilename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source image." << std::endl;
        std::cout << "  -o  Specify filename of result SVG." << std::endl;

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

    // Source image info.
    ptg_source_parameters parameters;
    memset(&parameters, 0, sizeof(ptg_source_parameters));

    // Results.
    unsigned int* vertexCount;
    ptg_vec2** vertices;

    // Generate collision geometry.
    ptg_generate_collision_geometry(&parameters, &vertexCount, &vertices);

    // Free image.
    stbi_image_free(data);

    // Free results.
    ptg_free_results(vertexCount, vertices);


    // Test SVG output.
    vertexCount = new unsigned int[2];
    vertices = new ptg_vec2*[2];

    // Test data.
    vertexCount[0] = 2;
    vertices[0] = new ptg_vec2[vertexCount[0]];

    vertices[0][0].x = 0;
    vertices[0][0].y = 0;

    vertices[0][1].x = 50;
    vertices[0][1].y = 50;

    vertexCount[1] = 2;
    vertices[1] = new ptg_vec2[vertexCount[1]];

    vertices[1][0].x = 89;
    vertices[1][0].y = 20;

    vertices[1][1].x = 40;
    vertices[1][1].y = 23;

    // Define the colors of the layers.
    ptg_color* colors = new ptg_color[2];
    colors[0].r = 0;
    colors[0].g = 0;
    colors[0].b = 0;

    colors[1].r = 255;
    colors[1].g = 0;
    colors[1].b = 0;

    // Output SVG file.
    WriteSVG(outputFilename, 2, colors, vertexCount, vertices, true);

    // Clean up test data.
    delete[] vertexCount;
    delete[] vertices[0];
    delete[] vertices[1];
    delete[] vertices;

    return 0;
}
