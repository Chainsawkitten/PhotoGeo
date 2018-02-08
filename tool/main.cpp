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

    // Display help if no valid configuration was given.
    if (inputFilename[0] == '\0' || outputFilename[0] == '\0') {
        std::cout << "usage: PhotoGeoCmd -i inputFilename -o outputFilename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source image." << std::endl;
        std::cout << "  -o  Specify filename of result SVG." << std::endl;

        return 0;
    }

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
