#ifndef MARCHING_SQUARES_HPP
#define MARCHING_SQUARES_HPP

#include <photogeo.h>

/**
 * Trace image using marching squares.
 * @param layer Binary layer.
 * @param layer_width Width of the layer.
 * @param layer_height Height of the layer.
 * @param out_outlines Variable to store resulting outlines.
 * @param out_outline_count Variable to store number of resulting outlines.
 */
void ptgi_trace_marching_squares(bool* layer, unsigned int layer_width, unsigned int layer_height, ptg_outline*& out_outlines, unsigned int& out_outline_count);

#endif
