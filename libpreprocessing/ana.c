/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Preprocessing - a library for image pre-processing.
 *
 * Copyright (C) 2016, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains implementation of functional analysis for integer 32
 * pre-processing operations.
 */

#include "preprocessing/ana.h"

#include "preprocessing/def.h"
#include "preprocessing/vmem.h"

/* from libeve */
#include "../libeve/eve/fixed_point.h"

/* from std c */
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PRIVATE INTERFACE *********************************************************/

/**
 * Perform median filtering on an image.
 *
 * @param src1  the pointer to start pixel of image.
 * @param rows1 the number of pixels of image in x dimension (rows).
 * @param cols1 the number of pixels of image in y dimension (columns).
 * @param src2  the pointer to start pixel of kernel.
 * @param rows2 the number of pixels of kernel in x dimension (rows).
 * @param cols2 the number of pixels of kernel in y dimension (columns).
 * @param dst   the pointer to start pixel of result image.
 *
 * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
 */
static int ana_median(const int32_t* src1, uint16_t rows1, uint16_t cols1,
        const int32_t* src2, uint16_t rows2, uint16_t cols2, int32_t* dst);

/**
 * Cross-correlate an image with a kernel. Edge handling is done by mirroring
 * at image border.
 *
 * @param src1  the pointer to start pixel of image.
 * @param rows1 the number of pixels of image in x dimension (rows).
 * @param cols1 the number of pixels of image in y dimension (columns).
 * @param src2  the pointer to start pixel of kernel.
 * @param rows2 the number of pixels of kernel in x dimension (rows).
 * @param cols2 the number of pixels of kernel in y dimension (columns).
 * @param dst   the pointer to start pixel of result image.
 *
 * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
 */
static int ana_crossCorrelateMirror(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst);

/**
 * Cross-correlate an image with a kernel. Edge handling is done by zero
 * padding.
 *
 * @param src1  the pointer to start pixel of image.
 * @param rows1 the number of pixels of image in x dimension (rows).
 * @param cols1 the number of pixels of image in y dimension (columns).
 * @param src2  the pointer to start pixel of kernel.
 * @param rows2 the number of pixels of kernel in x dimension (rows).
 * @param cols2 the number of pixels of kernel in y dimension (columns).
 * @param dst   the pointer to start pixel of result image.
 *
 * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
 */
static int ana_crossCorrelateZero(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst);

/**
 * Convolve an image with a kernel. Edge handling is done by mirroring at
 * image border.
 *
 * @param src1  the pointer to start pixel of image.
 * @param rows1 the number of pixels of image in x dimension (rows).
 * @param cols1 the number of pixels of image in y dimension (columns).
 * @param src2  the pointer to start pixel of kernel.
 * @param rows2 the number of pixels of kernel in x dimension (rows).
 * @param cols2 the number of pixels of kernel in y dimension (columns).
 * @param dst   the pointer to start pixel of result image.
 *
 * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
 */
static int ana_convolveMirror(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst);

/**
 * Convolve an image with a kernel. Edge handling is done by zero padding.
 *
 * @param src1  the pointer to start pixel of image.
 * @param rows1 the number of pixels of image in x dimension (rows).
 * @param cols1 the number of pixels of image in y dimension (columns).
 * @param src2  the pointer to start pixel of kernel.
 * @param rows2 the number of pixels of kernel in x dimension (rows).
 * @param cols2 the number of pixels of kernel in y dimension (columns).
 * @param dst   the pointer to start pixel of result image.
 *
 * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
 */
static int ana_convolveZero(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst);

/* PUBLIC IMPLEMENTATION *****************************************************/

