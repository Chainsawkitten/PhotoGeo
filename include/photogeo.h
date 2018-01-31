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

/**
 * @brief Sum two numbers.
 * @param x First number.
 * @param y Second number.
 * @return The sum of the numbers.
 */
PHOTOGEO_API int ptg_add(int x, int y);

#ifdef __cplusplus
}
#endif

#endif
