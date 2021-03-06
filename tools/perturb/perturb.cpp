#include "perturb.hpp"

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <cstring>
#include <stb_image.h>
#include <random>
#include <chrono>

// Helper functions.
static void extract_alpha_channel(cv::Mat& alpha_channel, const unsigned char* data);
static void replace_background(cv::Mat& color_channels);
static void divide_channels(cv::Mat& color_channels, const cv::Mat& alpha_channel);
static void blend(cv::Mat& results, const cv::Mat& color_channels, const cv::Mat& alpha_channel);
static void load_texture(cv::Mat& output, const char* filename);
static void mirror(cv::Mat& image, int flip_code);
static void tile(const cv::Mat& image, cv::Mat& result, int xoffset, int yoffset);
static void multiply_channels(cv::Mat& alpha_channel, const cv::Mat& marker);

void perturb(unsigned char* data, unsigned int width, unsigned int height) {
    // Set up random number generator.
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 engine(seed);

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

    // Load marker texture and tile.
    cv::Mat marker_texture;
    load_texture(marker_texture, "perturb_data/marker.png");

    std::uniform_int_distribution<int> xoffset_distribution(0, width - 1);
    std::uniform_int_distribution<int> yoffset_distribution(0, height - 1);
    cv::Mat marker_tiled(height, width, CV_8UC3);
    tile(marker_texture, marker_tiled, xoffset_distribution(engine), yoffset_distribution(engine));

    stbi_image_free(marker_texture.data);

    // Multiply alpha channel and marker texture.
    multiply_channels(alpha_channel_small, marker_tiled);

    // Decide which paper texture to use.
    std::uniform_int_distribution<int> paper_distribution(0, 4);
    std::string paper_filename = "perturb_data/paper" + std::to_string(paper_distribution(engine)) + ".jpg";

    // Load paper texture and scale.
    cv::Mat paper_texture;
    load_texture(paper_texture, paper_filename.c_str());

    cv::Mat results(height, width, CV_8UC3);
    cv::resize(paper_texture, results, cv::Size(width, height));

    stbi_image_free(paper_texture.data);

    // Randomly mirror paper.
    std::uniform_int_distribution<int> bool_distribution(0, 1);
    if (bool_distribution(engine))
        mirror(results, 0);

    if (bool_distribution(engine))
        mirror(results, 1);

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

/*
 * Load an OpenCV texture from a file.
 * The matrix data needs to be deallocated with stbi_image_free.
 * @param output Matrix to store the texture in.
 * @param filename Filename of the texture to load.
 */
static void load_texture(cv::Mat& output, const char* filename) {
    // Load image.
    int components, width, height;
    unsigned char* data = stbi_load(filename, &width, &height, &components, 0);

    if (data == NULL)
        std::cerr << "Couldn't load image " << filename << "." << std::endl;

    // Create OpenCV texture.
    output = cv::Mat(height, width, CV_8UC3, data);
}

/*
 * Mirror image.
 * @param image Image to mirror.
 * @param flip_code OpenCV flip code. See cv::flip.
 */
static void mirror(cv::Mat& image, int flip_code) {
    cv::Mat temp(image.rows, image.cols, image.type());
    cv::flip(image, temp, flip_code);
    image = temp;
}

/*
 * Repeat an image to fill another image.
 * @param image The image to tile.
 * @param result Where to store the resulting image.
 */
static void tile(const cv::Mat& image, cv::Mat& result, int xoffset, int yoffset) {
    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            result.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>((y + yoffset) % image.rows, (x + xoffset) % image.cols);
        }
    }
}

/*
 * Multiple the alpha channel with the marker texture.
 * @param alpha_channel The alpha channel.
 * @param marker The tiled marker
 */
static void multiply_channels(cv::Mat& alpha_channel, const cv::Mat& marker) {
    for (int y = 0; y < alpha_channel.rows; ++y) {
        for (int x = 0; x < alpha_channel.cols; ++x) {
            const double alpha1 = alpha_channel.at<unsigned char>(y, x) / 255.0;
            const double alpha2 = marker.at<cv::Vec3b>(y, x)[0] / 255.0;
            alpha_channel.at<unsigned char>(y, x) = alpha1 * alpha2 * 255.0 + 0.5;
        }
    }
}