int preprocessing_ana_underThresh(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        int32_t thresh, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            if (eve_fp_compare32(src + p, &thresh) == -1)
            {
                dst[p] = FP32_BINARY_TRUE;
            }
            else
            {
                dst[p] = 0;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_ana_equalThresh(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        int32_t thresh, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            if (eve_fp_compare32(src + p, &thresh) == 0)
            {
                dst[p] = FP32_BINARY_TRUE;
            }
            else
            {
                dst[p] = 0;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_ana_overThresh(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        int32_t thresh, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            if (eve_fp_compare32(src + p, &thresh) == 1)
            {
                dst[p] = FP32_BINARY_TRUE;
            }
            else
            {
                dst[p] = 0;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_ana_minImage(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst, int32_t* dstMin)
{
    int32_t min = EVE_FP32_MAX;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            if (eve_fp_compare32(src + p, &min) == -1)
            {
                min = src[p];
            }
        }
    }

    if (dstMin != 0)
    {
        *dstMin = min;
    }

    // Create mask of pixels containing the minimum value.
    return preprocessing_ana_equalThresh(sdSrc, rows, cols, min, sdDst);
}

/*****************************************************************************/

int preprocessing_ana_maxImage(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst, int32_t* dstMax)
{
    int32_t max = EVE_FP32_NAN;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            if (eve_fp_compare32(src + p, &max) == 1)
            {
                max = src[p];
            }
        }
    }


    if (dstMax != 0)
    {
        *dstMax = max;
    }
    
    // Create mask of pixels containing the minimum value.
    return preprocessing_ana_equalThresh(sdSrc, rows, cols, max, sdDst);
}

/*****************************************************************************/

int preprocessing_ana_deriveX(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    int32_t kernel[3] = { eve_fp_int2s32(-1, FP32_FWL), 0,
        eve_fp_int2s32(1, FP32_FWL) };

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    return ana_convolveMirror(src, rows, cols, &kernel[0], 3, 1, dst);
}

/*****************************************************************************/

int preprocessing_ana_deriveY(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    int32_t kernel[3] = { eve_fp_int2s32(-1, FP32_FWL), 0,
        eve_fp_int2s32(1, FP32_FWL) };

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    return ana_convolveMirror(src, rows, cols, &kernel[0], 1, 3, dst);
}

/*****************************************************************************/

int preprocessing_ana_createHistogram(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, unsigned int values, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    int32_t deltaValue = 0;

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, 2,
                    (uint16_t)(values))))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Check parameters.
    if (dst == 0)
    {
        printf("Invalid pointer: %p\n", dst);
        return PREPROCESSING_INVALID_ADDRESS;
    }

    if (values <= 1)
    {
        printf("Histogram values equal and lower than 1 are not allowed: "
                "%u.\n", values);
        return PREPROCESSING_INVALID_NUMBER;
    }

    // Create value range.
    deltaValue = eve_fp_divide32(EVE_FP32_MAX,
            eve_fp_int2s32((int)(values / 2), FP32_FWL), FP32_FWL);
    
    if (deltaValue == EVE_FP32_NAN)
    {
        return PREPROCESSING_INVALID_NUMBER;
    }
    
    dst[0] = eve_fp_subtract32(0, 
            eve_fp_multiply32(deltaValue,
                eve_fp_int2s32((int)(values / 2), FP32_FWL), FP32_FWL));

    if (dst[0] == EVE_FP32_NAN)
    {
        return PREPROCESSING_INVALID_NUMBER;
    }

    for (unsigned int n = 1; n < values; n++)
    {
        PREPROCESSING_DEF_CHECK_POINTER(dst, n, values)

        dst[n] = eve_fp_add32(dst[n -1], deltaValue);

        if (dst[n] == EVE_FP32_NAN)
        {
            return status;
        }
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)

            for (unsigned int n = 0; n < values; n++)
            {
                PREPROCESSING_DEF_CHECK_POINTER(dst, n + values, 2 * values)

                if (eve_fp_compare32(src + p, dst + n) == 1)
                {
                    continue;
                }

                dst[n + values]++;
                break;
            }
        }
    }

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_ana_crossCorrelate(uint32_t sdSrc1, uint16_t rows1,
        uint16_t cols1, uint32_t sdSrc2, uint16_t rows2, uint16_t cols2,
        uint32_t sdDst)
{
    const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    
    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows1, cols1))
            || (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows2,
                    cols2))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows1,
                    cols1)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Here we use mirroring.
    return ana_crossCorrelateMirror(src1, rows1, cols1, src2, rows2, cols2,
            dst);
}

/*****************************************************************************/

