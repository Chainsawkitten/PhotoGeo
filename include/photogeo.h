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

/// Input parameters to ptg_generate_collision_geometry.
struct ptg_source_parameters {
    /// Source image in 3-channel RGB format.
    unsigned char* image;

    /// The width of the source image.
    unsigned int width;

    /// The height of the source image.
    unsigned int height;

    /// The number of background colors.
    unsigned int background_color_count;

    /// The background colors.
    ptg_color* background_colors;

    /// The number of color layers.
    unsigned int color_layer_count;

    /// The colors of the different color layers.
    ptg_color* color_layer_colors;

    /// Flags.
    unsigned int flags;
};

/**
 * Generate collision geometry from image.
 * @param parameters Input parameters.
 * @param out_vertex_count Variable to store resulting vertex counts.
 * @param out_vertices Variable to store resulting vertices.
 * @todo Implement this. Currently does nothing.
 */
PHOTOGEO_API void ptg_generate_collision_geometry(const ptg_source_parameters* parameters, unsigned int** out_vertex_count, ptg_vec2*** out_vertices);

/**
 * Deallocate the memory that was allocated to store the results.
 * @param vertex_count Vertex count.
 * @param vertices Vertices.
 */
PHOTOGEO_API void ptg_free_results(unsigned int* vertex_count, ptg_vec2** vertices);

#ifdef __cplusplus
}
#endif

#endif
