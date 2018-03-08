#ifndef READ_SVG_HPP
#define READ_SVG_HPP

#include <photogeo.h>

/**
 * Read and parse SVG file.
 * @param filename The filename of the file to read.
 * @return The contours in the SVG file.
 */
ptg_tracing_results read_svg(const char* filename);

#endif