int preprocessing_ana_convolve(uint32_t sdSrc1, uint16_t rows1, uint16_t cols1,
        uint32_t sdSrc2, uint16_t rows2, uint16_t cols2, uint32_t sdDst)
{
    const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    
    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows1, cols1))
            || (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows2,
                    cols2))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows1,
                    cols1)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Here we use mirroring.
    return ana_convolveZero(src1, rows1, cols1, src2, rows2, cols2,
            dst);
}

/*****************************************************************************/

int preprocessing_ana_median(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    int32_t kernel[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    return ana_median(src, rows, cols, &kernel[0], 3, 3, dst);
}

/*****************************************************************************/

int preprocessing_ana_cast(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    float* dst = preprocessing_vmem_getDataAddress(sdDst);
    
    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            dst[p] = (float)(eve_fp_signed32ToDouble(src[p], FP32_FWL));
        }
    }

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_ana_invertMask(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            if (src[p] != 0)
            {
                dst[p] = 0;
            }
            else
            {
                dst[p] = 1;
            }
        }
    }

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_ana_cropImage(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint16_t rStart, uint16_t cStart, uint16_t rEnd, uint16_t cEnd,
        uint32_t sdDst)
{
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;
    unsigned int pDst = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst,
                    (uint16_t)(rEnd - rStart), (uint16_t)(cEnd - cStart))))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r = rStart; r < rEnd; r++)
    {
        for (unsigned int c = cStart; c < cEnd; c++)
        {
            p = r * cols + c;
            pDst = (r - rStart) * (unsigned int)(cEnd - cStart) + (c - cStart);

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, pDst,
                    (rEnd - rStart) * (cEnd - cStart))

            dst[pDst] = src[p];
        }
    }

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_ana_constructRowImage(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, unsigned int rowsNew, uint32_t sdDst)
{
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int sizeDst = (unsigned int)(rowsNew) * cols;
    unsigned int p = 0;
    unsigned int pDst = 0;
    unsigned long imgs = rowsNew / rows;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst,
                    (uint16_t)(rowsNew), cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int n = 0; n < imgs; n++)
    {
        for (unsigned int r = 0; r < rows; r++)
        {
            for (unsigned int c = 0; c < cols; c++)
            {
                p = r * cols + c;
                pDst = n * rows * cols + p;

                // Check for valid pointer position.
                PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
                PREPROCESSING_DEF_CHECK_POINTER(src, pDst, sizeDst)

                dst[pDst] = src[p];
            }
        }
    }

    return PREPROCESSING_SUCCESSFUL;
}

/* PRIVATE IMPLEMENTATION ****************************************************/

