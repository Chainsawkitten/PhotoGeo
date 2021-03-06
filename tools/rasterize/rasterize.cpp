#include "rasterize.hpp"

#include <cstring>

void rasterize(const ptg_tracing_results* tracing_results, const ptg_color* colors, unsigned int width, unsigned int height, ptg_color* image) {
    // White background color.
    for (unsigned int i = 0; i < width * height; ++i)
        image[i] = {255, 255, 255};

    bool* collision = new bool[width];

    // Loop through all layers.
    for (unsigned int layer = 0; layer < tracing_results->layer_count; ++layer) {
        // Loop through all rows.
        for (unsigned int y = 0; y < height; ++y) {
            memset(collision, 0, width);

            // Loop through all lines.
            for (unsigned int outline = 0; outline < tracing_results->outline_counts[layer]; ++outline) {
                for (unsigned int line = 0; line < tracing_results->outlines[layer][outline].vertex_count - 1; ++line) {
                    // Check if line intersects with the row.
                    // SVG has double the width/height of PNG.
                    ptg_vec2 v1 = tracing_results->outlines[layer][outline].vertices[line];
                    ptg_vec2 v2 = tracing_results->outlines[layer][outline].vertices[line + 1];
                    if ((v2.y > y * 2 + 1) == (v1.y > y * 2 + 1))
                        continue;

                    // Find intersection point.
                    const double x1 = v1.x;
                    const double y1 = v1.y;
                    const double x2 = v2.x;
                    const double y2 = v2.y;
                    const double k = (x2 - x1) / (y2 - y1);
                    unsigned int intersection = x1 + k * ((double)y * 2.0 + 1.0 - y1);
                    intersection /= 2;

                    // Mark intersection point.
                    if (intersection < width)
                        collision[intersection] = !collision[intersection];
                }
            }

            // Write pixel values.
            bool inside = false;
            for (unsigned int x = 0; x < width; ++x) {
                // Check point in polygon problem.
                if (collision[x])
                    inside = !inside;

                if (inside)
                    image[y * width + x] = colors[layer];
            }
        }
    }

    // Clean up.
    delete[] collision;
}
