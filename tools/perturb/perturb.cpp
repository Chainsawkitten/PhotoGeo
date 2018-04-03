#include "perturb.hpp"

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <cstring>

// Helper functions.
static void extract_alpha_channel(cv::Mat& alpha_channel, const unsigned char* data);

void perturb(unsigned char* data, unsigned int width, unsigned int height) {
    std::cout << "perturb has not yet been implemented." << std::endl;

    // TODO: Replace background color with black.
    // Make alpha channel (background = transparent, foreground = opaque).
    cv::Mat alpha_channel(height, width, CV_8UC1);
    extract_alpha_channel(alpha_channel, data);

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
}
