#include <photogeo.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include "conversion.hpp"
#include "png.hpp"
#include "profiling.hpp"
#include "svg.hpp"

#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Stages during profiling-
enum profiling_stage {
    IMAGE_PROCESSING,
    QUANTIZATION,
    TRACING,
    VERTEX_REDUCTION,
    STAGE_COUNT
};

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* input_filename = "";
    const char* output_filename = "";
    std::vector<ptg_color> background_colors;
    std::vector<ptg_color> foreground_colors;
    const char* log_filename = "";
    unsigned int iteration_count = 1;
    bool output_image_processing = false;
    bool output_quantization = false;
    bool output_tracing = false;
    bool output_vertex_reduction = false;

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

            // Log filename.
            if (argv[argument][1] == 'l' && argv[argument][2] == 'o' && argc > argument + 1)
                log_filename = argv[++argument];

            // Iteration count.
            if (argv[argument][1] == 'l' && argv[argument][2] == 'i' && argc > argument + 1)
                iteration_count = std::stoi(argv[++argument]);

            // Output image processing.
            if (argv[argument][1] == 't' && argv[argument][2] == 'p')
                output_image_processing = true;

            // Output quantization.
            if (argv[argument][1] == 't' && argv[argument][2] == 'q')
                output_quantization = true;

            // Output tracing.
            if (argv[argument][1] == 't' && argv[argument][2] == 't')
                output_tracing = true;

            // Output vertex reduction.
            if (argv[argument][1] == 't' && argv[argument][2] == 'v')
                output_vertex_reduction = true;
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
        std::cout << "  -lo Specify filename of log file." << std::endl;
        std::cout << "  -li Specify how many times to iterate test." << std::endl
                  << "      Integer values only." << std::endl;
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

    // Allocate profiling results.
    profiling::result* profiling_results = new profiling::result[iteration_count * STAGE_COUNT];

    // Generate collision geometry.
    for (unsigned int iteration = 0; iteration < iteration_count; ++iteration) {

        if (iteration_count > 1)
            std::cout << "Iteration: " << iteration + 1 << std::endl;

        // Load source image.
        int width, height, components;
        unsigned char* data = stbi_load(input_filename, &width, &height, &components, 0);

        if (data == NULL) {
            std::cerr << "Couldn't load image " << input_filename << "." << std::endl;
            return 1;
        }

        if (components != 3) {
            std::cerr << "Image has to be RGB (3 channels)." << std::endl;
            return 1;
        }

        // Image parameters.
        ptg_image_parameters image_parameters;
        memset(&image_parameters, 0, sizeof(ptg_image_parameters));
        image_parameters.image = reinterpret_cast<ptg_color*>(data);
        image_parameters.width = width;
        image_parameters.height = height;
        image_parameters.background_color_count = background_colors.size();
        image_parameters.background_colors = background_colors.data();
        image_parameters.color_layer_count = foreground_colors.size();
        image_parameters.color_layer_colors = foreground_colors.data();

        // Image processing methods.
        std::vector<ptg_image_processing_method> methods;
        methods.push_back(PTG_MEDIAN_FILTER);
        methods.push_back(PTG_BILATERAL_FILTER);

        // Image processing parameters.
        ptg_image_processing_parameters image_processing_parameters;
        image_processing_parameters.method_count = methods.size();
        image_processing_parameters.methods = methods.data();

        // Quantization parameters.
        ptg_quantization_parameters quantization_parameters;
        quantization_parameters.quantization_method = PTG_EUCLIDEAN_LINEAR;

        // Tracing parameters.
        ptg_tracing_parameters tracing_parameters;
        tracing_parameters.tracing_method = PTG_MARCHING_SQUARES;

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
        
        // Image processing.
        {
            // Profile image processing.
            PROFILE(&profiling_results[iteration * STAGE_COUNT + IMAGE_PROCESSING]);

            // Perform image processing.
            ptg_image_process(generation_parameters.image_parameters, generation_parameters.image_processing_parameters);

            // Output image processing results.
            if (output_image_processing) {
                std::cout << "Writing to image_processing.png." << std::endl;
                if (!stbi_write_png("image_processing.png", width, height, components, image_parameters.image, width * components))
                    std::cout << "Couldn't write image." << std::endl;
            }
        }

        // Quantization.
        ptg_quantization_results quantization_results;
        {
            // Profile quantization.
            PROFILE(&profiling_results[iteration * STAGE_COUNT + QUANTIZATION]);

            // Perform quantization.
            ptg_quantize(generation_parameters.image_parameters, generation_parameters.quantization_parameters, &quantization_results);

            // Output quantization results.
            if (output_quantization) {
                std::cout << "Writing to quantization.png." << std::endl;
                write_quantized_to_png("quantization.png", quantization_results.layers, width, height, foreground_colors.data(), foreground_colors.size());
            }
        }

        // Tracing.
        ptg_tracing_results tracing_results;
        {
            // Profile tracing.
            PROFILE(&profiling_results[iteration * STAGE_COUNT + TRACING]);
            
            // Perform tracing.
            ptg_trace(generation_parameters.image_parameters, &quantization_results, generation_parameters.tracing_parameters, &tracing_results);

            // Output tracing results.
            if (output_tracing) {
                std::cout << "Writing to tracing.svg." << std::endl;
                write_svg("tracing.svg", &image_parameters, tracing_results.outlines, tracing_results.outline_counts, false);
            }
        }

        // Free quantization results.
        ptg_free_quantization_results(&quantization_results);
        
        // Free image.
        stbi_image_free(data);

        // Vertex reduction.
        {
            // Profile vertex reduction.
            PROFILE(&profiling_results[iteration * STAGE_COUNT + VERTEX_REDUCTION]);

            // Output before vertex reduction.
            if (output_vertex_reduction) {
                std::cout << "Writing to before_reduction.svg." << std::endl;
                write_svg("before_reduction.svg", &image_parameters, tracing_results.outlines, tracing_results.outline_counts, false);
            }

            // Perform vertex reduction.
            ptg_reduce(&tracing_results, generation_parameters.vertex_reduction_parameters);

            // Output after vertex reduction.            
            if (output_vertex_reduction) {
                std::cout << "Writing to after_reduction.svg." << std::endl;
                write_svg("after_reduction.svg", &image_parameters, tracing_results.outlines, tracing_results.outline_counts, false);
            }
        }

        // Fetch outlines.
        ptg_outline** outlines = tracing_results.outlines;
        unsigned int* outline_counts = tracing_results.outline_counts;

        // Output results to SVG file.
        write_svg(output_filename, &image_parameters, outlines, outline_counts, false);

        // Free outlines.
        ptg_free_results(foreground_colors.size(), outlines, outline_counts);
    }

    // Calculate mean of test results.
    double mean[STAGE_COUNT];
    for (unsigned int stage_it = 0; stage_it < STAGE_COUNT; ++stage_it) {
        mean[stage_it] = 0.0;
        for (unsigned int test_it = 0; test_it < iteration_count; ++test_it) {
            mean[stage_it] += profiling_results[test_it * STAGE_COUNT + stage_it].time;
        }
        mean[stage_it] /= iteration_count;
    }

    // Calculate standard deviation of test results.
    double deviation[STAGE_COUNT];
    for (unsigned int stage_it = 0; stage_it < STAGE_COUNT; ++stage_it) {
        deviation[stage_it] = 0.0;
        for (unsigned int test_it = 0; test_it < iteration_count; ++test_it) {
            double value = profiling_results[test_it * STAGE_COUNT + stage_it].time - mean[stage_it];
            deviation[stage_it] += value * value;
        }
        deviation[stage_it] /= iteration_count;
    }

    // Output test results to log.
    if (log_filename != "") {
        std::ofstream log(log_filename);
        if (log.is_open()) {
            log << "Iteration count: " << iteration_count << std::endl;
            for (unsigned int stage_it = 0; stage_it < STAGE_COUNT; ++stage_it) {
                log << "Stage ID: " << stage_it << std::endl
                    << "Mean(ms): " << mean[stage_it] << "; Standard deviation: " << deviation[stage_it] << ";" << std::endl;
            }
            log.close();
        } else
            std::cout << "Unable to open log file: " << log_filename << std::endl;
    }

    // Free test results.
    delete[] profiling_results;

    return 0;
}
