/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Prep - a library for image pre-processing.
 *
 * Copyright (C) 2016, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains implementation of arithmetic for integer 32
 * pre-processing operations.
 */

#include "preprocessing/arith.h"

#include"preprocessing/def.h"
#include "preprocessing/vmem.h"

/* from libeve */
#include "../libeve/eve/fixed_point.h"

/* from std c */
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PUBLIC IMPLEMENTATION *****************************************************/

int preprocessing_arith_addImages(uint32_t sdSrc1, uint32_t sdSrc2,
        uint16_t rows, uint16_t cols, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
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
            PREPROCESSING_DEF_CHECK_POINTER(src1, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(src2, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            dst[p] = eve_fp_add32(src1[p], src2[p]);

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_subtractImages(uint32_t sdSrc1, uint32_t sdSrc2,
        uint16_t rows, uint16_t cols, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
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
            PREPROCESSING_DEF_CHECK_POINTER(src1, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(src2, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            dst[p] = eve_fp_subtract32(src1[p], src2[p]);

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_multiplyImages(uint32_t sdSrc1, uint32_t sdSrc2,
        uint16_t rows, uint16_t cols, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
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
            PREPROCESSING_DEF_CHECK_POINTER(src1, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(src2, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            dst[p] = eve_fp_multiply32(src1[p], src2[p], FP32_FWL);

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_divideImages(uint32_t sdSrc1, uint32_t sdSrc2,
        uint16_t rows, uint16_t cols, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
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
            PREPROCESSING_DEF_CHECK_POINTER(src1, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(src2, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

            dst[p] = eve_fp_divide32(src1[p], src2[p], FP32_FWL);

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_addScalar(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        int32_t scalar, uint32_t sdDst)
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

            dst[p] = eve_fp_add32(src[p], scalar);



            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_subtractScalar(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, int32_t scalar, uint32_t sdDst)
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

            dst[p] = eve_fp_subtract32(src[p], scalar);

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_multiplyScalar(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, int32_t scalar, uint32_t sdDst)
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

            dst[p] = eve_fp_multiply32(src[p], scalar, FP32_FWL);

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_divideScalar(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, int32_t scalar, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    if (scalar == 0)
    {
        printf("Divison by 0 is not allowed.\n");
        return PREPROCESSING_INVALID_NUMBER;
    }

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

            dst[p] = eve_fp_divide32(src[p], scalar, FP32_FWL);

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_meanImage(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    
    if (dst == 0)
    {
        return PREPROCESSING_INVALID_ADDRESS;
    }
    
    status = preprocessing_arith_sumImage(sdSrc, rows, cols, sdDst);

    if (status != PREPROCESSING_SUCCESSFUL)
    {
        return status;
    }

    dst[0] = eve_fp_divide32(dst[0],
            eve_fp_int2s32((int)(rows) * cols, FP32_FWL), FP32_FWL);

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_arith_sumImage(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;
 
    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if (!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Refuse null pointer.
    if (dst == 0)
    {
        return PREPROCESSING_INVALID_ADDRESS;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)

            dst[0] = eve_fp_add32(dst[0], src[p]);
           // printf("numero  = %d\n",  dst[0]);
            //printf("destino y[%d] = %f\n", p, eve_fp_signed32ToDouble(dst[0], FP32_FWL));

            if (dst[0] == EVE_FP32_NAN)

            {

                return PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_meanColumns(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, uint32_t sdDst)
{
    int status = 0;

    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    if (dst == 0)
    {
        return PREPROCESSING_INVALID_ADDRESS;
    }

    status = preprocessing_arith_sumColumns(sdSrc, rows, cols, sdDst);

    if (status != PREPROCESSING_SUCCESSFUL)
    {
        return status;
    }

    for (uint16_t c = 0; c < cols; c++)
    {
        dst[c] = eve_fp_divide32(dst[c], eve_fp_int2s32((int)(cols), FP32_FWL),
                FP32_FWL);
    }

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_arith_sumColumns(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    
    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, 1, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int c = 0; c < cols; c++)
    {
        for (unsigned int r = 0; r < rows; r++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            PREPROCESSING_DEF_CHECK_POINTER(dst, c, cols)

            dst[c] = eve_fp_add32(dst[c], src[p]);

            if (dst[c] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
                break;
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_rootMeanSquare(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, uint32_t sdDst)
{
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;
    int32_t square = 0;
 
    const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if (!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Refuse null pointer.
    if (dst == 0)
    {
        return PREPROCESSING_INVALID_ADDRESS;
    }

    // Process.
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            p = r * cols + c;

            // Check for valid pointer position.
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size)
            
            square = eve_fp_multiply32(src[p], src[p], FP32_FWL);

            if (square == EVE_FP32_NAN)
            {
                dst[0] = EVE_FP32_NAN;
                return PREPROCESSING_INVALID_NUMBER;
            }

            dst[0] = eve_fp_add32(dst[0], square);

            if (dst[0] == EVE_FP32_NAN)
            {
                return PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    dst[0] = eve_fp_divide32(dst[0], eve_fp_int2s32((int)(size), FP32_FWL),
            FP32_FWL);
    dst[0] = eve_fp_double2s32(
            sqrt(eve_fp_signed32ToDouble(dst[0], FP32_FWL)), FP32_FWL);

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_arith_squareRootImage(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, uint32_t sdDst)
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

            // Note: Here we use real numbers and so the square root is defined
            //       for positive numbers only. For negative numbers we can use
            //       the complex square root csqrt in complex.h (C99).
            if (src[p] < 0)
            {
                dst[p] = EVE_FP32_NAN;
                status = PREPROCESSING_INVALID_NUMBER;
            }
            else
            {
                dst[p] = eve_fp_double2s32(
                        sqrt(eve_fp_signed32ToDouble(src[p], FP32_FWL)),
                        FP32_FWL);
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_logarithm10Image(uint32_t sdSrc, uint16_t rows,
        uint16_t cols, uint32_t sdDst)
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

            // Note: Here we use real numbers and so the logarithm is defined
            //       for positive numbers only. For negative numbers we can use
            //       the complex logarithm clog10 in complex.h (C99).
            if (src[p] <= 0)
            {
                dst[p] = EVE_FP32_NAN;
                status = PREPROCESSING_INVALID_NUMBER;
            }
            else
            {
                dst[p] = eve_fp_double2s32(
                        log10(eve_fp_signed32ToDouble(src[p], FP32_FWL)),
                        FP32_FWL);
            }
        }
    }

    return status;
}

/*****************************************************************************/

int preprocessing_arith_multiplyMatrices(uint32_t sdSrc1, uint16_t rows1,
        uint16_t cols1, uint32_t sdSrc2, uint16_t rows2, uint16_t cols2,
        uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size1 = (unsigned int)(rows1) * cols1;
    unsigned int size2 = (unsigned int)(rows2) * cols2;
    unsigned int sizeDst = (unsigned int)(rows1) * cols2;
    unsigned int p1 = 0;
    unsigned int p2 = 0;
    unsigned int pDst = 0;
    int32_t prod = 0;

    const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);
    
    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows1, cols1))
            || (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows2,
                    cols2))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows1,
                    cols2)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    // Process.
    for (unsigned int r1 = 0; r1 < rows1; r1++)
    {
        for (unsigned int c1 = 0; c1 < cols2; c1++)
        {
            pDst = r1 * cols2 + c1;

            PREPROCESSING_DEF_CHECK_POINTER(dst, pDst, sizeDst)

            if (dst[pDst] == EVE_FP32_NAN)
            {
                break;
            }

            for (unsigned int r2 = 0; r2 < rows2; r2++)
            {
                p1 = r1 * cols1 + r2;
                p2 = r2 * cols2 + c1;

                PREPROCESSING_DEF_CHECK_POINTER(src1, p1, size1)
                PREPROCESSING_DEF_CHECK_POINTER(src2, p2, size2)

                prod = eve_fp_multiply32(src1[p1], src2[p2], FP32_FWL);

                if (prod == EVE_FP32_NAN)
                {
                    status = PREPROCESSING_INVALID_NUMBER;
                    dst[pDst] = EVE_FP32_NAN;
                    break;
                }

                dst[pDst] = eve_fp_add32(dst[pDst], prod);

                if (dst[pDst] == EVE_FP32_NAN)
                {
                    status = PREPROCESSING_INVALID_NUMBER;
                    break;
                }
            }
        }
    }

    return status;
}
