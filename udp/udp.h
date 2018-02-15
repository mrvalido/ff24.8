/*
 * udp.h
 *
 *  Created on: 02 feburary. 2018
 *      Author: dennis
 */

#ifndef UDP_UDP_H
#define UDP_UDP_H

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>

/* from libeve */
#include "../libeve/eve/fixed_point.h"

/* from libpreprocessing */
#include "../libpreprocessing/preprocessing/ana.h"
#include "../libpreprocessing/preprocessing/arith.h"
#include "../libpreprocessing/preprocessing/def.h"
#include "../libpreprocessing/preprocessing/def_flatfield.h"
#include "../libpreprocessing/preprocessing/vmem.h"

/* from fits */
#include "../fits/FITS_Interface.h"


/**
 * NAND creation, reading and writing functions.
 *
 * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
 * @{
 */
void udp_createNANDFLASH(int32_t *NANDFLASH, int32_t **entriesOfNAND,
        int stdimagesize,  int numberOfImages);
int udp_loadImage(int32_t *nandSrc, uint16_t rows, uint16_t cols,
            uint32_t sdDst);
int udp_storeImage(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        int32_t *nandDst);
/**
 * @}
 */

/**
     * Get mask of image i
     *
     * @param sdSrc  	the VMEM (SDRAM) address of image.
     * @param rows   	the number of image rows.
     * @param cols   	the number of image columns.
     * @param index  	the index of the current image.
     * @param sdDst  	the VMEM (SDRAM) address of result: mask of image i.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int udp_getMask(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint16_t index, uint32_t sdDst);

/**
     * Generates mask of an image pixel by pixel, and calculates log10 of the source image.
     *
     * @param sdSrc 	the VMEM (SDRAM) address of image.
     * @param rows   	the number of image rows.
     * @param cols   	the number of image columns.
     * @param index  	the index of the current image.
     * @param iMin  	the min intesity value of the input image.
     * @param iMax   	the max intesity value of the input image.
     * @param sdDst  	the VMEM (SDRAM) address of result: input image masked.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int udp_maskImagesLog10(uint32_t sdSrc,
            uint16_t rows, uint16_t cols, uint16_t index, uint32_t iMin, uint32_t iMax, uint32_t sdDst);

/**
    * Generate a ROI from an image
    *
    * @param sdSrc 		the VMEM (SDRAM) address of image.
    * @param rows   	the number of image rows.
    * @param cols   	the number of image columns.
    * @param dx			the X position of beginning
    * @param dy     	the Y position of beginning
    * @param sdDst  	the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int udp_createROI(uint32_t sdSrc, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst);

/**
    * Add a ROI to an image
    *
    * @param sdSrc1 	the VMEM (SDRAM) address of image.
    * @param sdSrc2		the VMEM (SDRAM) address of ROI
    * @param rows   	the number of image rows.
    * @param cols   	the number of image columns.
    * @param dx			the X position of begining
    * @param dy     	the Y position of begining
    * @param sdDst  	the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int udp_addROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst);

/**
    * Substract a ROI to an image
    *
    * @param sdSrc1 	the VMEM (SDRAM) address of image.
    * @param sdSrc2		the VMEM (SDRAM) address of ROI
    * @param rows   	the number of image rows.
    * @param cols   	the number of image columns.
    * @param dx			the X position of beginning
    * @param dy     	the Y position of beginning
    * @param sdDst  	the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int udp_substractROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst);

/**
    * Normalize an image using PixCnt
    *
    * @param sdSrc1  	the VMEM (SDRAM) address of image.
    * @param sdSrc2  	the VMEM (SDRAM) address of pixCnt.
    * @param rows   	the number of image rows.
    * @param cols   	the number of image columns.
    * @param sdDst  	the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int udp_normalize(uint32_t sdSrc1, uint32_t sdSrc2,  uint16_t rows, uint16_t cols, uint32_t sdDst);

/**
    * Calculates mean (5-sigma)
    *
    * @param sdSrc1  	the VMEM (SDRAM) address of normalized gainTmp.
    * @param sdSrc2  	the VMEM (SDRAM) address of pixCnt.
    * @param rows   	the number of image rows.
    * @param cols   	the number of image columns.
    * @param sdDst  	the VMEM (SDRAM) address of result image.
    *
    * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
    */
int udp_mean(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst);

/**
     * Criba 5-sigma creating stats matrix
     *
     * @param sdSrc  		the VMEM (SDRAM) address of normalized gainTmp.
     * @param rows   		the number of image rows.
     * @param cols   		the number of image columns.
     * @param mean			the value of mean (obtained from 'mean' method).
     * @param fiveSigma   	the value of 5-sigma (obtained from 'mean' method).
     * @param sdDst  		the VMEM (SDRAM) address of result: stats matrix.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int udp_fivesigma(uint32_t sdSrc, uint16_t rows, uint16_t cols,
		uint32_t mean, uint32_t fiveSigma, uint32_t sdDst);

/**
     * Calculates flatfield using ln10
     *
     * @param sdSrc1  	the VMEM (SDRAM) address of normalized gainTmp.
     * @param sdSrc2	the VMEM (SDRAM) address of masks.
     * @param rows   	the number of image rows.
     * @param cols   	the number of image columns.
     * @param sdDst  	the VMEM (SDRAM) address of result: flatfield.
     *
     * @return PREPROCESSING_SUCCESSFUL on success, failure code otherwise.
     */
int udp_flatfield(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst);


/**
    * Returns the max of two variables
    *
    * @param a  	variable 1.
    * @param b  	variable 2.
    *
    * @return maximum value.
    */
int16_t udp_max16(int16_t a, int16_t b);

/**
    * Returns the min of two variables
    *
    * @param a  	variable 1.
    * @param b  	variable 2.
    *
    * @return minimum value.
    */
int16_t udp_min16(int16_t a, int16_t b);

/**
    * Convert double to floating point, rounding the number first
    *
    * @param value  		Original value in double
    * @param fractionBits  	Word length
    *
    * @return value converted to floating point.
    */
int32_t udp_double2s32rounded(double value, unsigned int fractionBits);


#endif /* UDP_UDP_H */
