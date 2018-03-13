#include <iostream>
#include <read_svg.hpp>

int main(int argc, const char* argv[]) {
    // Handle commandline arguments.
    const char* first_filename = "";
    const char* second_filename = "";

    for (int argument = 1; argument < argc; ++argument) {
        // All arguments start with -.
        if (argv[argument][0] == '-') {
            // Input filename.
            if (argv[argument][1] == '1' && argc > argument + 1)
                first_filename = argv[++argument];

            // Output filename.
            if (argv[argument][1] == '2' && argc > argument + 1)
                second_filename = argv[++argument];
        }
    }

    // Display help if no valid configuration was given.
    if (first_filename[0] == '\0' || second_filename[0] == '\0') {
        std::cout << "usage: compare -1 first.svg -2 second.svg" << std::endl << std::endl;

        std::cout << "Parameters:" << std::endl;
        std::cout << "  -1  Specify filename of first SVG." << std::endl;
        std::cout << "  -2  Specify filename of second SVG." << std::endl;

        return 0;
    }

    // Load SVG images.
    ptg_tracing_results svg1, svg2;
    ptg_color *colors1, *colors2;
    unsigned width1, height1, width2, height2;
    read_svg(first_filename, &svg1, &colors1, &width1, &height1);
    read_svg(second_filename, &svg2, &colors2, &width2, &height2);

    std::cout << "Compare tool not yet implemented." << std::endl;

    // TODO: Perform comparison.

    // Free tracing results.
    free_svg_results(&svg1, &colors1);
    free_svg_results(&svg2, &colors2);

    return 0;
}
