#include "image_processing.hpp"

#include <cstring>
#include "kuwahara.hpp"

void ptgi_image_process(const ptg_image_parameters* image_parameters, const ptg_image_processing_parameters* image_processing_parameters) {
    cv::Mat src = cv::Mat(image_parameters->height, image_parameters->width, CV_8UC3, image_parameters->image);
    cv::Mat dst = cv::Mat(image_parameters->height, image_parameters->width, CV_8UC3);
    for (unsigned int i = 0; i < image_processing_parameters->method_count; ++i) {
        switch (image_processing_parameters->methods[i]) {
            case PTG_GAUSSIAN_BLUR:        
                cv::filter2D(src, dst, -1, cv::getGaussianKernel(3, 0.5), cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
                memcpy(image_parameters->image, dst.data, image_parameters->width * image_parameters->height * sizeof(ptg_color));
                break;
            case PTG_BILATERAL_FILTER:
                cv::bilateralFilter(src, dst, -1, 50, 5, cv::BORDER_DEFAULT);
                memcpy(image_parameters->image, dst.data, image_parameters->width * image_parameters->height * sizeof(ptg_color));
                break;
            case PTG_MEDIAN_FILTER:
                cv::medianBlur(src, dst, 3);
                memcpy(image_parameters->image, dst.data, image_parameters->width * image_parameters->height * sizeof(ptg_color));
                break;
            case PTG_KUWAHARA_FILTER:
                kuwahara_filter(src, dst, 2);
                memcpy(image_parameters->image, dst.data, image_parameters->width * image_parameters->height * sizeof(ptg_color));
                break;
        }
    }
}
