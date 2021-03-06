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
    ptg_color* image;

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

/// Methods to use during image processing.
typedef enum {
    PTG_GAUSSIAN_BLUR, ///< Gaussian blur.
    PTG_BILATERAL_FILTER, ///< Bilateral filter.
    PTG_MEDIAN_FILTER, ///< Median filter.
    PTG_KUWAHARA_FILTER ///< Kuwahara filter.
} ptg_image_processing_method;

/// Parameters regarding the image processing step.
struct ptg_image_processing_parameters {
    /// The number of image processing methods to use.
    unsigned int method_count;

    /// Which methods to use during image processing.
    ptg_image_processing_method* methods;
};

/// Method to use to quantize image.
typedef enum {
    PTG_EUCLIDEAN_SRGB, ///< Euclidean distance in sRGB space.
    PTG_EUCLIDEAN_LINEAR, ///< Euclidean distance in linear RGB space.
    PTG_CIE76, ///< CIE76.
    PTG_CIE94, ///< CIE94.
    PTG_CIEDE2000 ///< CIEDE2000.
} ptg_quantization_method;

/// Parameters regarding the quantization step.
struct ptg_quantization_parameters {
    /// Which method to use to quantize the image.
    ptg_quantization_method quantization_method;
};

/// Results from the quantization step.
struct ptg_quantization_results {
    /// Number of layers.
    unsigned int layer_count;

    /// The color layers.
    bool** layers;
};

/// Method to use to trace image.
typedef enum {
    PTG_MARCHING_SQUARES ///<Marching squares.
} ptg_tracing_method;

/// Parameters regarding the tracing step.
struct ptg_tracing_parameters {
    /// Which method to use to trace the image.
    ptg_tracing_method tracing_method;
};

/// Results from the tracing step.
struct ptg_tracing_results {
    /// Number of layers.
    unsigned int layer_count;

    /// Number of outlines per layer.
    unsigned int* outline_counts;

    /// The outlines.
    ptg_outline** outlines;
};

/// Method to use to reduce vertex count.
typedef enum {
    PTG_NO_VERTEX_REDUCTION, ///< Don't perform any vertex reduction.
    PTG_DOUGLAS_PEUCKER, ///< Douglas-Peucker.
    PTG_VISVALINGAM_WHYATT ///< Visvalingam-Whyatt.
} ptg_vertex_reduction_method;

/// Parameters regarding the vertex reduction step.
struct ptg_vertex_reduction_parameters {
    ptg_vertex_reduction_method vertex_reduction_method;
};

/// Parameters for generating collision geometry.
struct ptg_generation_parameters {
    /// Source image parameters.
    const ptg_image_parameters* image_parameters;

    /// Image processing parameters.
    const ptg_image_processing_parameters* image_processing_parameters;

    /// Parameters regarding the quantization step.
    const ptg_quantization_parameters* quantization_parameters;

    /// Parameters regarding the tracing step.
    const ptg_tracing_parameters* tracing_parameters;

    /// Parameters regarding the vertex reduction step.
    const ptg_vertex_reduction_parameters* vertex_reduction_parameters;
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
 * @param layer_count The number of layers.
 * @param outlines The outlines.
 * @param outline_counts Number of outlines.
 */
PHOTOGEO_API void ptg_free_results(unsigned int layer_count, ptg_outline** outlines, unsigned int* outline_counts);

/**
 * Process image.
 * @param image_parameters Source image parameters.
 * @param image_processing_parameters Image processing parameters.
 */
PHOTOGEO_API void ptg_image_process(const ptg_image_parameters* image_parameters, const ptg_image_processing_parameters* image_processing_parameters);

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
PHOTOGEO_API void ptg_free_tracing_results(ptg_tracing_results* tracing_results);

/**
 * Reduce vertex count.
 * @param tracing_results Tracing results.
 * @param vertex_reduction_parameters Vertex reduction parameters.
 */
PHOTOGEO_API void ptg_reduce(ptg_tracing_results* tracing_results, const ptg_vertex_reduction_parameters* vertex_reduction_parameters);

#ifdef __cplusplus
}
#endif

#endif
