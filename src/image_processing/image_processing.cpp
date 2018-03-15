#include "image_processing.hpp"

#include <opencv2\imgproc.hpp>

void ptg_image_process(const ptg_image_parameters* image_parameters, const ptg_image_processing_parameters* image_processing_parameters) {
    cv::Mat src = cv::Mat(image_parameters->height, image_parameters->width, CV_8UC3, image_parameters->image);
    cv::Mat dst = cv::Mat(image_parameters->height, image_parameters->width, CV_8UC3);
    cv::filter2D(src, dst, src.depth(), cv::getGaussianKernel(3, 0.8));
    std::memcpy(image_parameters->image, dst.data, image_parameters->width * image_parameters->height * sizeof(ptg_color));
}
