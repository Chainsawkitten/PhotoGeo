#include "color_conversion.hpp"

#include <cmath>

cie_xyz rgb_to_xyz(const ptg_color& color) {
    cie_xyz result;

    double r = color.r;
    double g = color.g;
    double b = color.b;

    // Convert from sRGB to linear.
    r = srgb_to_linear(r);
    g = srgb_to_linear(g);
    b = srgb_to_linear(b);

    // TODO: Convert to XYZ.

    return result;
}

cie_lab xyz_to_lab(const cie_xyz& color) {
    cie_lab result;

    // TODO: Implement.

    return result;
}

double srgb_to_linear(double c) {
    double a = 0.055;
    double cutoff = 0.04045;

    return c > cutoff ? pow(c + a / (1.0 + a), 2.4) : (c / 12.92);
}
