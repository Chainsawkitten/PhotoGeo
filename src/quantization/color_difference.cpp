#include "color_difference.hpp"

#include "color_conversion.hpp"
#include <cmath>

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

double color_distance_cie76_sqr(const cie_lab& a, const cie_lab& b) {
    // Calculate delta-E.
    // https://en.wikipedia.org/wiki/Color_difference#CIE76
    // Retrieved 2018-02-14
    return distance(a.l, a.a, a.b,
                    b.l, b.a, b.b);
}

double color_distance_cie94_sqr(const cie_lab& a, const cie_lab& b) {
    // Calculate delta-E, using the k-values for graphic art.
    // https://en.wikipedia.org/wiki/Color_difference#CIE94
    // Retrieved 2018-02-14
    const double k1 = 0.045;
    const double k2 = 0.015;
    const double k_l = 1.0;
    const double k_c = 1.0;
    const double k_h = 1.0;

    const double delta_l = a.l - b.l;
    const double c1 = sqrt(a.a * a.a + a.b * a.b);
    const double c2 = sqrt(b.a * b.a + b.b * b.b);
    const double delta_c = c1 - c2;
    const double delta_a = a.a - b.a;
    const double delta_b = a.b - b.b;
    double delta_h = delta_a * delta_a + delta_b * delta_b - delta_c * delta_c;
    delta_h = (delta_h < 0.0) ? 0.0 : sqrt(delta_h);

    const double s_l = 1.0;
    const double s_c = 1.0 + k1 * c1;
    const double s_h = 1.0 + k2 * c1;

    const double de1 = delta_l / (k_l * s_l);
    const double de2 = delta_c / (k_c * s_c);
    const double de3 = delta_h / (k_h * s_h);

    return de1 * de1 + de2 * de2 + de3 * de3;
}

// Pi.
static const double pi = 3.14159265359;

// Convert angle from radians to degrees.
static double rad2deg(double angle) {
    return angle * 180.0 / pi;
}

// Convert angle from degrees to radians.
static double deg2rad(double angle) {
    return angle * pi / 180.0;
}

// Calculate the angle between 0 and 360 degrees.
static double angle(double b, double a_prime) {
    const double epsilon = 0.00001;

    if (b < epsilon && a_prime < epsilon)
        return 0.0;

    double h = atan2(b, a_prime);
    h = (h < 0.0) ? h + 2.0 * pi : h;
    return rad2deg(h);
}

double color_distance_ciede2000_sqr(const cie_lab& a, const cie_lab& b) {
    // Calculate delta-E.
    // https://en.wikipedia.org/wiki/Color_difference#CIEDE2000
    // Retrieved 2018-02-16
    const double k_l = 1.0;
    const double k_c = 1.0;
    const double k_h = 1.0;

    const double delta_l = b.l - a.l;
    const double l_avg = (a.l + b.l) * 0.5;
    const double c1 = sqrt(a.a * a.a + a.b * a.b);
    const double c2 = sqrt(b.a * b.a + b.b * b.b);
    const double c_avg = (c1 + c2) * 0.5;

    const double a_factor = 1.0 + 0.5 * (1.0 - sqrt(pow(c_avg, 7.0) / (pow(c_avg, 7.0) + pow(25.0, 7.0))));
    const double a1_prime = a.a * a_factor;
    const double a2_prime = b.a * a_factor;

    const double c1_prime = sqrt(a1_prime * a1_prime + a.b * a.b);
    const double c2_prime = sqrt(a2_prime * a2_prime + b.b * b.b);
    const double delta_c = c2_prime - c1_prime;
    const double c_prime_avg = (c1_prime + c2_prime) * 0.5;

    const double h1 = angle(a.b, a1_prime);
    const double h2 = angle(b.b, a2_prime);

    double h_diff = h2 - h1;
    double delta_h = h_diff;
    if (fabs(h_diff) > 180.0)
        delta_h += (h2 > h1) ? -360.0 : 360.0;
    delta_h = 2.0 * sqrt(c1_prime * c2_prime) * sin(deg2rad(delta_h * 0.5));

    double h_avg = h1 + h2;
    if (fabs(h_diff) > 180.0)
        h_avg += (h_avg < 360.0) ? 360.0 : -360.0;
    if (h1 != 0.0 && h2 != 0.0)
        h_avg *= 0.5;

    const double t = 1.0 - 0.17 * cos(deg2rad(h_avg - 30.0))
                         + 0.24 * cos(deg2rad(2.0 * h_avg))
                         + 0.32 * cos(deg2rad(3.0 * h_avg + 6.0))
                         - 0.20 * cos(deg2rad(4.0 * h_avg - 63.0));

    const double l50 = l_avg - 50.0;
    const double s_l = 1.0 + 0.015 * l50 * l50 / sqrt(20.0 + l50 * l50);
    const double s_c = 1.0 + 0.045 * c_prime_avg;
    const double s_h = 1.0 + 0.015 * c_prime_avg * t;

    const double h_exp = (h_avg - 275.0) / 25.0;
    const double r_t = -2.0 * sqrt(pow(c_prime_avg, 7.0) / (pow(c_prime_avg, 7.0) + pow(25.0, 7.0))) * sin(deg2rad(60.0 * exp(-h_exp * h_exp)));

    const double de1 = delta_l / (k_l * s_l);
    const double de2 = delta_c / (k_c * s_c);
    const double de3 = delta_h / (k_h * s_h);

    return de1 * de1 + de2 * de2 + de3 * de3 + r_t * de2 * de3;
}
