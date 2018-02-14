#include "quantization.hpp"

#include <limits>
#include "color_conversion.hpp"

void quantize(const ptg_image_parameters* parameters, bool** layers, double (*distance_function)(const ptg_color&, const ptg_color&)) {
    for (unsigned int y = 0; y < parameters->height; ++y) {
        for (unsigned int x = 0; x < parameters->width; ++x) {
            ptg_color color = parameters->image[y * parameters->width + x];

            // Find closest color.
            unsigned int layer = 0;
            double shortest = std::numeric_limits<double>::max();
            for (unsigned int i = 0; i < parameters->background_color_count + parameters->color_layer_count; ++i) {
                // Color to compare to.
                ptg_color comparisonColor;
                if (i < parameters->background_color_count)
                    comparisonColor = parameters->background_colors[i];
                else
                    comparisonColor = parameters->color_layer_colors[i - parameters->background_color_count];

                // Calculate distance.
                double distance = distance_function(color, comparisonColor);
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

double color_distance_euclidean_sqr(const ptg_color& a, const ptg_color& b) {
    // Red.
    double difference = (double)a.r - b.r;
    double distance = difference * difference;

    // Green.
    difference = (double)a.g - b.g;
    distance += difference * difference;

    // Blue.
    difference = (double)a.b - b.b;
    distance += difference * difference;

    return distance;
}

double color_distance_cie76_sqr(const ptg_color& a, const ptg_color& b) {
    // Convert to CIE L*a*b*.
    cie_lab a_lab = xyz_to_lab(rgb_to_xyz(a));
    cie_lab b_lab = xyz_to_lab(rgb_to_xyz(b));

    // Calculate delta-E.

    // L*.
    double difference = a_lab.l - b_lab.l;
    double distance = difference * difference;

    // a*.
    difference = a_lab.a - b_lab.a;
    distance += difference * difference;

    // b*.
    difference = a_lab.b - b_lab.b;
    distance += difference * difference;

    return distance;
}
