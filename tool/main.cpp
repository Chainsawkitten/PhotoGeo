#include <photogeo.h>
#include <iostream>
#include <cstring>

int main(int argc, const char* argv[]) {
    // Source image info.
    ptg_source_parameters parameters;
    memset(&parameters, 0, sizeof(ptg_source_parameters));

    // Results.
    unsigned int* vertexCount;
    ptg_vec2* vertices;

    // Generate collision geometry.
    ptg_generate_collision_geometry(&parameters, &vertexCount, &vertices);

    // Free results.
    ptg_free_results(vertexCount, vertices);

    return 0;
}
