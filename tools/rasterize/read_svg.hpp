#ifndef READ_SVG_HPP
#define READ_SVG_HPP

#include <photogeo.h>

/**
 * Read and parse SVG file.
 * @param filename The filename of the file to read.
 * @param results Where to store the results.
 * @param colors Where to store the layer colors.
 * @param width Where to store the width of the image.
 * @param height Where to store the height of the image.
 */
void read_svg(const char* filename, ptg_tracing_results* results, ptg_color** colors, unsigned int* width, unsigned int* height);

/**
 * Free the results for read_svg.
 * @param results Tracing results.
 * @param colors Layer colors.
 */
void free_svg_results(ptg_tracing_results* results, ptg_color** colors);

#endif
