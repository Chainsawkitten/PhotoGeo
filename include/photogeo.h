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

/// Vertices representing one continuous edge.
struct ptg_outline {
    /// Number of vertices.
    unsigned int vertex_count;

    /// Vertex data.
    ptg_vec2* vertices;
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

/// Results from the quantization step.
struct ptg_quantization_results {
    /// The color layers.
    bool** layers;

    /// Number of layers.
    unsigned int layer_count;
};

/// Parameters regarding the tracing step.
struct ptg_tracing_parameters {
    char padding;
};

/// Results from the tracing step.
struct ptg_tracing_results {
    /// The outlines.
    ptg_outline** outlines;

    /// Number of outlines.
    unsigned int* outline_counts;

    /// Number of layers.
    unsigned int layer_count;
};

/// Parameters for generating collision geometry.
struct ptg_generation_parameters {
    /// Source image parameters.
    const ptg_image_parameters* image_parameters;

    /// Parameters regarding the quantization step.
    const ptg_quantization_parameters* quantization_parameters;

    /// Parameters regarding the tracing step.
    const ptg_tracing_parameters* tracing_parameters;
};

/**
 * Generate collision geometry from image.
 * @param parameters Input parameters.
 * @param out_outlines Variable to store resulting outlines.
 * @param out_outline_counts Variable to store resulting outline counts.
 * @todo Implement this. Currently does nothing.
 */
PHOTOGEO_API void ptg_generate_collision_geometry(const ptg_generation_parameters* parameters, ptg_outline*** out_outlines, unsigned int** out_outline_counts);

/**
 * Deallocate the memory that was allocated to store the results.
 * @param outlines The outlines.
 * @param outline_counts Number of outlines.
 */
PHOTOGEO_API void ptg_free_results(ptg_outline** outlines, unsigned int* outline_counts);

/**
 * Quantize image.
 * @param image_parameters Source image parameters.
 * @param quantization_parameters Quantization parameters.
 * @param out_quantization_results Variable to store quantization results.
 */
PHOTOGEO_API void ptg_quantize(const ptg_image_parameters* image_parameters, const ptg_quantization_parameters* quantization_parameters, ptg_quantization_results* out_quantization_results);

/**
 * Free allocated memory for results during quantization.
 * @param quantization_results Quantization results to free.
 */
PHOTOGEO_API void ptg_free_quantization_results(ptg_quantization_results* quantization_results);

/**
 * Trace image.
 * @param image_parameters Source image parameters.
 * @param quantization_results Quantization results.
 * @param tracing_parameters Tracing parameters.
 * @param out_tracing_results Variable to store quantization results.
 */
PHOTOGEO_API void ptg_trace(const ptg_image_parameters* image_parameters, const ptg_quantization_results* quantization_results, const ptg_tracing_parameters* tracing_parameters, ptg_tracing_results* out_tracing_results);

/**
 * Free allocated memory for results during tracing.
 * @param tracing_results Tracing results to free.
 */
void ptg_free_tracing_results(ptg_tracing_results* tracing_results);

#ifdef __cplusplus
}
#endif

#endif
