//----------------------------------------------------------------------------
//   The confidential and proprietary information contained in this file may
//   only be used by a person authorised under and to the extent permitted
//   by a subsisting licensing agreement from ARM Limited or its affiliates.
//
//          (C) COPYRIGHT [2020] ARM Limited or its affiliates.
//              ALL RIGHTS RESERVED
//
//   This entire notice must be reproduced on all copies of this file
//   and copies of this file may only be made by a person if such person is
//   permitted to do so under the terms of a subsisting license agreement
//   from ARM Limited or its affiliates.
//----------------------------------------------------------------------------
#ifndef __GDC_H__
#define __GDC_H__
// ---------------------------------------------------------------------------------
#ifdef _WIN32
    #ifdef libgdc_EXPORTS
        #define ARM_GDC_API     __declspec(dllexport)
    #else
        #define ARM_GDC_API     __declspec(dllimport)
    #endif
    #define LIBINIT
    #define LIBCLOSE
#else
    #define ARM_GDC_API     __attribute__ ((visibility("default")))
    #define LIBINIT         __attribute__ ((constructor))
    #define LIBCLOSE        __attribute__ ((destructor))
#endif
//---------------------------------------------------------
#include <stdint.h>
//---------------------------------------------------------
/**
 * Frame format
 */
typedef enum {
    FMT_UNKNOWN,
    FMT_LUMINANCE,
    FMT_PLANAR_444,
    FMT_PLANAR_420,
    FMT_SEMIPLANAR_420
} frame_format_t;
//---------------------------------------------------------
/**
 * Rectangle definition
 */
typedef struct {
    int32_t x; ///< Start x coordinate
    int32_t y; ///< Start y coordinate
    int32_t w; ///< width
    int32_t h; ///< height
} rect_t;
//---------------------------------------------------------
/**
 * Types of supported transformations
 */
typedef enum {
    PANORAMIC,
    CYLINDRICAL,
    STEREOGRAPHIC,
    UNIVERSAL,
    CUSTOM,
    AFFINE,
    DEWARP_KEYSTONE
} transformation_t;
//---------------------------------------------------------
/**
 * ECC binary generation
 */
typedef enum {
    ECC_DISABLED,  // ECC configuration is disabled
    ECC_ENABLED,   // ONLY ECC configuration will be generated
    ECC_EMBEDDED   // ECC configuration is ebmedded in the transformation configuration
} ecc_gen_t;
//---------------------------------------------------------
/**
 * Resolution definition
 */
typedef struct {
    uint32_t w;  ///< width in pixels
    uint32_t h;  ///< height in pixels
} resolution_t;
//---------------------------------------------------------
/**
 * Grid point
 */
typedef struct {
    double x;  ///< x coordinate
    double y;  ///< y coordinate
} point_t;

//---------------------------------------------------------
/**
 * Custom transformation structure
 */
typedef struct {
    uint8_t full_tile_calc;
    uint16_t tile_incr_x;
    uint16_t tile_incr_y;
    int32_t w;          ///< Number or points in horizontal direction in the custom transformation grid
    int32_t h;          ///< Number or points in vertical direction in the custom transformation grid
    double centerx;     ///< center along x axis
    double centery;     ///< center along y axis
    point_t* points;    ///< array with points, number of elements = w*h
} custom_tranformation_t;
//---------------------------------------------------------
/**
 * Window definition and transformation parameters
 * For parameters meaning read GDC guide
 */
typedef struct {
    rect_t out_r;                   ///< Output window position and size
    transformation_t transform;     ///< Used transformation
    rect_t input_roi_r;
    int32_t pan;                    ///< Target shift in horizontal direction from centre of the output image in pixels
    int32_t tilt;                   ///< Target shift in vertical direction from centre of the output image in pixels
    double zoom;                    ///< Target zoom dimensionless coefficient (must not be bigger than zero)
    double strength;                ///< Dimensionless non-negative parameter defining the strength of transformation along X axis
    double strengthY;               ///< Dimensionless non-negative parameter defining the strength of transformation along X axis
    double angle;                   ///< Angle of main projection axis rotation around itself in degrees
    // universal transformation
    double elevation;               ///< Angle in degrees which specify the main projection axis
    double azimuth;                 ///< Angle in degrees which specify the main projection axis, counted clockwise from North direction (positive to East)
    int32_t keep_ratio;             ///< Keep the same stretching strength in both horizontal and vertical directions
    double FOV_h;                   ///< Size of output field of view in vertical dimension in degrees
    double FOV_w;                   ///< Size of output field of view in horizontal dimension in degrees
    double cylindricity_y;          ///< Level of cylindricity for target projection shape in vertical direction
    double cylindricity_x;          ///< Level of cylindricity for target projection shape in horizontal direction
    char custom_file[128];          ///< File name of the file containing custom transformation description
    custom_tranformation_t custom;  ///< Parsed custom transformation structure
    double trapezoid_left_angle;    ///< Left Acute angle in degrees between trapezoid base and leg
    double trapezoid_right_angle;   ///< Right Acute angle in degrees between trapezoid base and leg
    uint8_t check_compute;          ///< Perform fixed point and floating point result comparisons
} window_t;
//---------------------------------------------------------
/**
 * Common parameters
 */
