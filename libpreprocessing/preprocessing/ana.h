/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Preprocessing - a library for image pre-processing.
 *
 * Copyright (C) 2016, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains declarations of functional analysis for integer 32
 * pre-processing operations.
 */

#ifndef PREPROCESSING_ANA_H
#define PREPROCESSING_ANA_H

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Mark all pixels of an image whose values lie under a certain threshold
     * in a mask.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param thresh the threshold to check pixel values against.
     * @param sdDst  the VMEM (SDRAM) address of result image mask.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_underThresh(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, int32_t thresh, uint32_t sdDst);

    /**
     * Mark all pixels of an image whose values are equal to a certain
     * threshold in a mask.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param thresh the threshold to check pixel values against.
     * @param sdDst  the VMEM (SDRAM) address of result image mask.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_equalThresh(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, int32_t thresh, uint32_t sdDst);

    /**
     * Mark all pixels of an image whose values lie over a certain threshold
     * in a mask.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param thresh the threshold to check pixel values against.
     * @param sdDst  the VMEM (SDRAM) address of result image mask.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_overThresh(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, int32_t thresh, uint32_t sdDst);

    /**
     * Find the minimum in all pixels of an image and mark these pixels in an
     * image mask. We use /a preprocessing_ana_equalThresh to create the image
     * mask.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param sdDst  the VMEM (SDRAM) address of result image mask.
     * @param dstMin the pointer to the storage destination of the minimum.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_minImage(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst, int32_t* dstMin);

    /**
     * Find the maximum in all pixels of an image and mark these pixels in an
     * image mask. We use /a preprocessing_ana_equalThreshFP to create the
     * image mask.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param sdDst  the VMEM (SDRAM) address of result image mask.
     * @param dstMax the pointer to the storage destination of the maximum.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_maxImage(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint32_t sdDst, int32_t* dstMax);

    /**
     * Calculate the derivation of the image in x direction.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_deriveX(uint32_t sdSrc, uint16_t rows, uint16_t cols,
            uint32_t sdDst);

    /**
     * Calculate the derivation of the image in y direction.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_deriveY(uint32_t sdSrc, uint16_t rows, uint16_t cols,
            uint32_t sdDst);

    /**
     * Create a histrogram of all pixel values of an image. The number of
     * histogram values (or resolution) can be specified with a parameter.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param values the number of values for histogram resolution.
     * @param sdDst  the VMEM (SDRAM) address of result histogram.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_createHistogram(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, unsigned int values, uint32_t sdDst);

    /**
     * Cross-correlate an image with a kernel.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of image 1.
     * @param rows1  the number of image 1 rows.
     * @param cols1  the number of image 1 columns.
     * @param sdSrc2 the VMEM (SDRAM) address of image 2 or kernel.
     * @param rows2  the number of image 2 or kernel rows.
     * @param cols2  the number of image 2 or kernel columns.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_crossCorrelate(uint32_t sdSrc1, uint16_t rows1,
            uint16_t cols1, uint32_t sdSrc2, uint16_t rows2,
            uint16_t cols2, uint32_t sdDst);

    /**
     * Convolve an image with a kernel.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of image 1.
     * @param rows1  the number of image 1 rows.
     * @param cols1  the number of image 1 columns.
     * @param sdSrc2 the VMEM (SDRAM) address of image 2 or kernel.
     * @param rows2  the number of image 2 or kernel rows.
     * @param cols2  the number of image 2 or kernel columns.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_convolve(uint32_t sdSrc1, uint16_t rows1,
            uint16_t cols1, uint32_t sdSrc2, uint16_t rows2, uint16_t cols2,
            uint32_t sdDst);

    /**
     * Perform median filtering on an image.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_median(uint32_t sdSrc, uint16_t rows, uint16_t cols,
            uint32_t sdDst);

    /**
     * Cast all pixels of an image of float data type to 32 bit float data
     * type.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_cast(uint32_t sdSrc, uint16_t rows, uint16_t cols,
            uint32_t sdDst);

    /**
     * Invert all pixels of an image mask so that 0s become 1s and vice versa.
     *
     * @param sdSrc the VMEM (SDRAM) address of image.
     * @param rows  the number of image rows.
     * @param cols  the number of image columns.
     * @param sdDst the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_invertMask(uint32_t sdSrc1, uint16_t rows,
            uint16_t cols, uint32_t sdDst);

    /**
     * Crop an image.
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param rStart the start row.
     * @param cStart the start column.
     * @param rEnd   the end row.
     * @param cEnd   the end column.
     * @param sdDst  the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_cropImage(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, uint16_t rStart, uint16_t cStart, uint16_t rEnd,
            uint16_t cEnd, uint32_t sdDst);

    /**
     * Construct an image out of a number of rows.
     *
     * @param sdSrc   the VMEM (SDRAM) address of image.
     * @param rows    the number of image rows.
     * @param cols    the number of image columns.
     * @param rowsNew the number of rows after construction.
     * @param sdDst   the VMEM (SDRAM) address of result image.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
    int preprocessing_ana_constructRowImage(uint32_t sdSrc, uint16_t rows,
            uint16_t cols, unsigned int rowsNew, uint32_t sdDst);

#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSING_ANA_H */
