#ifndef KUWAHARA_HPP
#define KUWAHARA_HPP

#include <opencv2/imgproc.hpp>

/**
 * Non-linear smoothing filter.
 * @param src Source image.
 * @param dst Destination image.
 * @param kernel_size Size of the kernel N, where the total size is 2*N+1.
 */
void kuwahara_filter(const cv::Mat& src, cv::Mat& dst, int kernel_size);

#endif
