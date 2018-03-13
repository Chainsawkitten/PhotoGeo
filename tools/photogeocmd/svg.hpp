#ifndef SVG_HPP
#define SVG_HPP

#include <photogeo.h>

/**
 * Output outlines to SVG file.
 * @param filename Name of the file to write to.
 * @param image_parameters Source image parameters.
 * @param outlines The outlines in each layer.
 * @param outline_counts The vertex count for each layer.
 * @param markers Whether to place markers on the vertices.
 */
void write_svg(const char* filename, const ptg_image_parameters* image_parameters, ptg_outline** outlines, unsigned int* outline_counts, bool markers = false);

#endif
