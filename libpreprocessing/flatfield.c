/*
 * Flatfield.c
 *
 *  Created on: 16 may. 2017
 *      Author: zaca
 */


#include "preprocessing/ana.h"
#include "preprocessing/arith.h"

#include "preprocessing/def.h"
#include "preprocessing/vmem.h"
#include "preprocessing/flatfield.h"
/* from libeve */
#include "../libeve/eve/fixed_point.h"

/* from std c */
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessing/def_flatfield.h"
#include "../udp/udp.h"

int preprocessing_arith_doGetConst(uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, int16_t iq, int16_t ir, uint32_t sdDst1, uint32_t sdDst2){

	int status = PREPROCESSING_SUCCESSFUL;

	//Calculate Mask of each image
	udp_loadImage(entriesOfNAND[MASK_TMP_INDEX], ROWS, COLS, sdTmp1);
	CHECK_STATUS(udp_getMask(sdTmp1, ROWS, COLS, ir, sdTmp2))
	CHECK_STATUS(udp_getMask(sdTmp1, ROWS, COLS, iq, sdTmp1))

	//Calculate ROIs of masks
	CHECK_STATUS(udp_createROI(sdTmp1 , rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(udp_createROI(sdTmp2 , rows, cols,  dx,  dy, sdTmp2))

	//Multiply ROIs to obtain mksDouble
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp3))

	udp_loadImage(entriesOfNAND[iq], ROWS, COLS, sdTmp1);
	udp_loadImage(entriesOfNAND[ir], ROWS, COLS, sdTmp2);

	//Calculate ROI of images
	CHECK_STATUS(udp_createROI(sdTmp1 , rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(udp_createROI(sdTmp2 , rows, cols,  dx,  dy, sdTmp2))

	//Calculate Diff
	CHECK_STATUS(preprocessing_arith_subtractImages(sdTmp1, sdTmp2, rows, cols, sdTmp1))
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp3, rows, cols, sdTmp1))

	//Apply diff to const
	CHECK_STATUS(udp_addROI(sdDst1, sdTmp1, rows, cols, -dx, -dy, sdDst1))
	CHECK_STATUS(udp_substractROI(sdDst1, sdTmp1, rows, cols, dx, dy, sdDst1))

	//Apply mskDouble to pixCount
	CHECK_STATUS(udp_addROI(sdDst2, sdTmp3, rows, cols, -dx, -dy, sdDst2))
	CHECK_STATUS(udp_addROI(sdDst2, sdTmp3, rows, cols,  dx,  dy, sdDst2))

	return status;
 }

int preprocessing_arith_iterate(uint32_t sdSrc, uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, uint16_t loops, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	for(uint16_t i = 0; i < loops; i++) {

		printf("\tItera %d of %d\n", i+1, loops);

		CHECK_STATUS(preprocessing_arith_doIteration(sdSrc, sdTmp1, sdTmp2, sdTmp3,
										rows, cols, sdDst))
	}

	udp_loadImage(entriesOfNAND[MASK_TMP_INDEX], ROWS, COLS, sdTmp1);
	CHECK_STATUS(udp_flatfield(sdDst, sdTmp1, rows, cols, sdDst))

	return status;
}

int preprocessing_arith_doIteration(uint32_t sdSrc, uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int sizeDisp = DISP_ROWS * DISP_COLS;
	unsigned int piq = 0;
	unsigned int pir = 0;

	const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc); 		//Disp
	int32_t* tmp2 = preprocessing_vmem_getDataAddress(sdTmp2);
	int32_t* tmp3 = preprocessing_vmem_getDataAddress(sdTmp3);

	// Check whether given rows and columns are in a valid range.
	if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, DISP_ROWS, DISP_COLS))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp2, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp3, rows, cols)))
	{
		return PREPROCESSING_INVALID_SIZE;
	}

	//Read Const from NAND (GainTmp)
	udp_loadImage(entriesOfNAND[CONS_INDEX], ROWS, COLS, sdTmp1);

	for(unsigned short iq = 1; iq < NUMBER_OF_IMAGES; iq++) {

		for(unsigned short ir = 0; ir < iq; ir++) {

			//Calculate point
			piq = iq*DISP_COLS;
			pir = ir*DISP_COLS;

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src, piq, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src, piq+1, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src, pir, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src, pir+1, sizeDisp);

			int dy = (int)eve_fp_subtract32(src[piq], src[pir])/FP32_BINARY_TRUE;
			int dx = (int)eve_fp_subtract32(src[piq + 1], src[pir + 1])/FP32_BINARY_TRUE;

			CHECK_STATUS(preprocessing_arith_doIterationTwoImages(sdDst, sdTmp2, sdTmp3, rows, cols, dx, dy, iq, ir, sdTmp1))
		}
	}

	//Normalize GainTmp
	udp_loadImage(entriesOfNAND[PIXCOUNT_INDEX], ROWS, COLS, sdTmp2);
	CHECK_STATUS(udp_normalize(sdTmp1, sdTmp2, rows, cols, sdTmp1))

	//Calculates mean (5-sigma)
	CHECK_STATUS(udp_mean(sdTmp1, sdTmp2, rows, cols, sdTmp3))

	// Check for valid pointer position.
	PREPROCESSING_DEF_CHECK_POINTER(tmp3, 0, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp3, 1, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp3, 2, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp3, 3, size);
	uint32_t mean = tmp3[0];
	uint32_t fiveSigma = tmp3[1];

	CHECK_STATUS(udp_fivesigma(sdTmp1, rows, cols, mean, fiveSigma, sdTmp2))

	PREPROCESSING_DEF_CHECK_POINTER(tmp2, 0, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp2, 1, size);

	uint32_t sum = eve_fp_subtract32(tmp3[3], tmp2[0]);
	uint32_t npix = eve_fp_subtract32(tmp3[2], tmp2[1]);

	uint32_t aver = eve_fp_divide32(sum, npix, FP32_FWL);

	//Update Gain
	CHECK_STATUS(preprocessing_arith_subtractScalar(sdTmp1, rows, cols, aver, sdDst))

	return status;
}

int preprocessing_arith_doIterationTwoImages(uint32_t Src, uint32_t sdTmp1, uint32_t sdTmp2,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, int16_t iq, int16_t ir, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	//Obtain Mask iq, ir
	udp_loadImage(entriesOfNAND[MASK_TMP_INDEX], ROWS, COLS, sdTmp1);
	CHECK_STATUS(udp_getMask(sdTmp1, rows, cols, ir, sdTmp2))
	CHECK_STATUS(udp_getMask(sdTmp1, rows, cols, iq, sdTmp1))

	//Calculate ROI masks
	CHECK_STATUS(udp_createROI(sdTmp1 , rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(udp_createROI(sdTmp2 , rows, cols,  dx,  dy, sdTmp2))

	//Calculate mskDouble
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp2))

	//Modify GainTmp
	CHECK_STATUS(preprocessing_ana_underThresh(sdTmp1, rows, cols, EVE_FP32_NAN, sdTmp1))
	CHECK_STATUS(udp_createROI(Src, rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp1))
	CHECK_STATUS(udp_addROI(sdDst, sdTmp1, rows, cols, dx, dy, sdDst))

	CHECK_STATUS(preprocessing_ana_underThresh(sdTmp1, rows, cols, EVE_FP32_NAN, sdTmp1))
	CHECK_STATUS(udp_createROI(Src, rows, cols,  dx,  dy, sdTmp1))
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp1))
	CHECK_STATUS(udp_addROI(sdDst, sdTmp1, rows, cols,  -dx,  -dy, sdDst))

	return status;
}
