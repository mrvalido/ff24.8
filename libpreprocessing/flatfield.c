/*
 * hough.c
 *
 *  Created on: 16 may. 2017
 *      Author: zaca
 */


#include "preprocessing/ana.h"
#include "preprocessing/arith.h"

#include "preprocessing/def.h"
#include "preprocessing/mydef.h"
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

//NAND FLASH METHODS

void createNANDFLASH(int32_t *NANDFLASH, int32_t **entriesOfNAND, int stdimagesize,  int numberOfImages){

	int nkeys;
	char **header;
	int *inputimg = (int*) malloc((uint32_t)stdimagesize*sizeof(int));			//Only one image

	printf("Load images in NAND FLASH!\n");
	char fileName[13] = "im/im00.fits";
	for(unsigned int i = 0; i < numberOfImages; i++) {
		fileName[6] = 48 + i;
		printf("%s\n", fileName);
		FITS_getImage(fileName, inputimg, stdimagesize, &nkeys, &header);
		for (int j = 0; j < stdimagesize; j++)
			NANDFLASH[i*stdimagesize + j]=(int32_t)eve_fp_int2s32(inputimg[j], FP32_FWL );
	}

	char maskFileName[13] = "im/mask.fits";
	printf("%s\n", maskFileName);
	FITS_getImage(maskFileName, inputimg, stdimagesize, &nkeys, &header);
	for (int j = 0; j < stdimagesize; j++)
		NANDFLASH[MASK_INDEX*stdimagesize + j]=(int32_t)eve_fp_int2s32(inputimg[j], FP32_FWL );


	//	1.) Load image data to NAND Flash:
	uint32_t 	img1Nand = 0;
	uint32_t 	img2Nand = img1Nand + stdimagesize;
	uint32_t	img3Nand = img2Nand + stdimagesize;
	uint32_t	img4Nand = img3Nand + stdimagesize;
	uint32_t	img5Nand = img4Nand + stdimagesize;
	uint32_t	img6Nand = img5Nand + stdimagesize;
	uint32_t	img7Nand = img6Nand + stdimagesize;
	uint32_t	img8Nand = img7Nand + stdimagesize;
	uint32_t	img9Nand = img8Nand + stdimagesize;
	uint32_t	maskNand = img9Nand + stdimagesize;
	uint32_t	dispNand = maskNand + stdimagesize;

	entriesOfNAND[0] =(NANDFLASH+img1Nand);
	entriesOfNAND[1] =(NANDFLASH+img2Nand);
	entriesOfNAND[2] =(NANDFLASH+img3Nand);
	entriesOfNAND[3] =(NANDFLASH+img4Nand);
	entriesOfNAND[4] =(NANDFLASH+img5Nand);
	entriesOfNAND[5] =(NANDFLASH+img6Nand);
	entriesOfNAND[6] =(NANDFLASH+img7Nand);
	entriesOfNAND[7] =(NANDFLASH+img8Nand);
	entriesOfNAND[8] =(NANDFLASH+img9Nand);
	entriesOfNAND[9] =(NANDFLASH+maskNand);
	entriesOfNAND[10]=(NANDFLASH+dispNand);

	//READ DISP
	int MAXCHAR = 1000;
	FILE *fp2;
	char str[MAXCHAR];
	char* filename = "disp.txt";

	fp2 = fopen(filename, "r");
	if (fp2 == NULL){
		printf("Could not open file %s",filename);
		return;
	}

	int index=0;
	while (fgets(str, MAXCHAR, fp2) != NULL){
		char *ch;
		ch = strtok(str, " ");
		while (ch != NULL) {
			NANDFLASH[DISP_INDEX*stdimagesize + index]=(int32_t)eve_fp_int2s32(atoi(ch), FP32_FWL );
			index++;
			ch = strtok(NULL, " ,");
		}
	}
	fclose(fp2);

	printf("Images loaded successfully!\n");
}

