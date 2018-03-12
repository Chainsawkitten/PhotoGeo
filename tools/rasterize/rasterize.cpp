#include "rasterize.hpp"

#include <iostream>

void rasterize(const ptg_tracing_results* tracing_results, unsigned int width, unsigned int height, ptg_color* image) {
    std::cerr << "rasterize has not yet implemented." << std::endl;

    // Loop through all rows.
    for (unsigned int y = 0; y < height; ++y) {
        // TODO: Perform raytracing.

        // Write pixel values.
        for (unsigned int x = 0; x < width; ++x) {
            // White background color.
            image[y * width + x] = {255, 255, 255};

            // TODO: Loop through all layers and check point in polygon problem.
        }
    }
}
