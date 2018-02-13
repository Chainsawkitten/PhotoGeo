#ifndef EUCLIDEAN_HPP
#define EUCLIDEAN_HPP

#include <photogeo.h>

/**
 * Quantize an image using euclidean distance in RGB space.
 * @param parameters Image input parameters.
 * @param layers Color layers to store results in.
 */
void quantize_euclidean(const ptg_image_parameters* parameters, bool** layers);

/**
 * Calculate the squared distance between two colors.
 * @param a First color.
 * @param b Second color.
 * @return The distance between the colors.
 */
unsigned int color_distance_euclidean_sqr(const ptg_color& a, const ptg_color& b);

#endif
