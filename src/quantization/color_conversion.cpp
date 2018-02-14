#include "color_conversion.hpp"

#include <cmath>

cie_xyz rgb_to_xyz(const ptg_color& color) {
    cie_xyz result;

    // Convert from sRGB to linear RGB.
    double r = srgb_to_linear(color.r);
    double g = srgb_to_linear(color.g);
    double b = srgb_to_linear(color.b);

    // TODO: Convert to XYZ.

    return result;
}

cie_lab xyz_to_lab(const cie_xyz& color) {
    cie_lab result;

    // TODO: Implement.

    return result;
}

double srgb_to_linear(unsigned char component) {
    // https://en.wikipedia.org/wiki/SRGB#The_reverse_transformation

    double a = 0.055;
    double cutoff = 0.04045;

    double c = (double)component / 255.0;
    return c > cutoff ? pow(c + a / (1.0 + a), 2.4) : (c / 12.92);
}
