#include "perturb.hpp"

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <cstring>

// Helper functions.
static void extract_alpha_channel(cv::Mat& alpha_channel, const unsigned char* data);
static void replace_background(cv::Mat& color_channels);

void perturb(unsigned char* data, unsigned int width, unsigned int height) {
    std::cout << "perturb has not yet been implemented." << std::endl;

    // Make alpha channel (background = transparent, foreground = opaque).
    cv::Mat alpha_channel(height, width, CV_8UC1);
    extract_alpha_channel(alpha_channel, data);

    // Replace background color with black.
    cv::Mat color_channels(height, width, CV_8UC3, data);
    replace_background(color_channels);

    // TODO: Gaussian blur.
    // TODO: Downscale.
    // TODO: Divide color channels with alpha channel.
    // TODO: Load marker texture and tile.
    // TODO: Multiply alpha channel and marker texture.
    // TODO: Load paper texture and scale.
    // TODO: Blend image with paper texture according to alpha channel.

    // Temp: Write back results.
    memcpy(data, color_channels.data, width * height * 3);
}

/*
 * Create an alpha channel from an image. All white pixels become fully transparent and all other pixels fully opaque.
 * @param alpha_channel Where to store the results.
 * @param data Source image.
 */
static void extract_alpha_channel(cv::Mat& alpha_channel, const unsigned char* data) {
    for (unsigned int y = 0; y < alpha_channel.rows; ++y) {
        for (unsigned int x = 0; x < alpha_channel.cols; ++x) {
            unsigned int pos = (y * alpha_channel.cols + x) * 3;
            bool is_white = data[pos + 0] == 255 &&
                            data[pos + 1] == 255 &&
                            data[pos + 2] == 255;

            alpha_channel.at<unsigned char>(y, x) = is_white ? 0 : 255;
        }
    }
}

/*
 * Replace background with black.
 * @param color_channels Image to replace background in.
 * @param width Width of the image.
 * @param height Height of the image.
 */
static void replace_background(cv::Mat& color_channels) {
    for (int y = 0; y < color_channels.rows; ++y) {
        for (int x = 0; x < color_channels.cols; ++x) {
            cv::Vec3b color = color_channels.at<cv::Vec3b>(y, x);
            if (color[0] == 255 && color[1] == 255 && color[2] == 255)
                color_channels.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
        }
    }
}
