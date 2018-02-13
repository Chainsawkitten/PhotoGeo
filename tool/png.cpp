#include "png.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void WriteQuantizedToPNG(const char* filename, const bool* const* layers, unsigned int width, unsigned int height, const ptg_color* colors, unsigned int colorCount) {
    // Allocate image data.
    const unsigned int components = 3;
    unsigned char* data = new unsigned char[width * height * components];

    // Make image.
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            // White background.
            data[(y * width + x) * components + 0] = 255;
            data[(y * width + x) * components + 1] = 255;
            data[(y * width + x) * components + 2] = 255;

            // Color layers.
            for (unsigned int layer = 0; layer < colorCount; ++layer) {
                if (layers[layer][y * width + x]) {
                    data[(y * width + x) * components + 0] = colors[layer].r;
                    data[(y * width + x) * components + 1] = colors[layer].g;
                    data[(y * width + x) * components + 2] = colors[layer].b;
                }
            }
        }
    }

    // Write image to disk.
    stbi_write_png(filename, width, height, components, data, width * components);

    // Cleanup.
    delete[] data;
}
