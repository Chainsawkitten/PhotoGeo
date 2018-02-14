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

// Calculate the distance between two points using the Pythagoran theorem.
static double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    double difference = x2 - x1;
    double distance = difference * difference;

    difference = y2 - y1;
    distance += difference * difference;

    difference = z2 - z1;
    distance += difference * difference;

    return distance;
}

double color_distance_euclidean_srgb_sqr(const ptg_color& a, const ptg_color& b) {
    return distance(a.r, a.g, a.b,
                    b.r, b.g, b.b);
}

double color_distance_euclidean_linear_sqr(const ptg_color& a, const ptg_color& b) {
    return distance(srgb_to_linear(a.r), srgb_to_linear(a.g), srgb_to_linear(a.b),
                    srgb_to_linear(b.r), srgb_to_linear(b.g), srgb_to_linear(b.b));
}

double color_distance_cie76_sqr(const ptg_color& a, const ptg_color& b) {
    // Convert to CIE L*a*b*.
    cie_lab a_lab = xyz_to_lab(rgb_to_xyz(a));
    cie_lab b_lab = xyz_to_lab(rgb_to_xyz(b));

    // Calculate delta-E.
    return distance(a_lab.l, a_lab.a, a_lab.b,
                    b_lab.l, b_lab.a, b_lab.b);
}
