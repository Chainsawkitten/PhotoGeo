#ifndef VERTEX_COUNT_HPP
#define VERTEX_COUNT_HPP

#include <photogeo.h>

/**
 * Write vertex count to log file.
 * @param filename Name of the file to write to.
 * @param svg The SVG file to count the vertices in.
 */
void log_vertex_count(const char* filename, const ptg_tracing_results* svg);

#endif
