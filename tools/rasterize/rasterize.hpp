#ifndef RASTERIZE_HPP
#define RASTERIZE_HPP

#include <photogeo.h>

/**
 * Rasterize vector contours.
 * @param tracing_results Contours to rasterize.
 * @param colors The layer colors.
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param image The image to write results to.
 */
void rasterize(const ptg_tracing_results* tracing_results, const ptg_color* colors, unsigned int width, unsigned int height, ptg_color* image);

#endif
