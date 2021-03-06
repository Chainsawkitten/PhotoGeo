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
    const double xn = 1.0 / 95.047;
    const double yn = 1.0 / 100.0;
    const double zn = 1.0 / 108.883;

    const double fx = f(color.x * xn);
    const double fy = f(color.y * yn);
    const double fz = f(color.z * zn);

    result.l = 116.0 * fy - 16.0;
    result.a = 500.0 * (fx - fy);
    result.b = 200.0 * (fy - fz);

    return result;
}

/*
 * Lookup table for sRGB to linear conversion.
 *
 * Values generated based on the math in:
 * https://en.wikipedia.org/wiki/SRGB#The_reverse_transformation
 * Retrieved 2018-02-14
 */
static double lookup_table[256] = { 0, 0.000303527, 0.000607054, 0.000910581, 0.00121411, 0.00151763, 0.00182116,
                                    0.00212469, 0.00242822, 0.00273174, 0.00303527, 0.003544, 0.00390426, 0.00428502,
                                    0.00468661, 0.00510933, 0.0055535, 0.00601941, 0.00650736, 0.00701764, 0.00755052,
                                    0.00810629, 0.00868522, 0.00928758, 0.00991363, 0.0105636, 0.0112378, 0.0119365,
                                    0.0126598, 0.0134081, 0.0141816, 0.0149805, 0.0158051, 0.0166556, 0.0175321,
                                    0.018435, 0.0193645, 0.0203207, 0.0213039, 0.0223144, 0.0233522, 0.0244177,
                                    0.025511, 0.0266324, 0.0277819, 0.02896, 0.0301666, 0.0314021, 0.0326666,
                                    0.0339603, 0.0352834, 0.0366362, 0.0380187, 0.0394311, 0.0408737, 0.0423467,
                                    0.0438501, 0.0453842, 0.0469492, 0.0485452, 0.0501725, 0.0518311, 0.0535212,
                                    0.0552431, 0.0569969, 0.0587827, 0.0606008, 0.0624512, 0.0643342, 0.0662499,
                                    0.0681984, 0.07018, 0.0721948, 0.0742429, 0.0763245, 0.0784398, 0.0805889,
                                    0.0827719, 0.0849891, 0.0872405, 0.0895263, 0.0918467, 0.0942018, 0.0965917,
                                    0.0990167, 0.101477, 0.103972, 0.106503, 0.109069, 0.111671, 0.114309,
                                    0.116983, 0.119694, 0.12244, 0.125223, 0.128042, 0.130898, 0.133791,
                                    0.136721, 0.139688, 0.142692, 0.145733, 0.148812, 0.151929, 0.155083,
                                    0.158275, 0.161505, 0.164774, 0.16808, 0.171425, 0.174809, 0.178231,
                                    0.181692, 0.185191, 0.18873, 0.192308, 0.195925, 0.199582, 0.203278,
                                    0.207013, 0.210789, 0.214604, 0.218459, 0.222355, 0.22629, 0.230266,
                                    0.234283, 0.23834, 0.242437, 0.246576, 0.250755, 0.254975, 0.259237,
                                    0.26354, 0.267884, 0.27227, 0.276697, 0.281166, 0.285677, 0.290229,
                                    0.294824, 0.299461, 0.30414, 0.308862, 0.313626, 0.318432, 0.323282,
                                    0.328174, 0.333109, 0.338087, 0.343108, 0.348173, 0.35328, 0.358432,
                                    0.363626, 0.368865, 0.374147, 0.379473, 0.384843, 0.390257, 0.395715,
                                    0.401218, 0.406765, 0.412356, 0.417992, 0.423673, 0.429398, 0.435169,
                                    0.440984, 0.446844, 0.45275, 0.458701, 0.464697, 0.470739, 0.476826,
                                    0.482959, 0.489138, 0.495362, 0.501633, 0.507949, 0.514312, 0.520721,
                                    0.527177, 0.533678, 0.540227, 0.546822, 0.553463, 0.560152, 0.566887,
                                    0.57367, 0.580499, 0.587376, 0.5943, 0.601271, 0.60829, 0.615357,
                                    0.622471, 0.629633, 0.636842, 0.6441, 0.651406, 0.658759, 0.666161,
                                    0.673612, 0.68111, 0.688657, 0.696253, 0.703897, 0.71159, 0.719332,
                                    0.727123, 0.734963, 0.742851, 0.750789, 0.758777, 0.766813, 0.774899,
                                    0.783035, 0.79122, 0.799455, 0.80774, 0.816074, 0.824459, 0.832893,
                                    0.841378, 0.849913, 0.858498, 0.867133, 0.875819, 0.884556, 0.893343,
                                    0.902181, 0.911069, 0.920009, 0.928999, 0.938041, 0.947134, 0.956278,
                                    0.965473, 0.974719, 0.984017, 0.993367, 1.00277, 1.01222, 1.02173,
                                    1.03128, 1.04089, 1.05055, 1.06026, 1.07003, 1.07984, 1.08971,
                                    1.09964, 1.10961, 1.11964, 1.12972 };

double srgb_to_linear(unsigned char component) {
    // Use lookup table for performance.
    return lookup_table[component];
}
