#include <photogeo.h>

#include <iostream>
#include "quantization/quantization.hpp"
#include "tracing/marching_squares.hpp"

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
    // Allocate color layers.
    quantization_results->layers = new bool*[image_parameters->color_layer_count];
    for (unsigned int layer = 0; layer < image_parameters->color_layer_count; ++layer)
        quantization_results->layers[layer] = new bool[image_parameters->width * image_parameters->height];

    // Quantize image into layers.
    switch (quantization_parameters->quantization_method) {
        case PTG_EUCLIDEAN:
            quantize(image_parameters, quantization_results->layers, color_distance_euclidean_sqr);
            break;
        case PTG_CIE76:
            quantize(image_parameters, quantization_results->layers, color_distance_cie76_sqr);
            std::cerr << "CIE76 quantization has not yet been implemented." << std::endl;
            break;
    }
    quantization_results->layer_count = image_parameters->color_layer_count;
}

void ptg_free_quantization_results(ptg_quantization_results* quantization_results) {
    for (unsigned int layer = 0; layer < quantization_results->layer_count; ++layer)
        delete[] quantization_results->layers[layer];
    delete[] quantization_results->layers;
}

void ptg_trace(const ptg_image_parameters* image_parameters, const ptg_quantization_results* quantization_results, const ptg_tracing_parameters* tracing_parameters, ptg_tracing_results* out_tracing_results) {
    // Allocate outlines.
    out_tracing_results->layer_count = quantization_results->layer_count;
    out_tracing_results->outline_counts = new unsigned int[out_tracing_results->layer_count];
    out_tracing_results->outlines = new ptg_outline*[out_tracing_results->layer_count];

    // Trace image using marching squares.
    for (unsigned int layer_index = 0; layer_index < out_tracing_results->layer_count; ++layer_index)
        trace_marching_squares(quantization_results->layers[layer_index], image_parameters->width, image_parameters->height, out_tracing_results->outlines[layer_index], out_tracing_results->outline_counts[layer_index]);
}

void ptg_free_tracing_results(ptg_tracing_results* tracing_results) {
    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline)
            delete[] tracing_results->outlines[outline]->vertices;
        delete[] tracing_results->outlines[layer];
    }
    delete[] tracing_results->outlines;
    delete[] tracing_results->outline_counts;
}
