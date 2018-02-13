#ifndef MARCHING_SQUARE_HPP
#define MARCHING_SQUARE_HPP

#include <photogeo.h>

/**
* Trace image using marching squares.
 * @param layers Color layers.
 * @param layer_count The number of color layers.
 * @param image_parameters Source image parameters.
 * @param tracing_parameters Tracing parameters.
 * @param tracing_results Variable to store tracing results.
 */
void trace_marching_squares(const ptg_image_parameters* image_parameters, const ptg_quantization_results* quantization_results, ptg_tracing_results* out_tracing_results);

#endif
