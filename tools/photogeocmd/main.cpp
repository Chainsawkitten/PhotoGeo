#include <photogeo.h>
#include <iostream>
#include <cmath>
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
    bool profile_time = false;
    bool profile_memory = false;

    // Methods.
    std::vector<ptg_image_processing_method> image_processing_methods;
    ptg_quantization_method quantization_method = PTG_EUCLIDEAN_LINEAR;
    ptg_tracing_method tracing_method = PTG_MARCHING_SQUARES;
    ptg_vertex_reduction_method vertex_reduction_method = PTG_NO_VERTEX_REDUCTION;

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                input_filename = argv[++argument];

            // Output filename.
            else if (argv[argument][1] == 'o' && argc > argument + 1)
                output_filename = argv[++argument];

            // Background color.
            else if (argv[argument][1] == 'b' && argc > argument + 1)
                background_colors.push_back(text_to_color(argv[++argument]));

            // Foreground color.
            else if (argv[argument][1] == 'f' && argc > argument + 1)
                foreground_colors.push_back(text_to_color(argv[++argument]));

            // Output image processing.
            else if (argv[argument][1] == 't' && argv[argument][2] == 'p')
                output_image_processing = true;

            // Output quantization.
            else if (argv[argument][1] == 't' && argv[argument][2] == 'q')
                output_quantization = true;

            // Output tracing.
            else if (argv[argument][1] == 't' && argv[argument][2] == 't')
                output_tracing = true;

            // Output vertex reduction.
            else if (argv[argument][1] == 't' && argv[argument][2] == 'v')
                output_vertex_reduction = true;

            // Profile time.
            else if (argv[argument][1] == 'p' && argv[argument][2] == 't')
                profile_time = true;

            // Profile memory.
            else if (argv[argument][1] == 'p' && argv[argument][2] == 'm')
                profile_memory = true;

            // Log filename.
            else if (argv[argument][1] == 'l' && argv[argument][2] == 'o' && argc > argument + 1)
                log_filename = argv[++argument];

            // Iteration count.
            else if (argv[argument][1] == 'l' && argv[argument][2] == 'i' && argc > argument + 1)
                iteration_count = std::stoi(argv[++argument]);

            // Image processing methods.
            // Gaussian blur.
            else if (argv[argument][1] == 'p' && (argv[argument][2] - '0') == PTG_GAUSSIAN_BLUR)
                image_processing_methods.push_back(PTG_GAUSSIAN_BLUR);

            // Bilateral filter.
            else if (argv[argument][1] == 'p' && (argv[argument][2] - '0') == PTG_BILATERAL_FILTER)
                image_processing_methods.push_back(PTG_BILATERAL_FILTER);

            // Median filter.
            else if (argv[argument][1] == 'p' && (argv[argument][2] - '0') == PTG_MEDIAN_FILTER)
                image_processing_methods.push_back(PTG_MEDIAN_FILTER);

            // Kuwahara filter.
            else if (argv[argument][1] == 'p' && (argv[argument][2] - '0') == PTG_KUWAHARA_FILTER)
                image_processing_methods.push_back(PTG_KUWAHARA_FILTER);

            // Quantization method.
            // Euclidean distance in sRGB space.
            else if (argv[argument][1] == 'q' && (argv[argument][2] - '0') == PTG_EUCLIDEAN_SRGB)
                quantization_method = PTG_EUCLIDEAN_SRGB;

            // Euclidean distance in linear RGB space.
            else if (argv[argument][1] == 'q' && (argv[argument][2] - '0') == PTG_EUCLIDEAN_LINEAR)
                quantization_method = PTG_EUCLIDEAN_LINEAR;

            // CIE76.
            else if (argv[argument][1] == 'q' && (argv[argument][2] - '0') == PTG_CIE76)
                quantization_method = PTG_CIE76;

            // CIE94.
            else if (argv[argument][1] == 'q' && (argv[argument][2] - '0') == PTG_CIE94)
                quantization_method = PTG_CIE94;

            // CIEDE2000.
            else if (argv[argument][1] == 'q' && (argv[argument][2] - '0') == PTG_CIEDE2000)
                quantization_method = PTG_CIEDE2000;

            // Tracing method.
            // Marching squares.
            else if (argv[argument][1] == 't' && (argv[argument][2] - '0') == PTG_MARCHING_SQUARES)
                tracing_method = PTG_MARCHING_SQUARES;

            // Vertex reduction method.
            // Don't perform any vertex reduction.
            else if (argv[argument][1] == 'v' && (argv[argument][2] - '0') == PTG_NO_VERTEX_REDUCTION)
                vertex_reduction_method = PTG_NO_VERTEX_REDUCTION;

            // Douglas-Peucker.
            else if (argv[argument][1] == 'v' && (argv[argument][2] - '0') == PTG_DOUGLAS_PEUCKER)
                vertex_reduction_method = PTG_DOUGLAS_PEUCKER;

            // Visvalingam-Whyatt.
            else if (argv[argument][1] == 'v' && (argv[argument][2] - '0') == PTG_VISVALINGAM_WHYATT)
                vertex_reduction_method = PTG_VISVALINGAM_WHYATT;
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
        std::cout << "  -pt Profile time." << std::endl;
        std::cout << "  -pm Profile memory." << std::endl;
        std::cout << "  -lo Specify filename of log file." << std::endl;
        std::cout << "  -li Specify how many times to iterate test." << std::endl
                  << "      Integer values only." << std::endl;
        std::cout << "  -p0 Gaussian blur. Image processing method." << std::endl;
        std::cout << "  -p1 Bilateral filter. Image processing method." << std::endl;
        std::cout << "  -p2 Median filter. Image processing method." << std::endl;
        std::cout << "  -p3 Kuwahara filter. Image processing method." << std::endl;
        std::cout << "  -q0 Euclidean distance in sRGB space. Quantization method." << std::endl;
        std::cout << "  -q1 Euclidean distance in linear RGB space. Quantization method." << std::endl;
        std::cout << "  -q2 CIE76. Quantization method." << std::endl;
        std::cout << "  -q3 CIE94. Quantization method." << std::endl;
        std::cout << "  -q4 CIE2000. Quantization method." << std::endl;
        std::cout << "  -t0 Marching squares. Tracing method." << std::endl;
        std::cout << "  -v0 Don't perform any vertex reduction. Vertex reduction method." << std::endl;
        std::cout << "  -v1 Douglas-Peucker. Vertex reduction method." << std::endl;
        std::cout << "  -v2 Visvalingam-Whyatt. Vertex reduction method." << std::endl;

        return 0;
    }

    // We need at least one color.
    if (background_colors.size() + foreground_colors.size() == 0) {
        std::cout << "You must specify at least one background or foreground color." << std::endl;
        return 0;
    }

    // Allocate profiling results.
    profiling::result* profiling_results = new profiling::result[iteration_count * STAGE_COUNT];

    // Profiling.
    if (profile_time)
        std::cout << "Profiling time." << std::endl;
    if (profile_memory) {
        std::cout << "Profiling memory." << std::endl;
        // Start up profiling if memory is to be measured.
        profiling::start_up();
    }
    if (iteration_count > 1)
        std::cout << "Iteration count: " << iteration_count << std::endl;

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

        // Image processing parameters.
        ptg_image_processing_parameters image_processing_parameters;
        image_processing_parameters.method_count = image_processing_methods.size();
        image_processing_parameters.methods = image_processing_methods.data();

        // Quantization parameters.
        ptg_quantization_parameters quantization_parameters;
        quantization_parameters.quantization_method = quantization_method;

        // Tracing parameters.
        ptg_tracing_parameters tracing_parameters;
        tracing_parameters.tracing_method = tracing_method;

        // Vertex reduction parameters.
        ptg_vertex_reduction_parameters vertex_reduction_parameters;
        vertex_reduction_parameters.vertex_reduction_method = vertex_reduction_method;

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
            PROFILE(&profiling_results[iteration * STAGE_COUNT + IMAGE_PROCESSING], profile_time, profile_memory);

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
            PROFILE(&profiling_results[iteration * STAGE_COUNT + QUANTIZATION], profile_time, profile_memory);

            // Perform quantization.
            ptg_quantize(generation_parameters.image_parameters, generation_parameters.quantization_parameters, &quantization_results);

            // Output quantization results.
            if (output_quantization) {
                std::cout << "Writing to quantization.png." << std::endl;
                write_quantized_to_png("quantization.png", quantization_results.layers, width, height, foreground_colors.data(), foreground_colors.size());
            }
        }

        // Free image.
        stbi_image_free(data);

        // Tracing.
        ptg_tracing_results tracing_results;
        {
            // Profile tracing.
            PROFILE(&profiling_results[iteration * STAGE_COUNT + TRACING], profile_time, profile_memory);

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

        // Vertex reduction.
        {
            // Profile vertex reduction.
            PROFILE(&profiling_results[iteration * STAGE_COUNT + VERTEX_REDUCTION], profile_time, profile_memory);

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
    double mean_time[STAGE_COUNT];
    double mean_memory_init[STAGE_COUNT];
    double mean_memory_max[STAGE_COUNT];
    for (unsigned int stage_it = 0; stage_it < STAGE_COUNT; ++stage_it) {
        mean_time[stage_it] = 0.0;
        mean_memory_init[stage_it] = 0.0;
        mean_memory_max[stage_it] = 0.0;
        for (unsigned int test_it = 0; test_it < iteration_count; ++test_it) {
            mean_time[stage_it] += profiling_results[test_it * STAGE_COUNT + stage_it].time;
            mean_memory_init[stage_it] += profiling_results[test_it * STAGE_COUNT + stage_it].memory_init;
            mean_memory_max[stage_it] += profiling_results[test_it * STAGE_COUNT + stage_it].memory_max;
        }
        mean_time[stage_it] /= iteration_count;
        mean_memory_init[stage_it] /= iteration_count;
        mean_memory_max[stage_it] /= iteration_count;
    }

    // Calculate standard deviation of test results.
    double deviation_time[STAGE_COUNT];
    double deviation_memory_init[STAGE_COUNT];
    double deviation_memory_max[STAGE_COUNT];
    for (unsigned int stage_it = 0; stage_it < STAGE_COUNT; ++stage_it) {
        deviation_time[stage_it] = 0.0;
        deviation_memory_init[stage_it] = 0.0;
        deviation_memory_max[stage_it] = 0.0;
        if (iteration_count > 1) {
            for (unsigned int test_it = 0; test_it < iteration_count; ++test_it) {
                double value = profiling_results[test_it * STAGE_COUNT + stage_it].time - mean_time[stage_it];
                deviation_time[stage_it] += value * value;

                value = profiling_results[test_it * STAGE_COUNT + stage_it].memory_init - mean_memory_init[stage_it];
                deviation_memory_init[stage_it] += value * value;

                value = profiling_results[test_it * STAGE_COUNT + stage_it].memory_max - mean_memory_max[stage_it];
                deviation_memory_max[stage_it] += value * value;
            }
            deviation_time[stage_it] = sqrt(deviation_time[stage_it] / (iteration_count - 1));
            deviation_memory_init[stage_it] = sqrt(deviation_memory_init[stage_it] / (iteration_count - 1));
            deviation_memory_max[stage_it] = sqrt(deviation_memory_max[stage_it] / (iteration_count - 1));
        }
    }

    // Output test results to log.
    if (log_filename[0] != '\0') {
        std::ofstream log(log_filename);
        if (log.is_open()) {
            log << "Iteration count: " << iteration_count << std::endl;
            for (unsigned int stage_it = 0; stage_it < STAGE_COUNT; ++stage_it) {
                log << "Stage ID: " << stage_it << std::endl;
                if (profile_time)
                    log << "Time(milliseconds):\n\t\t\tMean: " << mean_time[stage_it] << "\n\t\t\tStandard deviation: " << deviation_time[stage_it] << std::endl;
                if (profile_memory)
                    log << "Memory initial(megabyte):\n\t\t\tMean: " << mean_memory_init[stage_it] << "\n\t\t\tStandard deviation: " << deviation_memory_init[stage_it] << std::endl
                        << "Memory max(megabyte):\n\t\t\tMean: " << mean_memory_max[stage_it] << "\n\t\t\tStandard deviation: " << deviation_memory_max[stage_it] << std::endl << std::endl;
            }
            log.close();
        } else
            std::cout << "Unable to open log file: " << log_filename << std::endl;
    }

    // Free test results.
    delete[] profiling_results;

    // Shutdown profiling if memory was measured.
    if (profile_memory)
        profiling::shutdown();

    return 0;
}
