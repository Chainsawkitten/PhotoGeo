#ifndef DOUGLAS_PEUCKER_HPP
#define DOUGLAS_PEUCKER_HPP

#include <photogeo.h>

/**
 * Reduce vertex geometry using Douglas-Peucker.
 * @param tracing_results Results from the tracing step.
 */
void ptgi_douglas_peucker(ptg_tracing_results* tracing_results);

#endif
