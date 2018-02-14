#ifndef QUANTIZATION_HPP
#define QUANTIZATION_HPP

#include <photogeo.h>

/*
 * Quantize an image.
 * @param parameters Image input parameters.
 * @param layers Color layers to store results in.
 * @param distance_function Function to calculate the distance between two colors.
 */
void quantize(const ptg_image_parameters* parameters, bool** layers, unsigned int (*distance_function)(const ptg_color&, const ptg_color&));

/*
 * Calculate the squared euclidean distance between two colors in RGB space.
 * @param a First color.
 * @param b Second color.
 * @return The distance between the colors.
 */
unsigned int color_distance_euclidean_sqr(const ptg_color& a, const ptg_color& b);

/*
 * Calculate the CIE76 delta-e between two colors.
 * @param a First color.
 * @param b Second color.
 * @return The perceptual distance between the colors.
 */
unsigned int color_distance_cie76(const ptg_color& a, const ptg_color& b);

#endif
