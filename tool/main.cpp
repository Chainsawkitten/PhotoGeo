#include <photogeo.h>
#include <iostream>
#include <cstring>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* inputFilename = "";
    const char* outputFilename = "";
    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                inputFilename = argv[++argument];

            // Output filename.
            if (argv[argument][1] == 'o' && argc > argument + 1)
                outputFilename = argv[++argument];
        }
    }

    std::cout << "Input: " << inputFilename << std::endl;
    std::cout << "Output: " << outputFilename << std::endl;

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
