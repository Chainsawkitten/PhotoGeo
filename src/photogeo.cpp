#include <photogeo.h>

#include <iostream>
#include "quantization/euclidean.hpp"

void ptg_generate_collision_geometry(const ptg_generation_parameters* parameters, ptg_outline*** out_outlines, unsigned int** out_outline_counts) {
    std::cerr << "ptg_generate_collision_geometry has not yet been implemented." << std::endl;

    // Quantization.
    ptg_quantization_results quantization_results;
    ptg_quantize(parameters->image_parameters, parameters->quantization_parameters, &quantization_results);
    ptg_free_quantization_results(&quantization_results);

    *out_outlines = nullptr;
    *out_outline_counts = nullptr;
}

void ptg_free_results(ptg_outline** outlines, unsigned int* outline_count) {
    std::cerr << "ptg_free_results has not yet been implemented." << std::endl;
}

void ptg_quantize(const ptg_image_parameters* image_parameters, const ptg_quantization_parameters* quantization_parameters, ptg_quantization_results* quantization_results) {
    std::cerr << "ptg_quantize has not yet been implemented." << std::endl;

    // Allocate color layers.
    quantization_results->layers = new bool*[image_parameters->color_layer_count];
    for (unsigned int layer = 0; layer < image_parameters->color_layer_count; ++layer)
        quantization_results->layers[layer] = new bool[image_parameters->width * image_parameters->height];

    // Quantize image into layers.
    quantize_euclidean(image_parameters, quantization_results->layers);
    quantization_results->layer_count = image_parameters->color_layer_count;
}

void ptg_free_quantization_results(ptg_quantization_results* quantization_results) {
    for (unsigned int layer = 0; layer < quantization_results->layer_count; ++layer)
        delete[] quantization_results->layers[layer];
    delete[] quantization_results->layers;
}

}