static int ana_median(const int32_t* src1, uint16_t rows1, uint16_t cols1,
        const int32_t* src2, uint16_t rows2, uint16_t cols2, int32_t* dst)
{
    unsigned int size1 = (unsigned int)(rows1) * cols1;
    unsigned int size2 = (unsigned int)(rows2) * cols2;
    unsigned int p1 = 0;
    unsigned int p2 = 0;

    // Adjust loop parameters for kernel.
    int dr = 0;
    int dc = 0;
    int r2Max = rows2 / 2 + 1;
    int r2Min = -(rows2 / 2);
    int c2Max = cols2 / 2 + 1;
    int c2Min = -(cols2 / 2);
    
    int32_t storage[25];
    unsigned int storageCount = 0;

    memset(storage, 0, sizeof(storage));

    // Check whether filter size exceeds reserved memory.
    if (sizeof(int32_t) * rows2 * cols2 > sizeof(storage))
    {
        printf("Median filter size %u exceeds reserved memory %u.\n",
                (unsigned int)(sizeof(int32_t) * rows2 * cols2),
                (unsigned int)(sizeof(storage)));
        return PREPROCESSING_INVALID_SIZE;
    }

    if ((rows2 != 0) && (rows2 % 2 == 0))
    {
        r2Min++;
    }

    if ((cols2 != 0) && (cols2 % 2 == 0))
    {
        c2Min++;
    }
    
    // Process.
    for (unsigned int r1 = 0; r1 < rows1; r1++)
    {
        for (unsigned int c1 = 0; c1 < cols1; c1++)
        {
            p1 = r1 * cols1 + c1;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src1, p1, size1)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p1, size1)

            storageCount = 0;

            for (int r2 = r2Min; r2 < r2Max; r2++)
            {
                for (int c2 = c2Min; c2 < c2Max; c2++)
                {
                    dr = r2 * cols1;
                    dc = c2;
                    p2 = (unsigned int)(r2 - r2Min) * cols2
                        + (unsigned int)(c2 - c2Min);

                    // Check for valid pointer position.
                    PREPROCESSING_DEF_CHECK_POINTER(src2, p2, size2)

                    // Consider non-zero kernel pixels only.
                    if (src2[p2] == 0)
                    {
                        continue;
                    }

                    // Check whether kernel exceeds image borders and apply
                    // zero padding by skipping.
                    if ((((int)(r1) * cols1 + dr) > (rows1 * cols1 - cols1))
                        || (((int)(r1) * cols1 + dr) < 0))
                    {
                        continue;
                    }

                    if ((((int)(c1) + dc) < 0) || (((int)(c1) + dc) >= cols1))
                    {
                        continue;
                    }

                    storage[storageCount] = src1[(int)(p1) + dr + dc];
                    storageCount++;
                }
            }

            // Sort median values.
            qsort(storage, storageCount, sizeof(int32_t), &eve_fp_compare32);

            // Choose median value differently depending on even or odd number
            // of elements.
            if ((storageCount % 2) == 0)
            {
                dst[p1] = eve_fp_add32(storage[(storageCount / 2) - 1],
                        + storage[storageCount / 2]);
                dst[p1] = eve_fp_divide32(dst[p1], eve_fp_int2s32(2, FP32_FWL),
                        FP32_FWL);
            }
            else
            {
                dst[p1] = storage[(storageCount / 2)];
            }
        }
    }

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

static int ana_crossCorrelateMirror(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size1 = (unsigned int)(rows1) * cols1;
    unsigned int size2 = (unsigned int)(rows2) * cols2;
    unsigned int p1 = 0;
    unsigned int p2 = 0;
    int32_t prod = 0;

    // Adjust loop parameters for kernel.
    int dr = 0;
    int dc = 0;
    int r2Max = rows2 / 2 + 1;
    int r2Min = -(rows2 / 2);
    int c2Max = cols2 / 2 + 1;
    int c2Min = -(cols2 / 2);

    if ((rows2 != 0) && (rows2 % 2 == 0))
    {
        r2Min++;
    }

    if ((cols2 != 0) && (cols2 % 2 == 0))
    {
        c2Min++;
    }
    
    // Process.
    for (unsigned int r1 = 0; r1 < rows1; r1++)
    {
        for (unsigned int c1 = 0; c1 < cols1; c1++)
        {
            p1 = r1 * cols1 + c1;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src1, p1, size1)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p1, size1)

            for (int r2 = r2Min; r2 < r2Max; r2++)
            {
                if ((dst[p1] == EVE_FP32_NAN)
                        && (status == PREPROCESSING_INVALID_NUMBER))
                {
                    break;
                }

                for (int c2 = c2Min; c2 < c2Max; c2++)
                {
                    dr = r2 * cols1;
                    dc = c2;
                    p2 = (unsigned int)(r2 - r2Min) * cols2
                            + (unsigned int)(c2 - c2Min);

                    // Check for valid pointer position.
                    PREPROCESSING_DEF_CHECK_POINTER(src2, p2, size2)

                    // Check whether kernel exceeds image borders and apply
                    // mirror padding.
                    if ((((int)(r1) * cols1 + dr) > (rows1 * cols1 - cols1))
                        || (((int)(r1) * cols1 + dr) < 0))
                    {
                        dr = 0 - dr;
                    }

                    if ((((int)(c1) + dc) < 0) || (((int)(c1) + dc) >= cols1))
                    {
                        dc = 0 - dc;
                    }

                    prod = eve_fp_multiply32(src1[(int)(p1) + dr + dc],
                            src2[p2], FP32_FWL);

                    if (prod == EVE_FP32_NAN)
                    {
                        dst[p1] = EVE_FP32_NAN;
                        status = PREPROCESSING_INVALID_NUMBER;
                        break;
                    }

                    dst[p1] = eve_fp_add32(dst[p1], prod);
                }
            }
        }
    }

    return status;
}

