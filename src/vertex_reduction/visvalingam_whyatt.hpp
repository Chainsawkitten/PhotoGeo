#ifndef VISVALINGAM_WHYATT_HPP
#define VISVALINGAM_WHYATT_HPP

#include <photogeo.h>

/**
 * Reduce vertex geometry using Visvalingam-Whyatt.
 * @param tracing_results Results from the tracing step.
 * @todo Optimize using min-heap.
 * @todo Handle complete removal of outline.
 * @todo Look into resizing result array (realloc).
 */
void ptgi_visvalingam_whyatt(ptg_tracing_results* tracing_results);

#endif
