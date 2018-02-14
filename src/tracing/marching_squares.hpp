#ifndef MARCHING_SQUARES_HPP
#define MARCHING_SQUARES_HPP

#include <photogeo.h>

/**
 * Trace image using marching squares.
 * @param image_parameters Source image parameters.
 * @param quantization_results Quantization parameters.
 * @param out_tracing_results Variable to store tracing results.
 */
void trace_marching_squares(const ptg_image_parameters* image_parameters, const ptg_quantization_results* quantization_results, ptg_tracing_results* out_tracing_results);

#endif
