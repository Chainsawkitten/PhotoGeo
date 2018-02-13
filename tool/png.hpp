#ifndef PNG_HPP
#define PNG_HPP

#include <photogeo.h>

/**
 * Write quantized image to PNG file.
 * @param filename The name of the file to write to.
 * @param layers The binary color layers.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param colors The foreground colors.
 * @param colorCount The number of layers.
 */
void WriteQuantizedToPNG(const char* filename, const bool* const* layers, unsigned int width, unsigned int height, const ptg_color* colors, unsigned int colorCount);

#endif
