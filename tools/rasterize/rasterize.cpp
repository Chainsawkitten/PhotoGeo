#include "rasterize.hpp"

#include <iostream>
#include <cstring>

void rasterize(const ptg_tracing_results* tracing_results, unsigned int width, unsigned int height, ptg_color* image) {
    std::cerr << "rasterize has not yet implemented." << std::endl;

    // White background color.
    for (unsigned int i = 0; i < width * height; ++i)
        image[i] = {255, 255, 255};

    bool* collision = new bool[width];

    // Loop through all layers.
    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        // Loop through all rows.
        for (unsigned int y = 0; y < height; ++y) {
            memset(collision, 0, width);

            // TODO: Perform raytracing.

            // Write pixel values.
            bool inside = false;
            for (unsigned int x = 0; x < width; ++x) {
                // Check point in polygon problem.
                if (collision[x])
                    inside = !inside;

                if (inside)
                    // TODO: Use the color of the layer rather than hard-coded black.
                    image[y * width + x] = {0, 0, 0};
            }
        }
    }

    // Clean up.
    delete[] collision;
}
