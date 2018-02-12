#ifndef PHOTOGEO_H
#define PHOTOGEO_H

// Functions need to be imported/exported when compiling a shared library (DLL) on Windows.
#if defined(_WIN32) && defined(BUILD_SHARED)
    #ifdef photogeo_EXPORTS
        #define PHOTOGEO_API __declspec(dllexport)
    #else
        #define PHOTOGEO_API __declspec(dllimport)
    #endif
#else
    #define PHOTOGEO_API
#endif

// Export C API.
#ifdef __cplusplus
extern "C" {
#endif

/// 2-dimensional vector.
struct ptg_vec2 {
    /// X-component.
    unsigned int x;

    /// Y-component.
    unsigned int y;
};

/// 3-channel color in RGB-space.
struct ptg_color {
    /// Red channel.
    unsigned char r;

    /// Green channel;
    unsigned char g;

    /// Blue channel.
    unsigned char b;
};

/// Source image parameters.
struct ptg_image_parameters {
    /// Source image in 3-channel RGB format.
    const ptg_color* image;

    /// The width of the source image.
    unsigned int width;

    /// The height of the source image.
    unsigned int height;

    /// The number of background colors.
    unsigned int background_color_count;

    /// The background colors.
    const ptg_color* background_colors;

    /// The number of color layers.
    unsigned int color_layer_count;

    /// The colors of the different color layers.
    const ptg_color* color_layer_colors;
};

/// Parameters regarding the quantization step.
struct ptg_quantization_parameters {
    char padding;
};

/// Parameters for generating collision geometry.
struct ptg_generation_parameters {
    /// Source image parameters.
    const ptg_image_parameters* image_parameters;

    /// Parameters regarding the quantization step.
    const ptg_quantization_parameters* quantization_parameters;
};

/**
 * Generate collision geometry from image.
 * @param parameters Input parameters.
 * @param out_vertex_count Variable to store resulting vertex counts.
 * @param out_vertices Variable to store resulting vertices.
 * @todo Implement this. Currently does nothing.
 */
PHOTOGEO_API void ptg_generate_collision_geometry(const ptg_generation_parameters* parameters, unsigned int** out_vertex_count, ptg_vec2*** out_vertices);

/**
 * Deallocate the memory that was allocated to store the results.
 * @param vertex_count Vertex count.
 * @param vertices Vertices.
 */
PHOTOGEO_API void ptg_free_results(unsigned int* vertex_count, ptg_vec2** vertices);

/**
 * Quantize image.
 * @param image_parameters Source image parameters.
 * @param quantization_parameters Quantization parameters.
 * @return Resulting color layers.
 */
PHOTOGEO_API bool** ptg_quantize(const ptg_image_parameters* image_parameters, const ptg_quantization_parameters* quantization_parameters);

/**
 * Free color layers allocated during quantization.
 * @param layers Color layers.
 * @param layer_count The number of color layers.
 */
PHOTOGEO_API void ptg_free_quantization_results(bool** layers, unsigned int layer_count);

#ifdef __cplusplus
}
#endif

#endif