/*****************************************************************************/
__attribute__((unused))
static int ana_crossCorrelateZero(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size1 = (unsigned int)(rows1) * cols1;
    unsigned int size2 = (unsigned int)(rows2) * cols2;
    unsigned int p1 = 0;
    unsigned int p2 = 0;
    int32_t prod = 0;

    // Adjust loop parameters for kernel.
    int dr = 0;
    int dc = 0;
    int r2Max = rows2 / 2 + 1;
    int r2Min = -(rows2 / 2);
    int c2Max = cols2 / 2 + 1;
    int c2Min = -(cols2 / 2);

    if ((rows2 != 0) && (rows2 % 2 == 0))
    {
        r2Min++;
    }

    if ((cols2 != 0) && (cols2 % 2 == 0))
    {
        c2Min++;
    }
    
    // Process.
    for (unsigned int r1 = 0; r1 < rows1; r1++)
    {
        for (unsigned int c1 = 0; c1 < cols1; c1++)
        {
            p1 = r1 * cols1 + c1;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src1, p1, size1)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p1, size1)

            for (int r2 = r2Min; r2 < r2Max; r2++)
            {
                if ((dst[p1] == EVE_FP32_NAN)
                        && (status == PREPROCESSING_INVALID_NUMBER))
                {
                    break;
                }

                for (int c2 = c2Min; c2 < c2Max; c2++)
                {
                    dr = r2 * cols1;
                    dc = c2;
                    p2 = (unsigned int)(r2 - r2Min) * cols2
                            + (unsigned int)(c2 - c2Min);

                    // Check for valid pointer position.
                    PREPROCESSING_DEF_CHECK_POINTER(src2, p2, size2)

                    // Check whether kernel exceeds image borders and apply
                    // zero padding by skipping.
                    if ((((int)(r1) * cols1 + dr) > (rows1 * cols1 - cols1))
                            || (((int)(r1) * cols1 + dr) < 0))
                    {
                        continue;
                    }

                    if ((((int)(c1) + dc) < 0) || (((int)(c1) + dc) >= cols1))
                    {
                        continue;
                    }

                    prod = eve_fp_multiply32(src1[(int)(p1) + dr + dc],
                            src2[p2], FP32_FWL);

                    if (prod == EVE_FP32_NAN)
                    {
                        dst[p1] = EVE_FP32_NAN;
                        status = PREPROCESSING_INVALID_NUMBER;
                        break;
                    }

                    dst[p1] = eve_fp_add32(dst[p1], prod);
                }
            }
        }
    }

    return status;
}

/*****************************************************************************/

