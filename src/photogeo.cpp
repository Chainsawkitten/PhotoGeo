#include <photogeo.h>

#include <iostream>

void ptg_generate_collision_geometry(const ptg_generation_parameters* parameters, unsigned int** out_vertex_count, ptg_vec2*** out_vertices) {
    std::cerr << "ptg_generate_collision_geometry has not yet been implemented." << std::endl;

    // Quantization.
    ptg_color** layers = ptg_quantize(parameters->image_parameters, parameters->quantization_parameters);
    ptg_free_quantization_results(layers);

    *out_vertex_count = nullptr;
    *out_vertices = nullptr;
}

void ptg_free_results(unsigned int* vertex_count, ptg_vec2** vertices) {
    std::cerr << "ptg_free_results has not yet been implemented." << std::endl;
}

ptg_color** ptg_quantize(const ptg_image_parameters* image_parameters, const ptg_quantization_parameters* quantization_parameters) {
    std::cerr << "ptg_quantize has not yet been implemented." << std::endl;

    return nullptr;
}

void ptg_free_quantization_results(ptg_color** layers) {
    std::cerr << "ptg_free_quantization_results has not yet been implemented." << std::endl;
}