typedef struct {
    frame_format_t format;  // frame format
    resolution_t in;        // input frame resolution
    resolution_t out;       // output frame resolution
    int32_t x_offset;       // center offset for input x coordinate
    int32_t y_offset;       // center offset for input y coordinate
    int32_t diameter;       // diameter of equivalent 180 field of view in pixels
    double fov;             // field of view
} param_t;
//---------------------------------------------------------
/**
 * Grid parameters
 */
typedef struct {
    int32_t  step_x;        // step in pixels for X coordinate
    int32_t  step_y;        // step in pixels for Y coordinate
    uint64_t array_length;  // length of allocated memory for array to fill points
    point_t* array;         // array with points
} grid_t;
//---------------------------------------------------------
/**
 * GDC context
 */
struct gdc_t;
typedef struct gdc_t gdc_t;
//---------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------
/**
 * Initialize GDC instance.
 * @return gdc instance pointer
 */
ARM_GDC_API gdc_t* gdc_init();
//---------------------------------------------------------
/**
 * Calculate GDC transformation
 * @param[in] gdc      instance pointer
 * @param[in] param    pointer to structure with common parameters
 * @param[in] wnd      array of windows parameters
 * @param[in] wnd_num  number of windows
 * @param[out] buffer   memory where to store output binary sequence
 * @param[in] fill_empty    if 1 fill empty space between windows
 * @param[in] fill_colours  array of color values to fill empty space
 * @param[in] ecc  enable or disable ecc config generation
 * @param[in] print_tiles   print resulting tiles in stdout
 * @param[in] reuse_solver   attempt to reuse solver
 * @return size of binary sequence in bytes stored in *buffer
 */
ARM_GDC_API uint32_t gdc_calculate(gdc_t* gdc, const param_t* param, const window_t* wnd, uint32_t wnd_num, uint32_t** buffer, int32_t fill_empty, const uint16_t fill_colours[3], ecc_gen_t ecc, int32_t print_tiles, uint8_t reuse_solver);
//---------------------------------------------------------
/** 
 * Cleanup all internals after processing
 * @param[in] gdc  instance pointer
 */
ARM_GDC_API void gdc_cleanup(gdc_t* gdc);
//---------------------------------------------------------
/**
 * Release binary sequence buffer
 * @param[in] gdc     instance pointer
 * @param[in] buffer  memory where output binary sequence was stored by previous call of gdc_calculate
 */
ARM_GDC_API void gdc_release_buffer(gdc_t* gdc, uint32_t* buffer);
//---------------------------------------------------------
/**
 * Calculate grid based on given transformation parameters
 * @param[in] gdc           instance pointer
 * @param[in] param         pointer to structure with common parameters
 * @param[in] wnd           array of windows parameters
 * @param[in] wnd_num       number of windows
 * @param[in] reuse_solver  attempt to reuse solver
 * @param[out] grids        structure where to store the grid
 * @return 0 if success
 */
ARM_GDC_API int32_t gdc_calculate_grid(gdc_t* gdc, const param_t* param, const window_t* wnd, uint32_t wnd_num, uint8_t reuse_solver, grid_t* grids);
//---------------------------------------------------------
/**
 * Parse json layout file to parameters and windows structures
 * @param[in]  buf      array with input json text
 * @param[out] param    common structure
 * @param[out] wnds     array of windows
 * @param[out] wnd_cnt  output number of windows
 * @return  0 if success
 */
ARM_GDC_API int32_t gdc_parse_json(const char* buf, param_t* param, window_t** wnds, uint32_t* wnd_cnt);
//---------------------------------------------------------
/**
 * Clean structures after usage
 * @param wnds[in]     array of windows
 * @param wnd_num[in]  output number of windows
 */
ARM_GDC_API void gdc_parse_json_clean(window_t** wnds, uint32_t wnd_num);
//---------------------------------------------------------
/**
 * Set custom points for a window. Custom points are not copied
 * @param window[in]         window to set custom points to
 * @param custom_params[in]  custom points to be set; will be used throughout transformation
 */
ARM_GDC_API void gdc_set_custom_points(window_t* window, const custom_tranformation_t *custom_params);
//---------------------------------------------------------
/**
 * Copy custom points to a window
 * @param window[in]         window to set custom points to
 * @param custom_params[in]  custom points to be set; copied and safe to release the memory on function return
 */
ARM_GDC_API void gdc_set_custom_points_copy(window_t* window, const custom_tranformation_t *custom_params);
//---------------------------------------------------------
#ifdef __cplusplus
}
#endif
//---------------------------------------------------------
#endif //__GDC_H__
