#include <photogeo.h>

#include <iostream>
#include "quantization/euclidean.hpp"

void ptg_generate_collision_geometry(const ptg_generation_parameters* parameters, unsigned int** out_vertex_count, ptg_vec2*** out_vertices) {
    std::cerr << "ptg_generate_collision_geometry has not yet been implemented." << std::endl;

    // Quantization.
    bool** layers = ptg_quantize(parameters->image_parameters, parameters->quantization_parameters);
    ptg_free_quantization_results(layers, parameters->image_parameters->color_layer_count);

    *out_vertex_count = nullptr;
    *out_vertices = nullptr;
}

void ptg_free_results(unsigned int* vertex_count, ptg_vec2** vertices) {
    std::cerr << "ptg_free_results has not yet been implemented." << std::endl;
}

bool** ptg_quantize(const ptg_image_parameters* image_parameters, const ptg_quantization_parameters* quantization_parameters) {
    std::cerr << "ptg_quantize has not yet been implemented." << std::endl;

    // Allocate color layers.
    bool** layers = new bool*[image_parameters->color_layer_count];
    for (unsigned int layer = 0; layer < image_parameters->color_layer_count; ++layer)
        layers[layer] = new bool[image_parameters->width * image_parameters->height];

    // Quantize image into layers.
    quantize_euclidean(image_parameters, layers);

    return layers;
}

void ptg_free_quantization_results(bool** layers, unsigned int layer_count) {
    for (unsigned int layer = 0; layer < layer_count; ++layer)
        delete[] layers[layer];
    delete[] layers;
}
