#ifndef SVG_HPP
#define SVG_HPP

#include <photogeo.h>

/**
 * Output outlines to SVG file.
 * @param layers The number of layers.
 * @param colors The color of each layer.
 * @param vertexCount The vertex count of each layer.
 * @param vertices The vertices in each layer.
 */
void WriteSVG(unsigned int layers, ptg_color* colors, unsigned int* vertexCount, ptg_vec2** vertices);

#endif