int readNAND(int32_t *nandSrc, uint16_t rows, uint16_t cols, uint32_t sdDst){
	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

	// Check whether given rows and columns are in a valid range.
	if (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)){
		return PREPROCESSING_INVALID_SIZE;
	}

	// Process.
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			p = r * cols + c;

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			dst[p] = nandSrc[p];

			if (dst[p] == EVE_FP32_NAN)
			{
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int writeNAND(uint32_t sdSrc, uint16_t rows, uint16_t cols, int32_t *nandDst){
	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);

	// Check whether given rows and columns are in a valid range.
	if (!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols)){
		return PREPROCESSING_INVALID_SIZE;
	}

	// Process.
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			p = r * cols + c;

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src, p, size);

			nandDst[p]=src[p];

			if (src[p] == EVE_FP32_NAN)
			{
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}
//END OF NAND FLASH METHODS

int preprocessing_zero(uint16_t rows, uint16_t cols, uint32_t sdDst)
{
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;

    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols))
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
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

				dst[p] = 0;

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

int preprocessing_arith_maskImagesLog10(uint32_t sdSrc,
            uint16_t rows, uint16_t cols, uint16_t index, uint32_t iMin, uint32_t iMax, uint32_t sdDst){
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;
    unsigned int mskTmp = 0;

    uint32_t zero = 0;

    int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
            || (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
    {
        return PREPROCESSING_INVALID_SIZE;
    }

    for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			p = r * cols + c;

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			//Checks if the source value is in [iMin, iMax] range
			if( (eve_fp_compare32(src + p, &iMin) == 1 ) && ((eve_fp_compare32(src + p, &iMax) == -1 ) || (eve_fp_compare32(src + p, &iMax) == 0 )) ){
				mskTmp = 1;

				//Calculate the log10 of the current pixel if greater than 1
				if((eve_fp_compare32(src + p, &zero) == 1 ) ){
					src[p] = eve_fp_double2s32(
												log10(eve_fp_signed32ToDouble(src[p], FP32_FWL)),
												FP32_FWL);
				}
			}
			else{
				mskTmp = 0;
				src[p] = 0;
			}

			dst[p] = dst[p] | ((mskTmp << index) << FP32_FWL);

			if (dst[p] == EVE_FP32_NAN || src[p] == EVE_FP32_NAN )
			{
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

    return status;
}

int preprocessing_arith_ROI(uint32_t sdSrc, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;
	unsigned int roiPoint = 0;

	//Calculate window edges
	unsigned int jyl = max(0, -dy);				//Row
	unsigned int jyh = min(0, -dy) + rows; 	//Row
	unsigned int jxl = max(0, -dx); 				//Column
	unsigned int jxh = min(0, -dx) + cols; 	//Column

	const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

	// Check whether given rows and columns are in a valid range.
	if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
	{
		return PREPROCESSING_INVALID_SIZE;
	}

	//Calculate ROI
	for(int y=jyl; y < jyh; y++){
		for(int x=jxl; x < jxh; x++){

			p = y * cols + x;
			roiPoint = (y-jyl) * cols + (x-jxl);

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, roiPoint, size);

			dst[roiPoint] = src[p];

			if (dst[roiPoint] == EVE_FP32_NAN)
			{
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int preprocessing_arith_addROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;
	unsigned int roiPoint = 0;

	// Calculate window edges
	unsigned int jyl = max(0, -dy), jyh = min(0, -dy) + rows; 	// ROWS
	unsigned int jxl = max(0, -dx), jxh = min(0, -dx) + cols; 		// COLUMNS

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

	//Calculate sum
	for(int y=jyl; y < jyh; y++){
		for(int x=jxl; x < jxh; x++){

			p = y * cols + x;
			roiPoint = (y-jyl) * cols + (x-jxl);

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src1, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(src2, roiPoint, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			dst[p] = eve_fp_add32(src1[p], src2[roiPoint]);

			if (dst[p] == EVE_FP32_NAN)
			{
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int preprocessing_arith_substractROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;
	unsigned int roiPoint = 0;

	// Calculate window edges
	unsigned int jyl = max(0, -dy), jyh = min(0, -dy) + rows; 	// ROWS
	unsigned int jxl = max(0, -dx), jxh = min(0, -dx) + cols; 		// COLUMNS

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

	//Calculate sum
	for(int y=jyl; y < jyh; y++){
		for(int x=jxl; x < jxh; x++){

			p = y * cols + x;
			roiPoint = (y-jyl) * cols + (x-jxl);

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src1, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(src2, roiPoint, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			dst[p] = eve_fp_subtract32(src1[p], src2[roiPoint]);

			if (dst[p] == EVE_FP32_NAN)
			{
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int preprocessing_arith_equalImages(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint32_t sdDst){

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
			PREPROCESSING_DEF_CHECK_POINTER(src, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			dst[p] = src[p];

			if (dst[p] == EVE_FP32_NAN)
			{
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int preprocessing_arith_normalicer(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	uint32_t one = FP32_BINARY_TRUE;

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
			PREPROCESSING_DEF_CHECK_POINTER(src1, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			if(eve_fp_compare32(src2 + p, &one) == 1){
				dst[p] = eve_fp_divide32(src1[p], src2[p], FP32_FWL);
			}

			if (dst[p] == EVE_FP32_NAN){
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int16_t max(int16_t a, int16_t b){
	if(a >= b) return a;
	return b;
}

int16_t min(int16_t a, int16_t b){
	if(a <= b) return a;
	return b;
}

int preprocessing_arith_mean(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	int npix = 0;
	uint32_t sum2 = 0;
	uint32_t sum3 = 0;
	uint32_t zero = 0;

	const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);		//GTmp
	const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);		//PixCnt
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);						//Media Matrix

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
			PREPROCESSING_DEF_CHECK_POINTER(src1, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(src2, p, size);

			if (eve_fp_compare32(src2 + p, &zero) == 1){
				sum2 = eve_fp_add32(sum2, src1[p]);
				sum3 = eve_fp_add32(sum3, eve_fp_multiply32(src1[p], src1[p], FP32_FWL));
				npix++;
			}
		}
	}

	// Check for valid pointer position.
	PREPROCESSING_DEF_CHECK_POINTER(dst, 0, size);
	PREPROCESSING_DEF_CHECK_POINTER(dst, 1, size);
	PREPROCESSING_DEF_CHECK_POINTER(dst, 2, size);
	PREPROCESSING_DEF_CHECK_POINTER(dst, 3, size);

	uint32_t npixFixed = eve_fp_int2s32(npix, FP32_FWL);
	dst[0] = eve_fp_divide32(sum2, npixFixed, FP32_FWL);		//Mean

	//Calculate 5 sigma
	//Original ->  5*sqrt( (sum3/npix) - dst[0]*dst[0] );
	uint32_t fiveFixed = eve_fp_int2s32(5, FP32_FWL);
	uint32_t sum3DividedByNpix = eve_fp_divide32(sum3, npixFixed, FP32_FWL);
	uint32_t pow2mean = eve_fp_multiply32(dst[0], dst[0], FP32_FWL);
	uint32_t divisionMinusPow = eve_fp_subtract32(sum3DividedByNpix, pow2mean );
	uint32_t sqrtOfDivisionMinusPow = eve_fp_double2s32( sqrt(eve_fp_signed32ToDouble( divisionMinusPow , FP32_FWL)), FP32_FWL);
	dst[1] = eve_fp_multiply32( fiveFixed, sqrtOfDivisionMinusPow, FP32_FWL);

	dst[2] = npixFixed;			//number of pixels
	dst[3] = sum2;

	if (dst[0] == EVE_FP32_NAN || dst[1] == EVE_FP32_NAN || dst[2] == EVE_FP32_NAN || dst[3] == EVE_FP32_NAN){
		status = PREPROCESSING_INVALID_NUMBER;
	}

	return status;
}

int preprocessing_arith_criba_fivesigma(uint32_t sdSrc, uint32_t mean, uint32_t fiveSigma,
		uint16_t rows, uint16_t cols, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	int npix = 0;
	uint32_t sum = 0;
	uint32_t tmp1 = 0;
	double tmp2 = 0;

	const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);	//GainTmp
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);				//Matrix Stats

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
			PREPROCESSING_DEF_CHECK_POINTER(src, p, size);

			//Original -> abs(val[i] - aver2) > fiveSigma
			tmp1 = eve_fp_subtract32(src[p], mean);
			tmp2 = eve_fp_signed32ToDouble(tmp1 , FP32_FWL);
			tmp2 = fabs(tmp2);
			tmp1 = eve_fp_double2s32(tmp2, FP32_FWL);

			if(eve_fp_compare32(&tmp1, &fiveSigma) == 1){
				sum = eve_fp_add32(sum, src[p]);
				npix++;
			}
		}
	}

	// Check for valid pointer position.
	PREPROCESSING_DEF_CHECK_POINTER(dst, 0, size);
	PREPROCESSING_DEF_CHECK_POINTER(dst, 1, size);

	dst[0] = sum;
	dst[1] =  eve_fp_int2s32(npix, FP32_FWL);

	if (dst[0] == EVE_FP32_NAN || dst[1] == EVE_FP32_NAN){
		status = PREPROCESSING_INVALID_NUMBER;
	}

	return status;
}

int preprocessing_arith_flatfield(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	uint32_t zero = 0;

	const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);		//GainTmp
	const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);		//Mask of all images
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);						//Flatfield

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
			PREPROCESSING_DEF_CHECK_POINTER(src1, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(src2, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			if (eve_fp_compare32(src2 + p, &zero) != 0){
				dst[p] = eve_fp_double2s32( pow(10.0, eve_fp_signed32ToDouble( src1[p] , FP32_FWL)), FP32_FWL);
			}

			if (dst[p] == EVE_FP32_NAN){
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int preprocessing_getMask(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint16_t index, uint32_t sdDst)
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
            PREPROCESSING_DEF_CHECK_POINTER(src, p, size);
            PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			dst[p] = (src[p] & (FP32_BINARY_TRUE << index)) >> index;

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

int preprocessing_arith_doGetConst(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3, uint32_t sdSrc4,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, uint32_t sdDst1, uint32_t sdDst2){

	int status = PREPROCESSING_SUCCESSFUL;

	//Calculate ROIs of masks
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc3 , rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc4 , rows, cols,  dx,  dy, sdTmp2))

	//Multiply ROIs to obtain mksDouble
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp3))

	CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp1))
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp2))

	//Calculate ROI of images
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc1 , rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc2 , rows, cols,  dx,  dy, sdTmp2))

	//Calculate Diff
	CHECK_STATUS(preprocessing_arith_subtractImages(sdTmp1, sdTmp2, rows, cols, sdTmp1))
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp3, rows, cols, sdTmp1))

	//Apply diff to const
	CHECK_STATUS(preprocessing_arith_addROI(sdDst1, sdTmp1, rows, cols, -dx, -dy, sdDst1))
	CHECK_STATUS(preprocessing_arith_substractROI(sdDst1, sdTmp1, rows, cols, dx, dy, sdDst1))

	//Apply mskDouble to pixCount
	CHECK_STATUS(preprocessing_arith_addROI(sdDst2, sdTmp3, rows, cols, -dx, -dy, sdDst2))
	CHECK_STATUS(preprocessing_arith_addROI(sdDst2, sdTmp3, rows, cols,  dx,  dy, sdDst2))

	return status;
 }

