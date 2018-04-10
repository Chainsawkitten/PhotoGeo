#include "quantization.hpp"

#include <limits>
#include "color_conversion.hpp"
#include "color_difference.hpp"

// Convert from one color space to another.
template<typename color_type>
static inline color_type convert(const ptg_color& color);

template<>
inline ptg_color convert<ptg_color>(const ptg_color& color) {
    return color;
}

template<>
inline cie_lab convert<cie_lab>(const ptg_color& color) {
    return xyz_to_lab(rgb_to_xyz(color));
}

template<typename color_type>
static void quantize_helper(const ptg_image_parameters* parameters, bool** layers, double (*distance_function)(const color_type&, const color_type&)) {
    // Get colors each pixel should be compared against (foreground and background colors).
    const unsigned int comparison_color_count = parameters->background_color_count + parameters->color_layer_count;
    ptg_color comparison_colors[comparison_color_count];
    for (unsigned int i = 0; i < comparison_color_count; ++i) {
        if (i < parameters->background_color_count)
            comparison_colors[i] = parameters->background_colors[i];
        else
            comparison_colors[i] = parameters->color_layer_colors[i - parameters->background_color_count];
    }

    // Convert comparison colors to the color space the comparison is performed in.
    color_type comparison_colors_conv[comparison_color_count];
    for (unsigned int i = 0; i < comparison_color_count; ++i) {
        comparison_colors_conv[i] = convert<color_type>(comparison_colors[i]);
    }

    // Loop through all pixels in image.
    for (unsigned int y = 0; y < parameters->height; ++y) {
        for (unsigned int x = 0; x < parameters->width; ++x) {
            ptg_color color = parameters->image[y * parameters->width + x];

            // Convert color to color space the comparison is performed in.
            color_type color_conv = convert<color_type>(color);

            // Find closest color.
            unsigned int layer = 0;
            double shortest = std::numeric_limits<double>::max();
            for (unsigned int i = 0; i < comparison_color_count; ++i) {
                // Calculate distance.
                double distance = distance_function(comparison_colors_conv[i], color_conv);
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

void quantize(const ptg_image_parameters* parameters, bool** layers, ptg_quantization_method quantization_method) {
    switch (quantization_method) {
        case PTG_EUCLIDEAN_SRGB:
            quantize_helper<ptg_color>(parameters, layers, color_distance_euclidean_srgb_sqr);
            break;
        case PTG_EUCLIDEAN_LINEAR:
            quantize_helper<ptg_color>(parameters, layers, color_distance_euclidean_linear_sqr);
            break;
        case PTG_CIE76:
            quantize_helper<cie_lab>(parameters, layers, color_distance_cie76_sqr);
            break;
        case PTG_CIE94:
            quantize_helper<cie_lab>(parameters, layers, color_distance_cie94_sqr);
            break;
        case PTG_CIEDE2000:
            quantize_helper<cie_lab>(parameters, layers, color_distance_ciede2000_sqr);
            break;
    }
}
