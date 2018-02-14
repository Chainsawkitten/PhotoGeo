#include "euclidean.hpp"

#include <limits>

void ptgi_quantize_euclidean(const ptg_image_parameters* parameters, bool** layers) {
    for (unsigned int y = 0; y < parameters->height; ++y) {
        for (unsigned int x = 0; x < parameters->width; ++x) {
            ptg_color color = parameters->image[y * parameters->width + x];

            // Find closest color.
            unsigned int layer = 0;
            unsigned int shortest = std::numeric_limits<unsigned int>::max();
            for (unsigned int i = 0; i < parameters->background_color_count + parameters->color_layer_count; ++i) {
                // Color to compare to.
                ptg_color comparisonColor;
                if (i < parameters->background_color_count)
                    comparisonColor = parameters->background_colors[i];
                else
                    comparisonColor = parameters->color_layer_colors[i - parameters->background_color_count];

                // Calculate distance.
                unsigned int distance = ptgi_color_distance_euclidean_sqr(color, comparisonColor);
                if (distance < shortest) {
                    shortest = distance;
                    layer = i;
                }
            }

            // Write color layers.
            for (unsigned int i = 0; i < parameters->color_layer_count; ++i)
                layers[i][y * parameters->width + x] = (i == layer - parameters->background_color_count);
        }
    }
}

unsigned int ptgi_color_distance_euclidean_sqr(const ptg_color& a, const ptg_color& b) {
    // Red.
    int difference = (int)a.r - b.r;
    unsigned int distance = difference * difference;

    // Green.
    difference = (int)a.g - b.g;
    distance += difference * difference;

    // Blue.
    difference = (int)a.b - b.b;
    distance += difference * difference;

    return distance;
}