int preprocessing_arith_iterate(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3, uint32_t sdSrc4,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3, uint32_t sdTmp4, uint32_t sdTmp5,
		uint16_t rows, uint16_t cols, uint16_t loops, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	for(uint16_t i = 0; i < loops; i++) {

		printf("\tItera %d of %d\n", i+1, loops);

		CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp1))
		CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp2))
		CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp3))
		CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp4))
		CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp5))

		CHECK_STATUS(preprocessing_arith_doIteration(sdSrc1, sdSrc2, sdSrc3, sdSrc4,
										sdTmp1, sdTmp2, sdTmp3, sdTmp4, sdTmp5,
										rows, cols, sdDst))
	}

	CHECK_STATUS(preprocessing_arith_flatfield(sdDst, sdSrc2, rows, cols, sdDst))

	return status;
}

int preprocessing_arith_doIteration(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3, uint32_t sdSrc4,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3, uint32_t sdTmp4, uint32_t sdTmp5,
		uint16_t rows, uint16_t cols, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int sizeDisp = DISP_ROWS * DISP_COLS;
	unsigned int piq = 0;
	unsigned int pir = 0;

	const int32_t* src4 = preprocessing_vmem_getDataAddress(sdSrc4); 		//Disp
	const int32_t* tmp4 = preprocessing_vmem_getDataAddress(sdTmp4);
	const int32_t* tmp5 = preprocessing_vmem_getDataAddress(sdTmp5);

	// Check whether given rows and columns are in a valid range.
	if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc4, DISP_ROWS, DISP_COLS))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp4, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp5, rows, cols)))
	{
		return PREPROCESSING_INVALID_SIZE;
	}

	//Creates a copy of Con (GainTmp)
	CHECK_STATUS(preprocessing_arith_equalImages(sdSrc1, rows, cols, sdTmp1))

	for(unsigned short iq = 1; iq < NUMBER_OF_IMAGES; iq++) {

		//Obtain iq mask
		CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp2))
		CHECK_STATUS(preprocessing_getMask(sdSrc2, rows, cols, iq, sdTmp2))

		for(unsigned short ir = 0; ir < iq; ir++) {

			//Obtain ir mask
			CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp3))
			CHECK_STATUS(preprocessing_getMask(sdSrc2, rows, cols, ir, sdTmp3))

			//Calculate point
			piq = iq*DISP_COLS;
			pir = ir*DISP_COLS;

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src4, piq, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src4, piq+1, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src4, pir, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src4, pir+1, sizeDisp);

			int dy = (int)eve_fp_subtract32(src4[piq], src4[pir])/FP32_BINARY_TRUE;
			int dx = (int)eve_fp_subtract32(src4[piq + 1], src4[pir + 1])/FP32_BINARY_TRUE;

			CHECK_STATUS(preprocessing_arith_doIterationTwoImages(sdDst, sdTmp2, sdTmp3, sdTmp4, sdTmp5, rows, cols, dx, dy, sdTmp1))
		}
	}

	//Normalize GainTmp
	CHECK_STATUS(preprocessing_arith_normalicer(sdTmp1, sdSrc3, rows, cols, sdTmp1))

	//Calculates mean (5-sigma)
	CHECK_STATUS(preprocessing_arith_mean(sdTmp1, sdSrc3, rows, cols, sdTmp4))

	// Check for valid pointer position.
	PREPROCESSING_DEF_CHECK_POINTER(tmp4, 0, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp4, 1, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp4, 2, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp4, 3, size);
	uint32_t mean = tmp4[0];
	uint32_t fiveSigma = tmp4[1];

	CHECK_STATUS(preprocessing_arith_criba_fivesigma(sdTmp1, mean, fiveSigma, rows, cols, sdTmp5))

	PREPROCESSING_DEF_CHECK_POINTER(tmp5, 0, size);
	PREPROCESSING_DEF_CHECK_POINTER(tmp5, 1, size);

	uint32_t sum = eve_fp_subtract32(tmp4[3], tmp5[0]);
	uint32_t npix = eve_fp_subtract32(tmp4[2], tmp5[1]);

	uint32_t aver = eve_fp_divide32(sum, npix, FP32_FWL);

	//Update Gain
	CHECK_STATUS(preprocessing_arith_subtractScalar(sdTmp1, rows, cols, aver, sdDst))

	return status;
}

int preprocessing_arith_doIterationTwoImages(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3,
		uint32_t sdTmp1, uint32_t sdTmp2,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	//Cleaning Temps
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp1))
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp2))

	//Calculate ROI masks
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc2 , rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc3 , rows, cols,  dx,  dy, sdTmp2))

	//Calculate mskDouble
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp2))

	//Modify GainTmp
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp1))
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc1, rows, cols, -dx, -dy, sdTmp1))
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp1))
	CHECK_STATUS(preprocessing_arith_addROI(sdDst, sdTmp1, rows, cols, dx, dy, sdDst))

	CHECK_STATUS(preprocessing_zero(ROWS, COLS, sdTmp1))
	CHECK_STATUS(preprocessing_arith_ROI(sdSrc1, rows, cols,  dx,  dy, sdTmp1))
	CHECK_STATUS(preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp1))
	CHECK_STATUS(preprocessing_arith_addROI(sdDst, sdTmp1, rows, cols,  -dx,  -dy, sdDst))

	return status;
}