static int ana_convolveMirror(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size1 = (unsigned int)(rows1) * cols1;
    unsigned int size2 = (unsigned int)(rows2) * cols2;
    unsigned int p1 = 0;
    unsigned int p2 = 0;
    int32_t prod = 0;

    // Adjust loop parameters for kernel.
    int dr = 0;
    int dc = 0;
    int r2Max = rows2 / 2 + 1;
    int r2Min = -(rows2 / 2);
    int c2Max = cols2 / 2 + 1;
    int c2Min = -(cols2 / 2);

    if ((rows2 != 0) && (rows2 % 2 == 0))
    {
        r2Min++;
    }

    if ((cols2 != 0) && (cols2 % 2 == 0))
    {
        c2Min++;
    }
    
    // Process.
    for (unsigned int r1 = 0; r1 < rows1; r1++)
    {
        for (unsigned int c1 = 0; c1 < cols1; c1++)
        {
            p1 = r1 * cols1 + c1;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src1, p1, size1)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p1, size1)

            for (int r2 = r2Min; r2 < r2Max; r2++)
            {
                if ((dst[p1] == EVE_FP32_NAN)
                        && (status == PREPROCESSING_INVALID_NUMBER))
                {
                    break;
                }

                for (int c2 = c2Min; c2 < c2Max; c2++)
                {
                    dr = r2 * cols1;
                    dc = c2;
                    
                    // Consider kernel rotation by 180 degrees.
                    p2 = (unsigned int)((rows2 * cols2 - 1)
                            - (r2 - r2Min) * cols2 - (c2 - c2Min));

                    // Check for valid pointer position.
                    PREPROCESSING_DEF_CHECK_POINTER(src2, p2, size2)

                    // Check whether kernel exceeds image borders and apply
                    // mirror padding.
                    if ((((int)(r1) * cols1 + dr) > (rows1 * cols1 - cols1))
                            || (((int)(r1) * cols1 + dr) < 0))
                    {
                        dr = 0 - dr;
                    }

                    if ((((int)(c1) + dc) < 0) || (((int)(c1) + dc) >= cols1))
                    {
                        dc = 0 - dc;
                    }

                    prod = eve_fp_multiply32(src1[(int)(p1) + dr + dc],
                            src2[p2], FP32_FWL);

                    if (prod == EVE_FP32_NAN)
                    {
                        dst[p1] = EVE_FP32_NAN;
                        status = PREPROCESSING_INVALID_NUMBER;
                        break;
                    }

                    dst[p1] = eve_fp_add32(dst[p1], prod);
                }
            }
        }
    }

    return status;
}

/*****************************************************************************/

static int ana_convolveZero(const int32_t* src1, uint16_t rows1,
        uint16_t cols1, const int32_t* src2, uint16_t rows2, uint16_t cols2,
        int32_t* dst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size1 = (unsigned int)(rows1) * cols1;
    unsigned int size2 = (unsigned int)(rows2) * cols2;
    unsigned int p1 = 0;
    unsigned int p2 = 0;
    int32_t prod = 0;

    // Adjust loop parameters for kernel.
    int dr = 0;
    int dc = 0;
    int r2Max = rows2 / 2 + 1;
    int r2Min = -(rows2 / 2);
    int c2Max = cols2 / 2 + 1;
    int c2Min = -(cols2 / 2);

    if ((rows2 != 0) && (rows2 % 2 == 0))
    {
        r2Min++;
    }

    if ((cols2 != 0) && (cols2 % 2 == 0))
    {
        c2Min++;
    }
    
    // Process.
    for (unsigned int r1 = 0; r1 < rows1; r1++)
    {
        for (unsigned int c1 = 0; c1 < cols1; c1++)
        {
            p1 = r1 * cols1 + c1;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src1, p1, size1)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p1, size1)

            for (int r2 = r2Min; r2 < r2Max; r2++)
            {
                if ((dst[p1] == EVE_FP32_NAN)
                        && (status == PREPROCESSING_INVALID_NUMBER))
                {
                    break;
                }

                for (int c2 = c2Min; c2 < c2Max; c2++)
                {
                    dr = r2 * cols1;
                    dc = c2;
                    // Consider kernel rotation by 180 degrees.
                    p2 = (unsigned int)((rows2 * cols2 - 1)
                            - (r2 - r2Min) * cols2 - (c2 - c2Min));

                    // Check for valid pointer position.
                    PREPROCESSING_DEF_CHECK_POINTER(src2, p2, size2)

                    // Check whether kernel exceeds image borders and apply
                    // zero padding by skipping.
                    if ((((int)(r1) * cols1 + dr) > (rows1 * cols1 - cols1))
                            || (((int)(r1) * cols1 + dr) < 0))
                    {
                        continue;
                    }

                    if ((((int)(c1) + dc) < 0) || (((int)(c1) + dc) >= cols1))
                    {
                        continue;
                    }

                    prod = eve_fp_multiply32(src1[(int)(p1) + dr + dc],
                            src2[p2], FP32_FWL);

                    if (prod == EVE_FP32_NAN)
                    {
                        dst[p1] = EVE_FP32_NAN;
                        status = PREPROCESSING_INVALID_NUMBER;
                        break;
                    }

                    dst[p1] = eve_fp_add32(dst[p1], prod);
                }
            }
        }
    }

    return status;
}
