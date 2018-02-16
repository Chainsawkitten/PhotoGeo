#include "color_conversion.hpp"

#include <cmath>

cie_xyz rgb_to_xyz(const ptg_color& color) {
    cie_xyz result;

    // Convert from sRGB to linear RGB.
    double r = srgb_to_linear(color.r);
    double g = srgb_to_linear(color.g);
    double b = srgb_to_linear(color.b);

    // Convert to XYZ, assuming D65 illuminant with a 2 degree standard observer.
    // https://en.wikipedia.org/wiki/SRGB#The_forward_transformation_(CIE_XYZ_to_sRGB)
    // Retrieved 2018-02-14
    result.x = 0.4124 * r + 0.3576 * g + 0.1805 * b;
    result.y = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    result.z = 0.0193 * r + 0.1192 * g + 0.9505 * b;

    // Scale values from normalized Y=1 to Y=100.
    result.x *= 100;
    result.y *= 100;
    result.z *= 100;

    return result;
}

// Helper function for xyz_to_lab.
static double f(double t) {
    if (t > 0.008856)
        return pow(t, 1.0 / 3.0);

    return t * 7.787 + 4.0 / 29.0;
}

cie_lab xyz_to_lab(const cie_xyz& color) {
    cie_lab result;

    // Convert to L*a*b* using illuminant D65 with normalization Y=100.
    // https://en.wikipedia.org/wiki/Lab_color_space#CIELAB-CIEXYZ_conversions
    // Retrieved 2018-02-14
    const double xn = 95.047;
    const double yn = 100.0;
    const double zn = 108.883;

    result.l = 116.0 * f(color.y / yn) - 16.0;
    result.a = 500.0 * (f(color.x / xn) - f(color.y / yn));
    result.b = 200.0 * (f(color.y / yn) - f(color.z / zn));

    return result;
}

double srgb_to_linear(unsigned char component) {
    // https://en.wikipedia.org/wiki/SRGB#The_reverse_transformation
    // Retrieved 2018-02-14
    double a = 0.055;
    double cutoff = 0.04045;

    double c = (double)component / 255.0;
    return c > cutoff ? pow(c + a / (1.0 + a), 2.4) : (c / 12.92);
}
