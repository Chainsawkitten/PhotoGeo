#include <photogeo.h>

#include <iostream>

void ptg_generate_collision_geometry(const ptg_source_parameters* parameters, unsigned int** out_vertex_count, ptg_vec2*** out_vertices) {
    std::cout << "ptg_generate_collision_geometry has not yet been implemented." << std::endl;

    *out_vertex_count = nullptr;
    *out_vertices = nullptr;
}

void ptg_free_results(unsigned int* vertex_count, ptg_vec2** vertices) {
    std::cout << "ptg_free_results has not yet been implemented." << std::endl;
}
