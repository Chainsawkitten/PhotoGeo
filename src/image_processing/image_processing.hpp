#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include <photogeo.h>

/**
 * Process image.
 * @param image_parameters Image input parameters.
 * @param Parameters regarding which methods to use during image processing.
 */
void ptgi_image_process(const ptg_image_parameters* image_parameters, const ptg_image_processing_parameters* image_processing_parameters);

#endif
