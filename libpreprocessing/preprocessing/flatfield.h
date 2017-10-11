/*
 * hough.h
 *
 *  Created on: 17 may. 2017
 *      Author: zaca
 */

#ifndef LIBPREPROCESSING_PREPROCESSING_HOUGH_H_
#define LIBPREPROCESSING_PREPROCESSING_HOUGH_H_

#include <sys/types.h>
#include <stdint.h>
#include "../FITS_Interface.h"

//NAND FLASH METHODS
void createNANDFLASH(int32_t *NANDFLASH, int32_t **entriesOfNAND, int stdimagesize,  int numberOfImages);

int readNAND(int32_t *nandSrc, uint16_t rows, uint16_t cols, uint32_t sdDst);

int writeNAND(uint32_t sdSrc, uint16_t rows, uint16_t cols, int32_t *nandDst);
//END OF NAND FLASH METHODS


int preprocessing_zero(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint32_t sdDst);

/**
     * Generates mask of an image pixel by pixel.
     *
     * @param sdSrc1 the VMEM (SDRAM) address of image 1.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param index  the index of the current image.
     * @param iMin   the min intesity value of the input image.
     * @param iMax   the max intesity value of the input image.
     * @param sdDst  the VMEM (SDRAM) address of result: input image masked.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_arith_maskImages(uint32_t sdSrc1,
            uint16_t rows, uint16_t cols, uint16_t index, uint32_t iMin, uint32_t iMax, uint32_t sdDst);

/**
    * Generate a ROI from an image
    *
    * @param sdSrc 	the VMEM (SDRAM) address of image.
    * @param rows   the number of image rows.
    * @param cols   the number of image columns.
    * @param dx		the X position of begining
    * @param dy     the Y position of begining
    * @param sdDst  the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int preprocessing_arith_ROI(uint32_t sdSrc, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst);

/**
    * Add a ROI to an image
    *
    * @param sdSrc1 the VMEM (SDRAM) address of image.
    * @param sdSrc2	the VMEM (SDRAM) address of ROI
    * @param rows   the number of image rows.
    * @param cols   the number of image columns.
    * @param dx		the X position of begining
    * @param dy     the Y position of begining
    * @param sdDst  the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int preprocessing_arith_addROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst);

/**
    * Substract a ROI to an image
    *
    * @param sdSrc1 the VMEM (SDRAM) address of image.
    * @param sdSrc2	the VMEM (SDRAM) address of ROI
    * @param rows   the number of image rows.
    * @param cols   the number of image columns.
    * @param dx		the X position of begining
    * @param dy     the Y position of begining
    * @param sdDst  the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int preprocessing_arith_substractROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst);

/**
    * Creates a copy of an image
    *
    * @param sdSrc  the VMEM (SDRAM) address of image.
    * @param rows   the number of image rows.
    * @param cols   the number of image columns.
    * @param sdDst  the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int preprocessing_arith_equalImages(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint32_t sdDst);

int preprocessing_arith_normalicer(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst);

int16_t max(int16_t a, int16_t b);
int16_t min(int16_t a, int16_t b);


/**
     * Get mask of image i
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param index  the index of the current image.
     * @param sdDst  the VMEM (SDRAM) address of result: mask of image i.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_getMask(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint16_t index, uint32_t sdDst);


/**
     * Get log of an image
     *
     * @param sdSrc  the VMEM (SDRAM) address of image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param sdDst  the VMEM (SDRAM) address of result.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_log10Image(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint32_t sdDst);


/**
     * Get algorithm's constant term of two images
     *
     * @param sdSrc1 the VMEM (SDRAM) address of log10 image 1.
     * @param sdSrc2 the VMEM (SDRAM) address of log10 image 2.
     * @param sdSrc3 the VMEM (SDRAM) address of mask of image 1.
     * @param sdSrc4 the VMEM (SDRAM) address of mask of image 2.
     * @param sdTmp1 the VMEM (SDRAM) address of temporal image.
     * @param sdTmp2 the VMEM (SDRAM) address of temporal image.
     * @param sdTmp3 the VMEM (SDRAM) address of temporal image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param dx  	 the offset X.
     * @param dy     the offset Y.
     * @param sdDst1  the VMEM (SDRAM) address of const.
     * @param sdDst2  the VMEM (SDRAM) address of pixCount.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_arith_doGetConst(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3, uint32_t sdSrc4,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, uint32_t sdDst1, uint32_t sdDst2);

/**
     * Calculate gain of two images for doIteration function
     *
     * @param sdSrc1 the VMEM (SDRAM) address of gain.
     * @param sdSrc2 the VMEM (SDRAM) address of mask 1.
     * @param sdSrc3 the VMEM (SDRAM) address of mask 2.
     * @param sdTmp1 the VMEM (SDRAM) address of temporal image.
     * @param sdTmp2 the VMEM (SDRAM) address of temporal image.
     * @param sdTmp3 the VMEM (SDRAM) address of temporal image.
     * @param rows   the number of image rows.
     * @param cols   the number of image columns.
     * @param dx  	 the offset X.
     * @param dy     the offset Y.
     * @param sdDst  the VMEM (SDRAM) address of gainTmp.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_arith_doIterationTwoImages(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, uint32_t sdDst);

#endif /* LIBPREPROCESSING_PREPROCESSING_HOUGH_H_ */
