#include "kuwahara.hpp"

#include <cstring>
#include <limits>

/*
 * Calculate squared length of a vector.
 * @param v The vector.
 * @return Length of the vector.
 */
static double sqr_length(const cv::Vec3i& v) {
    return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/*
 * Clamp value.
 * @param v Value to clamp.
 * @param lower Lower bound.
 * @param upper Upper bound.
 * @return Clamped value.
 */
template <typename T>
T clamp(const T& v, const T& lower, const T& upper) {
    return std::max(lower, std::min(v, upper));
}

/*
 * Calculate mean and variance for a quadrant.
 * @param src Source image.
 * @param r Row.
 * @param c Column.
 * @param kernel_size Size of the kernel N, where the total size is 2*N+1.
 * @param quadrant Current quadrant [1,2,3,4].
 * @param out_mean Variable to store mean.
 * @param out_sqr_variance Variable to store squared variance.
 */
static void kuwahara_kernel(const cv::Mat& src, int r, int c, int kernel_size, int quadrant, cv::Vec3i& out_mean, double& out_sqr_variance) {
    // Calculate mean.
    cv::Vec3i mean = cv::Vec3i(0, 0, 0);
    int r_start = r - kernel_size * (quadrant == 1 || quadrant == 2);
    int c_start = c - kernel_size * (quadrant == 2 || quadrant == 3);
    for (int r_offset = 0; r_offset < kernel_size + 1; ++r_offset)
        for (int c_offset = 0; c_offset < kernel_size + 1; ++c_offset)
            mean += src.at<cv::Vec3b>(clamp(r_start + r_offset, 0, src.rows - 1), clamp(c_start + c_offset, 0, src.cols - 1));
    mean = mean / ((kernel_size + 1) * (kernel_size + 1));

    // Calculate variance.
    double sqr_variance = 0.0;
    for (int r_offset = 0; r_offset < kernel_size + 1; ++r_offset) {
        for (int c_offset = 0; c_offset < kernel_size + 1; ++c_offset) {
            const cv::Vec3i& px = src.at<cv::Vec3b>(clamp(r_start + r_offset, 0, src.rows - 1), clamp(c_start + c_offset, 0, src.cols - 1));
            sqr_variance += sqr_length(px - mean);
        }
    }     
    sqr_variance = sqr_variance / ((kernel_size + 1) * (kernel_size + 1));

    out_mean = mean;
    out_sqr_variance = sqr_variance;
}

void kuwahara_filter(const cv::Mat& src, cv::Mat& dst, int kernel_size) {
    for (int c = 0; c < src.cols; ++c) {
        for (int r = 0; r < src.rows; ++r) {
            cv::Vec3i mean, in_mean;
            double in_sqr_variance;
            double sqr_variance = std::numeric_limits<double>::max();
            // Execute kuwahara kernel for each quadrant surrounding pixel. 
            for (int quadrant = 1; quadrant <= 4; ++quadrant) {
                kuwahara_kernel(src, r, c, kernel_size, quadrant, in_mean, in_sqr_variance);
                // Store mean with lowest variance.
                if (in_sqr_variance < sqr_variance) {
                    sqr_variance = in_sqr_variance;
                    mean = in_mean;
                }
            }
            // Set value to destination image.
            dst.at<cv::Vec3b>(r, c) = mean;
        }
    }
}
