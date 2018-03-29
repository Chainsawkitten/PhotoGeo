#include <photogeo.h>
#include <iostream>
#include <cstring>
#include <vector>
#include "conversion.hpp"
#include "png.hpp"
#include "svg.hpp"

#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* input_filename = "";
    const char* output_filename = "";
    std::vector<ptg_color> background_colors;
    std::vector<ptg_color> foreground_colors;
    bool test_image_processing = false;
    bool test_quantization = false;
    bool test_tracing = false;
    bool test_vertex_reduction = false;

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                input_filename = argv[++argument];

            // Output filename.
            if (argv[argument][1] == 'o' && argc > argument + 1)
                output_filename = argv[++argument];

            // Background color.
            if (argv[argument][1] == 'b' && argc > argument + 1)
                background_colors.push_back(text_to_color(argv[++argument]));

            // Foreground color.
            if (argv[argument][1] == 'f' && argc > argument + 1)
                foreground_colors.push_back(text_to_color(argv[++argument]));

            // Test image processing.
            if (argv[argument][1] == 't' && argv[argument][2] == 'p')
                test_image_processing = true;

            // Test quantization.
            if (argv[argument][1] == 't' && argv[argument][2] == 'q')
                test_quantization = true;

            // Test tracing.
            if (argv[argument][1] == 't' && argv[argument][2] == 't')
                test_tracing = true;

            // Test vertex reduction.
            if (argv[argument][1] == 't' && argv[argument][2] == 'v')
                test_vertex_reduction = true;
        }
    }

    // Display help if no valid configuration was given.
    if (input_filename[0] == '\0' || output_filename[0] == '\0') {
        std::cout << "usage: PhotoGeoCmd -i input_filename -o output_filename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source image." << std::endl;
        std::cout << "  -o  Specify filename of result SVG." << std::endl;
        std::cout << "  -b  Specify background color." << std::endl
                  << "      Format: R:G:B" <<  std::endl;
        std::cout << "  -f  Specify foreground color." << std::endl
                  << "      Format: R:G:B" << std::endl;
        std::cout << "  -tp Test image processing." << std::endl
                  << "      Results are outputted to PNG." << std::endl;
        std::cout << "  -tq Test quantization." << std::endl
                  << "      Results are outputted to PNG." << std::endl;
        std::cout << "  -tt Test tracing." << std::endl
                  << "      Results are outputted to SVG." << std::endl;
        std::cout << "  -tv Test vertex reduction." << std::endl
                  << "      Results are outputted to SVG." << std::endl;

        return 0;
    }

    // We need at least one color.
    if (background_colors.size() + foreground_colors.size() == 0) {
        std::cout << "You must specify at least one background or foreground color." << std::endl;
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

    // Source image parameters.
    ptg_image_parameters image_parameters;
    memset(&image_parameters, 0, sizeof(ptg_image_parameters));
    image_parameters.image = reinterpret_cast<ptg_color*>(data);
    image_parameters.width = width;
    image_parameters.height = height;
    image_parameters.background_color_count = background_colors.size();
    image_parameters.background_colors = background_colors.data();
    image_parameters.color_layer_count = foreground_colors.size();
    image_parameters.color_layer_colors = foreground_colors.data();

    // Image processing parameters.
    std::vector<ptg_image_processing_method> methods;
    methods.push_back(PTG_BILATERAL_FILTER);

    ptg_image_processing_parameters image_processing_parameters;
    image_processing_parameters.method_count = methods.size();
    image_processing_parameters.methods = methods.data();

    // Quantization parameters.
    ptg_quantization_parameters quantization_parameters;
    quantization_parameters.quantization_method = PTG_EUCLIDEAN_SRGB;

    // Tracing parameters.
    ptg_tracing_parameters tracing_parameters;

    // Vertex reduction parameters.
    ptg_vertex_reduction_parameters vertex_reduction_parameters;
    vertex_reduction_parameters.vertex_reduction_method = PTG_VISVALINGAM_WHYATT;

    // Generation parameters.
    ptg_generation_parameters generation_parameters;
    generation_parameters.image_parameters = &image_parameters;
    generation_parameters.image_processing_parameters = &image_processing_parameters;
    generation_parameters.quantization_parameters = &quantization_parameters;
    generation_parameters.tracing_parameters = &tracing_parameters;
    generation_parameters.vertex_reduction_parameters = &vertex_reduction_parameters;

    // Results.
    ptg_outline** outlines;
    unsigned int* outline_counts;

    // Generate collision geometry.
    ptg_generate_collision_geometry(&generation_parameters, &outlines, &outline_counts);
    
    // Test image processing.
    if (test_image_processing) {
        std::cout << "Testing image processing." << std::endl;
        ptg_image_process(&image_parameters, &image_processing_parameters);
        std::cout << "Writing to image_processing.png" << std::endl;
        if (!stbi_write_png("image_processing.png", width, height, components, image_parameters.image, width * components))
            std::cout << "Couldn't write image." << std::endl;
    }

    // Test quantization.
    if (test_quantization) {
        std::cout << "Testing quantization." << std::endl;
        ptg_quantization_results quantization_results;
        quantization_parameters.quantization_method = PTG_EUCLIDEAN_SRGB;
        ptg_quantize(&image_parameters, &quantization_parameters, &quantization_results);
        write_quantized_to_png("quantization_euclidean_srgb.png", quantization_results.layers, width, height, foreground_colors.data(), foreground_colors.size());
        ptg_free_quantization_results(&quantization_results);

        quantization_parameters.quantization_method = PTG_EUCLIDEAN_LINEAR;
        ptg_quantize(&image_parameters, &quantization_parameters, &quantization_results);
        write_quantized_to_png("quantization_euclidean_linear.png", quantization_results.layers, width, height, foreground_colors.data(), foreground_colors.size());
        ptg_free_quantization_results(&quantization_results);

        quantization_parameters.quantization_method = PTG_CIE76;
        ptg_quantize(&image_parameters, &quantization_parameters, &quantization_results);
        write_quantized_to_png("quantization_CIE76.png", quantization_results.layers, width, height, foreground_colors.data(), foreground_colors.size());
        ptg_free_quantization_results(&quantization_results);

        quantization_parameters.quantization_method = PTG_CIE94;
        ptg_quantize(&image_parameters, &quantization_parameters, &quantization_results);
        write_quantized_to_png("quantization_CIE94.png", quantization_results.layers, width, height, foreground_colors.data(), foreground_colors.size());
        ptg_free_quantization_results(&quantization_results);

        quantization_parameters.quantization_method = PTG_CIEDE2000;
        ptg_quantize(&image_parameters, &quantization_parameters, &quantization_results);
        write_quantized_to_png("quantization_CIEDE2000.png", quantization_results.layers, width, height, foreground_colors.data(), foreground_colors.size());
        ptg_free_quantization_results(&quantization_results);
    }

    // Test tracing.
    if (test_tracing) {
        std::cout << "Testing tracing." << std::endl;
        ptg_quantization_results quantization_results;
        quantization_parameters.quantization_method = PTG_EUCLIDEAN_LINEAR;
        ptg_quantize(&image_parameters, &quantization_parameters, &quantization_results);
        ptg_tracing_results tracing_results;
        ptg_trace(&image_parameters, &quantization_results, &tracing_parameters, &tracing_results);
        std::cout << "Writing to tracing.svg" << std::endl;
        write_svg("tracing.svg", &image_parameters, tracing_results.outlines, tracing_results.outline_counts, false);
        ptg_free_tracing_results(&tracing_results);
        ptg_free_quantization_results(&quantization_results);
    }

    // Test vertex reduction.
    if (test_vertex_reduction) {
        std::cout << "Testing vertex reduction." << std::endl;

        ptg_quantization_results quantization_results;
        quantization_parameters.quantization_method = PTG_EUCLIDEAN_LINEAR;
        ptg_quantize(&image_parameters, &quantization_parameters, &quantization_results);

        ptg_tracing_results tracing_results;
        ptg_trace(&image_parameters, &quantization_results, &tracing_parameters, &tracing_results);
        write_svg("before_reduction.svg", &image_parameters, tracing_results.outlines, tracing_results.outline_counts, false);

        ptg_reduce(&tracing_results, &vertex_reduction_parameters);
        write_svg("after_reduction.svg", &image_parameters, tracing_results.outlines, tracing_results.outline_counts, false);

        ptg_free_tracing_results(&tracing_results);
        ptg_free_quantization_results(&quantization_results);
    }

    // Output results to SVG file.
    write_svg(output_filename, &image_parameters, outlines, outline_counts, false);

    // Free image.
    stbi_image_free(data);

    // Free results.
    ptg_free_results(foreground_colors.size(), outlines, outline_counts);

    return 0;
}
