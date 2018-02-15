/*
 * Flatfield.h
 *
 *  Created on: 17 may. 2017
 *      Author: zaca
 */

#ifndef LIBPREPROCESSING_PREPROCESSING_HOUGH_H_
#define LIBPREPROCESSING_PREPROCESSING_HOUGH_H_

#include <sys/types.h>
#include <stdint.h>
#include "../../fits/FITS_Interface.h"

int32_t **entriesOfNAND;


/**
     * Get algorithm's constant term of two images
     *
     * @param sdTmp1 	the VMEM (SDRAM) address of temporal image.
     * @param sdTmp2 	the VMEM (SDRAM) address of temporal image.
     * @param sdTmp3 	the VMEM (SDRAM) address of temporal image.
     * @param rows   	the number of image rows.
     * @param cols   	the number of image columns.
     * @param dx  	 	the offset X.
     * @param dy     	the offset Y.
     * @param iq		index of image iq
     * @param ir		index of image ir
     * @param sdDst1  	the VMEM (SDRAM) address of const.
     * @param sdDst2  	the VMEM (SDRAM) address of pixCount.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_arith_doGetConst(uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, int16_t iq, int16_t ir, uint32_t sdDst1, uint32_t sdDst2);

/**
     * Calculates Iterate
     *
     * @param sdSrc 	the VMEM (SDRAM) address of disp.
     * @param sdTmp1 	the VMEM (SDRAM) address of temporal image.
     * @param sdTmp2 	the VMEM (SDRAM) address of temporal image.
     * @param sdTmp3 	the VMEM (SDRAM) address of temporal image.
     * @param rows   	the number of image rows.
     * @param cols   	the number of image columns.
     * @param loops 	the number of inner for loop iterations
     * @param sdDst  	the VMEM (SDRAM) address of gain.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_arith_iterate(uint32_t sdSrc, uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, uint16_t loops, uint32_t sdDst);

/**
     * Calculates one Iteration
     *
     * @param sdSrc 	the VMEM (SDRAM) address of disp.
     * @param sdTmp1 	the VMEM (SDRAM) address of temporal image.
     * @param sdTmp2 	the VMEM (SDRAM) address of temporal image.
     * @param sdTmp3 	the VMEM (SDRAM) address of temporal image.
     * @param rows   	the number of image rows.
     * @param cols   	the number of image columns.
     * @param sdDst  	the VMEM (SDRAM) address of gain.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_arith_doIteration(uint32_t sdSrc, uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, uint32_t sdDst);

/**
     * Calculate gain of two images for doIteration function
     *
     * @param sdSrc 	the VMEM (SDRAM) address of gain.
     * @param sdTmp1 	the VMEM (SDRAM) address of temporal image.
     * @param sdTmp2 	the VMEM (SDRAM) address of temporal image.
     * @param rows   	the number of image rows.
     * @param cols   	the number of image columns.
     * @param dx  	 	the offset X.
     * @param dy     	the offset Y.
     * @param iq		index of image iq
     * @param ir		index of image ir
     * @param sdDst  	the VMEM (SDRAM) address of gainTmp.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int preprocessing_arith_doIterationTwoImages(uint32_t Src, uint32_t sdTmp1, uint32_t sdTmp2,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, int16_t iq, int16_t ir, uint32_t sdDst);

#endif /* LIBPREPROCESSING_PREPROCESSING_HOUGH_H_ */
