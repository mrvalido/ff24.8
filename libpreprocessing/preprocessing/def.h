/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Preprocessing - a library for image pre-processing.
 *
 * Copyright (C) 2016, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains basic declarations and definitions used all over
 * libpreprocessing.
 */

#ifndef PREPROCESSING_DEF_H
#define PREPROCESSING_DEF_H

/**
 * This is the word length of the fractional part of the 24.8 fixed point
 * representation.
 */
#define FP32_FWL 8

/**
 * This is the bit mask of the fractional part of the 24.8 fixed point
 * representation.
 */
#define FP32_FWL_MASK 0xFF

/**
 * This is a bit mask to extract the sign bit of the 24.8 fixed point
 * representation.
 */
#define FP32_SIGN (1 << 31)

/**
 * This is a bit mask to extract the absolute value of the 24.8 fixed point
 * representation.
 */
#define FP32_ABS_MASK 0x7FFFFFFF

/**
 * This value represents a true pixel in a 24.8 fixed point binary mask. It is
 * set to 1.00 so a simple multiplication by a mask can be used to extract the
 * true pixels from an image.
 */
#define FP32_BINARY_TRUE (1 << FP32_FWL)

/**
 * This is the value of pi.
 */
#define PI 3.14159265358979323846

/**
 * These macros are used to convert angle values from degree to radian and
 * vice versa.
 * @{
 */
#define RAD(deg) (double)((double)(deg) * PI / 180.0)
#define DEG(rad) (double)((double)(rad) * 180.0 / PI)
/**
 * @}
 */

/**
 * This macro is used to check the range of a pointer at a certain position.
 */
#define PREPROCESSING_DEF_CHECK_POINTER(p, n, size) \
    if ((p == 0) || ((p + n) >= (p + size)) || ((p + n) < p)) \
    { \
        printf("Invalid data pointer: %p.\n", p + n); \
        return PREPROCESSING_INVALID_ADDRESS; \
    }

/**
 * These are the reserved return values of the operation functions.
 */
enum preprocessing_Status
{
    PREPROCESSING_SUCCESSFUL = 0,
    PREPROCESSING_INVALID_ADDRESS,
    PREPROCESSING_INVALID_NUMBER,
    PREPROCESSING_INVALID_SIZE,
    PREPROCESSING_NO_MEMORY
};

#endif /* PREPROCESSING_DEF_H */
