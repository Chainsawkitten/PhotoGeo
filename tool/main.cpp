#include <photogeo.h>
#include <iostream>
#include <cstring>
#include <vector>
#include "conversion.hpp"
#include "png.hpp"
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
    imageParameters.width = width;
    imageParameters.height = height;
    imageParameters.background_color_count = backgroundColors.size();
    imageParameters.background_colors = backgroundColors.data();
    imageParameters.color_layer_count = foregroundColors.size();
    imageParameters.color_layer_colors = foregroundColors.data();

    // Quantization parameters.
    ptg_quantization_parameters quantizationParameters;
    quantizationParameters.quantization_method = PTG_EUCLIDEAN_SRGB;

    // Tracing parameters.
    ptg_tracing_parameters tracingParameters;

    // Generation parameters.
    ptg_generation_parameters generationParameters;
    generationParameters.image_parameters = &imageParameters;
    generationParameters.quantization_parameters = &quantizationParameters;
    generationParameters.tracing_parameters = &tracingParameters;

    // Results.
    ptg_outline** outlines;
    unsigned int* outline_counts;

    // Generate collision geometry.
    ptg_generate_collision_geometry(&generationParameters, &outlines, &outline_counts);

    {
        // Test quantization.
        std::cout << "Testing quantization." << std::endl;
        ptg_quantization_results quantization_results;
        ptg_quantize(&imageParameters, &quantizationParameters, &quantization_results);
        WriteQuantizedToPNG("quantizationEuclideanSRGB.png", quantization_results.layers, width, height, foregroundColors.data(), foregroundColors.size());
        ptg_free_quantization_results(&quantization_results);

        quantizationParameters.quantization_method = PTG_EUCLIDEAN_LINEAR;
        ptg_quantize(&imageParameters, &quantizationParameters, &quantization_results);
        WriteQuantizedToPNG("quantizationEuclideanLinear.png", quantization_results.layers, width, height, foregroundColors.data(), foregroundColors.size());
        ptg_free_quantization_results(&quantization_results);

        quantizationParameters.quantization_method = PTG_CIE76;
        ptg_quantize(&imageParameters, &quantizationParameters, &quantization_results);
        WriteQuantizedToPNG("quantizationCIE76.png", quantization_results.layers, width, height, foregroundColors.data(), foregroundColors.size());
        ptg_free_quantization_results(&quantization_results);
    }

    {
        // Test tracing.
        std::cout << "Testing tracing." << std::endl;
        ptg_quantization_results quantization_results;
        ptg_quantize(&imageParameters, &quantizationParameters, &quantization_results);
        ptg_tracing_results tracing_results;
        ptg_trace(&imageParameters, &quantization_results, &tracingParameters, &tracing_results);
        std::cout << "Writing to tracing.svg" << std::endl;
        WriteSVG("tracing.svg", foregroundColors.size(), foregroundColors.data(), tracing_results.outlines, tracing_results.outline_counts, false);
        ptg_free_tracing_results(&tracing_results);
        ptg_free_quantization_results(&quantization_results);
    }

    // Free image.
    stbi_image_free(data);

    // Free results.
    ptg_free_results(outlines, outline_counts);


    // Test SVG output.
    ptg_outline** outlines_test = new ptg_outline*[foregroundColors.size()];
    unsigned int* outline_counts_test = new unsigned int[foregroundColors.size()];

    // Test data.
    for (unsigned int i = 0; i < foregroundColors.size(); ++i) {
        outline_counts_test[i] = 1;
        outlines_test[i] = new ptg_outline[outline_counts_test[i]];
        outlines_test[i]->vertex_count = 2;
        outlines_test[i]->vertices = new ptg_vec2[outlines_test[i]->vertex_count];

        outlines_test[i]->vertices[0].x = 0;
        outlines_test[i]->vertices[0].y = i * 50;

        outlines_test[i]->vertices[1].x = 50;
        outlines_test[i]->vertices[1].y = i * 50;
    }

    // Output SVG file.
    WriteSVG(outputFilename, foregroundColors.size(), foregroundColors.data(), outlines_test, outline_counts_test, true);

    // Clean up test data.
    for (unsigned int layer = 0; layer < foregroundColors.size(); ++layer) {
        for (unsigned int outline = 0; outline < outline_counts_test[layer]; ++outline)
            delete[] outlines_test[layer][outline].vertices;
        delete[] outlines_test[layer];
    }
    delete[] outline_counts_test;
    delete[] outlines_test;

    return 0;
}
