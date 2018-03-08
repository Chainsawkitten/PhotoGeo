#include <iostream>
#include <read_svg.hpp>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* input_filename = "";
    const char* output_filename = "";

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == 'i' && argc > argument + 1)
                input_filename = argv[++argument];

            // Output filename.
            if (argv[argument][1] == 'o' && argc > argument + 1)
                output_filename = argv[++argument];
        }
    }

    // Display help if no valid configuration was given.
    if (input_filename[0] == '\0' || output_filename[0] == '\0') {
        std::cout << "usage: rasterize -i input_filename -o output_filename" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -i  Specify filename of source SVG." << std::endl;
        std::cout << "  -o  Specify filename of result PNG." << std::endl;

        return 0;
    }

    // Load SVG image.
    ptg_tracing_results svg = read_svg(input_filename);

    std::cerr << "rasterization tool not yet implemented." << std::endl;

    // TODO: Rasterize.
    // TODO: Free tracing results.

    return 0;
}
