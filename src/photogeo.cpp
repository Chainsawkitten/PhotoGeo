#include <photogeo.h>

#include <iostream>
#include "quantization/quantization.hpp"
#include "tracing/marching_squares.hpp"
#include "vertex_reduction/visvalingam_whyatt.hpp"

void ptg_generate_collision_geometry(const ptg_generation_parameters* parameters, ptg_outline*** out_outlines, unsigned int** out_outline_counts) {
    // Quantization.
    ptg_quantization_results quantization_results;
    ptg_quantize(parameters->image_parameters, parameters->quantization_parameters, &quantization_results);

    // Tracing.
    ptg_tracing_results tracing_results;
    ptg_trace(parameters->image_parameters, &quantization_results, parameters->tracing_parameters, &tracing_results);

    // Quantization results are no longer needed.
    ptg_free_quantization_results(&quantization_results);

    // Vertex reduction.
    ptg_reduce(&tracing_results, parameters->vertex_reduction_parameters);

    *out_outlines = tracing_results.outlines;
    *out_outline_counts = tracing_results.outline_counts;
}

void ptg_free_results(unsigned int layer_count, ptg_outline** outlines, unsigned int* outline_counts) {
    for (unsigned int layer_index = 0; layer_index < layer_count; ++layer_index) {
        for (unsigned int outline_index = 0; outline_index < outline_counts[layer_index]; ++outline_index)
            delete[] outlines[layer_index][outline_index].vertices;
        delete[] outlines[layer_index];
    }
    delete[] outlines;
    delete[] outline_counts;
}

void ptg_quantize(const ptg_image_parameters* image_parameters, const ptg_quantization_parameters* quantization_parameters, ptg_quantization_results* quantization_results) {
    // Allocate color layers.
    quantization_results->layers = new bool*[image_parameters->color_layer_count];
    for (unsigned int layer = 0; layer < image_parameters->color_layer_count; ++layer)
        quantization_results->layers[layer] = new bool[image_parameters->width * image_parameters->height];

    // Quantize image into layers.
    switch (quantization_parameters->quantization_method) {
        case PTG_EUCLIDEAN_SRGB:
            quantize(image_parameters, quantization_results->layers, color_distance_euclidean_srgb_sqr);
            break;
        case PTG_EUCLIDEAN_LINEAR:
            quantize(image_parameters, quantization_results->layers, color_distance_euclidean_linear_sqr);
            break;
        case PTG_CIE76:
            quantize(image_parameters, quantization_results->layers, color_distance_cie76_sqr);
            break;
        case PTG_CIE94:
            quantize(image_parameters, quantization_results->layers, color_distance_cie94_sqr);
            break;
        case PTG_CIEDE2000:
            quantize(image_parameters, quantization_results->layers, color_distance_ciede2000_sqr);
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
        ptgi_trace_marching_squares(quantization_results->layers[layer_index], image_parameters->width, image_parameters->height, out_tracing_results->outlines[layer_index], out_tracing_results->outline_counts[layer_index]);
}

void ptg_free_tracing_results(ptg_tracing_results* tracing_results) {
    ptg_free_results(tracing_results->layer_count, tracing_results->outlines, tracing_results->outline_counts);
}

void ptg_reduce(ptg_tracing_results* tracing_results, const ptg_vertex_reduction_parameters* vertex_reduction_parameters) {
    switch (vertex_reduction_parameters->vertex_reduction_method) {
        case PTG_VISVALINGAM_WHYATT:
            ptgi_visvalingam_whyatt(tracing_results);
            break;
    }
}
