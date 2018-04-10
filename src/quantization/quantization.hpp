#ifndef QUANTIZATION_HPP
#define QUANTIZATION_HPP

#include <photogeo.h>

struct cie_lab;

/*
 * Quantize an image.
 * @param parameters Image input parameters.
 * @param layers Color layers to store results in.
 * @param quantization_method What method to use when quantizing the image.
 */
void quantize(const ptg_image_parameters* parameters, bool** layers, ptg_quantization_method quantization_method);

/*
 * Calculate the squared euclidean distance between two colors in sRGB space.
 * @param a First color.
 * @param b Second color.
 * @return The distance between the colors.
 */
double color_distance_euclidean_srgb_sqr(const ptg_color& a, const ptg_color& b);

/*
 * Calculate the squared euclidean distance between two colors in linear RGB space.
 * @param a First color.
 * @param b Second color.
 * @return The distance between the colors.
 */
double color_distance_euclidean_linear_sqr(const ptg_color& a, const ptg_color& b);

/*
 * Calculate the squared CIE76 delta-e between two colors.
 * Uses D65 illuminant.
 * @param a First color.
 * @param b Second color.
 * @return The perceptual distance between the colors.
 */
double color_distance_cie76_sqr(const cie_lab& a, const cie_lab& b);

/*
 * Calculate the squared CIE94 delta-e between two colors.
 * Uses D65 illuminant and k-values for graphic art.
 * @param a First color.
 * @param b Second color.
 * @return The perceptual distance between the colors.
 */
double color_distance_cie94_sqr(const cie_lab& a, const cie_lab& b);

/*
 * Calculate the squared CIEDE2000 delta-e between two colors.
 * Uses D65 illuminant.
 * @param a First color.
 * @param b Second color.
 * @return The perceptual distance between the colors.
 */
double color_distance_ciede2000_sqr(const cie_lab& a, const cie_lab& b);

#endif
