#ifndef EUCLIDEAN_HPP
#define EUCLIDEAN_HPP

#include <photogeo.h>

/**
 * Quantize an image using euclidean distance in RGB space.
 * @param parameters Image input parameters.
 * @param layers Color layers to store results in.
 */
void quantize_euclidean(const ptg_image_parameters* parameters, bool** layers);

#endif
