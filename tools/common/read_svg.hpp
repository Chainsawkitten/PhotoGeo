#ifndef READ_SVG_HPP
#define READ_SVG_HPP

#include <photogeo.h>

/**
 * Read and parse SVG file.
 * @param filename The filename of the file to read.
 * @param results Where to store the results.
 */
void read_svg(const char* filename, ptg_tracing_results* results);

#endif
