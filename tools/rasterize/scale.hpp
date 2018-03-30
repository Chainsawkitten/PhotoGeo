#ifndef SCALE_HPP
#define SCALE_HPP

#include <photogeo.h>

/**
 * Scale a vector image.
 * @param svg The vector image to scale.
 * @param scale The scale.
 */
void scale_svg(ptg_tracing_results* svg, unsigned int scale);

#endif
