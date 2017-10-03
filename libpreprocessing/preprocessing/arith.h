/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Preprocessing - a library for image pre-processing.
 *
 * Copyright (C) 2016, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains declarations of arithmetic for 24.8 fixed point
 * pre-processing operations.
 */

#ifndef PREPROCESSING_ARITH_H
#define PREPROCESSING_ARITH_H

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Add two images pixel by pixel.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of image 1.
     * @param sdSrc2 the VMEM (SDRAM) address of image 2.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_addImages(uint32_t sdSrc1, uint32_t sdSrc2,
            uint16_t rows, uint16_t cols, uint32_t sdDst);

    /**
     * Subtract two images pixel by pixel.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of image 1.
     * @param sdSrc2 the VMEM (SDRAM) address of image 2.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_subtractImages(uint32_t sdSrc1, uint32_t sdSrc2,
            uint16_t rows, uint16_t cols, uint32_t sdDst);

    /**
     * Multiply two images pixel by pixel.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of image 1.
     * @param sdSrc2 the VMEM (SDRAM) address of image 2.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_multiplyImages(uint32_t sdSrc1, uint32_t sdSrc2,
            uint16_t rows, uint16_t cols, uint32_t sdDst);

    /**
     * Divide two images pixel by pixel.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of image 1.
     * @param sdSrc2 the VMEM (SDRAM) address of image 2.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_divideImages(uint32_t sdSrc1, uint32_t sdSrc2,
            uint16_t rows, uint16_t cols, uint32_t sdDst);

    /**
     * Add a scalar to an image.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param scalar the value to add.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_addScalar(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, int32_t scalar, uint32_t sdDst);

    /**
     * Subtract a scalar from an image.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param scalar the value to add.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_subtractScalar(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, int32_t scalar, uint32_t sdDst);

    /**
     * Multiply an image by a scalar.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param scalar the value to add.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_multiplyScalar(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, int32_t scalar, uint32_t sdDst);

    /**
     * Divide an image by a scalar.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param scalar the value to add.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_divideScalar(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, int32_t scalar, uint32_t sdDst);

    /**
     * Calculate the mean value of all pixels of an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_meanImage(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Calculate the sum of all pixels of an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_sumImage(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Calculate the mean value of all pixels in each column of an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_meanColumns(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Calculate the sum of all pixels in each column of an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_sumColumns(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Calculate the root mean square of an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_rootMeanSquare(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Calculate the square root of all pixels of an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_squareRootImage(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Calculate the logarithm of all pixels with base 10 of an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_logarithm10Image(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Multiply two matrices.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of matrix 1.
     * @param rows1  the number of matrix 1 rows.
     * @param cols1  the number of matrix 1 columns.
     * @param sdSrc2 the VMEM (SDRAM) address of matrix 2.
     * @param rows2  the number of matrix 2 rows.
     * @param cols2  the number of matrix 2 columns.
     * @param sdDst  the VMEM (SDRAM) address of result matrix.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_arith_multiplyMatrices(uint32_t sdSrc1, uint16_t rows1,
            uint16_t cols1, uint32_t sdSrc2, uint16_t rows2, uint16_t cols2,
            uint32_t sdDst);

#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSING_ARITH_H */
