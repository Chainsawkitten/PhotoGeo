#include "perturb.hpp"

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <cstring>

// Helper functions.
static void extract_alpha_channel(cv::Mat& alpha_channel, const unsigned char* data);
static void replace_background(cv::Mat& color_channels);
static void divide_channels(cv::Mat& color_channels, const cv::Mat& alpha_channel);
static void blend(cv::Mat& results, const cv::Mat& color_channels, const cv::Mat& alpha_channel);

void perturb(unsigned char* data, unsigned int width, unsigned int height) {
    std::cout << "perturb has not yet been implemented." << std::endl;

    // Make alpha channel (background = transparent, foreground = opaque).
    cv::Mat alpha_channel(height, width, CV_8UC1);
    extract_alpha_channel(alpha_channel, data);

    // Replace background color with black.
    cv::Mat color_channels(height, width, CV_8UC3, data);
    replace_background(color_channels);

    // Gaussian blur.
    const double sigma = 1.5;
    cv::GaussianBlur(alpha_channel, alpha_channel, cv::Size(0, 0), sigma);
    cv::GaussianBlur(color_channels, color_channels, cv::Size(0, 0), sigma);

    // Downscale.
    width /= 2;
    height /= 2;

    cv::Mat alpha_channel_small(height, width, CV_8UC1);
    cv::resize(alpha_channel, alpha_channel_small, cv::Size(width, height));

    cv::Mat color_channels_small(height, width, CV_8UC3);
    cv::resize(color_channels, color_channels_small, cv::Size(width, height));

    // Divide color channels with alpha channel.
    divide_channels(color_channels_small, alpha_channel_small);

    // TODO: Load marker texture and tile.
    // TODO: Multiply alpha channel and marker texture.
    // TODO: Load paper texture and scale.

    // Temp: White background instead of paper texture.
    cv::Mat results(height, width, CV_8UC3);
    for (int y = 0; y < results.rows; ++y) {
        for (int x = 0; x < results.cols; ++x) {
            results.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
        }
    }

    // Blend image with paper texture according to alpha channel.
    blend(results, color_channels_small, alpha_channel_small);

    // Write back results.
    memcpy(data, results.data, width * height * 3);
}

/*
 * Create an alpha channel from an image. All white pixels become fully transparent and all other pixels fully opaque.
 * @param alpha_channel Where to store the results.
 * @param data Source image.
 */
static void extract_alpha_channel(cv::Mat& alpha_channel, const unsigned char* data) {
    for (int y = 0; y < alpha_channel.rows; ++y) {
        for (int x = 0; x < alpha_channel.cols; ++x) {
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

/*
 * Divide the color channels by the alpha channel.
 * @param color_channels The color channels.
 * @param alpha_channel The alpha channel.
 */
static void divide_channels(cv::Mat& color_channels, const cv::Mat& alpha_channel) {
    for (int y = 0; y < color_channels.rows; ++y) {
        for (int x = 0; x < color_channels.cols; ++x) {
            const double alpha = alpha_channel.at<unsigned char>(y, x) / 255.0;
            if (alpha > 0.01) {
                cv::Vec3b color = color_channels.at<cv::Vec3b>(y, x);
                color[0] = (double)color[0] / alpha;
                color[1] = (double)color[1] / alpha;
                color[2] = (double)color[2] / alpha;
                color_channels.at<cv::Vec3b>(y, x) = color;
            }
        }
    }
}

/*
 * Blend results and colors together based on alpha channel.
 * @param results Background texture.
 * @param color_channels Colors.
 * @param alpha_channel Alpha channel.
 */
static void blend(cv::Mat& results, const cv::Mat& color_channels, const cv::Mat& alpha_channel) {
    for (int y = 0; y < color_channels.rows; ++y) {
        for (int x = 0; x < color_channels.cols; ++x) {
            const double alpha = alpha_channel.at<unsigned char>(y, x) / 255.0;
            cv::Vec3b result = results.at<cv::Vec3b>(y, x);
            cv::Vec3b color = color_channels.at<cv::Vec3b>(y, x);

            result[0] = result[0] * (1.0 - alpha) + color[0] * alpha;
            result[1] = result[1] * (1.0 - alpha) + color[1] * alpha;
            result[2] = result[2] * (1.0 - alpha) + color[2] * alpha;

            results.at<cv::Vec3b>(y, x) = result;
        }
    }
}
