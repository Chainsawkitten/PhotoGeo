#ifndef QUANTIZATION_HPP
#define QUANTIZATION_HPP

#include <photogeo.h>

/*
 * Quantize an image.
 * @param parameters Image input parameters.
 * @param layers Color layers to store results in.
 * @param quantization_method What method to use when quantizing the image.
 */
void quantize(const ptg_image_parameters* parameters, bool** layers, ptg_quantization_method quantization_method);

#endif
