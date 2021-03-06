#ifndef COLOR_CONVERSION_HPP
#define COLOR_CONVERSION_HPP

#include <photogeo.h>

// Color in CIE 1931 XYZ.
struct cie_xyz {
    double x;
    double y;
    double z;
};

// Color in CIE L*a*b*.
struct cie_lab {
    double l; //< Lightness.
    double a; //< Green-red.
    double b; //< Blue-yellow.
};

/*
 * Convert color from RGB space to CIE 1931 XYZ space.
 * Uses D65 illuminant.
 * @param color The color to convert.
 * @return The color in CIE 1931 XYZ space.
 */
cie_xyz rgb_to_xyz(const ptg_color& color);

/*
 * Convert color from CIE 1931 XYZ space to CIE L*a*b* space.
 * Uses D65 illuminant.
 * @param color The color to convert.
 * @return The color in CIE L*a*b* space.
 */
cie_lab xyz_to_lab(const cie_xyz& color);

/*
 * Convert sRGB component to linear RGB.
 * @param component Component to convert (0-255).
 * @return Component in linear space (0-1).
 */
double srgb_to_linear(unsigned char component);

#endif
