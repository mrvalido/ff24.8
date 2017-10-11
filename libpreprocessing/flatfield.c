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

uint32_t *i1NAND;
uint32_t *i2NAND;
uint32_t *i3NAND;
uint32_t *i4NAND;
uint32_t *i5NAND;
uint32_t *i6NAND;
uint32_t *i7NAND;
uint32_t *i8NAND;
uint32_t *i9NAND;
uint32_t *maskNAND;
uint32_t *dispNAND;


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

int preprocessing_zero(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint32_t sdDst)
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

				dst[p] = 0;

            if (dst[p] == EVE_FP32_NAN)
            {
                status = PREPROCESSING_INVALID_NUMBER;
            }
        }
    }

    return status;
}

int preprocessing_arith_maskImages(uint32_t sdSrc1,
            uint16_t rows, uint16_t cols, uint16_t index, uint32_t iMin, uint32_t iMax, uint32_t sdDst){
    int status = PREPROCESSING_SUCCESSFUL;
    unsigned int size = (unsigned int)(rows) * cols;
    unsigned int p = 0;
    unsigned int mskTmp = 0;

    int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
    int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

    // Check whether given rows and columns are in a valid range.
    if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
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
			PREPROCESSING_DEF_CHECK_POINTER(src1, p, size)
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size)

			if( (eve_fp_compare32(src1 + p, &iMin) == 1 ) && ((eve_fp_compare32(src1 + p, &iMax) == -1 ) || (eve_fp_compare32(src1 + p, &iMax) == 0 )) ){
				mskTmp = 1;
			}
			else{
				mskTmp = 0;
				src1[p] = 0;
			}

			dst[p] = dst[p] | ((mskTmp << index) << FP32_FWL);
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

	const int32_t* src = preprocessing_vmem_getDataAddress(sdSrc);
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);

	//Calculate window edges
	unsigned int jyl = max(0, -dy);			//Row
	unsigned int jyh = min(0, -dy) + rows; 	//Row
	unsigned int jxl = max(0, -dx); 		//Column
	unsigned int jxh = min(0, -dx) + cols; 	//Column

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

	const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
	const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);


	// Calculate window edges
	unsigned int jyl = max(0, -dy), jyh = min(0, -dy) + rows; // ROWS
	unsigned int jxl = max(0, -dx), jxh = min(0, -dx) + cols; // COLUMNS

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

	const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);
	const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);


	// Calculate window edges
	unsigned int jyl = max(0, -dy), jyh = min(0, -dy) + rows; // ROWS
	unsigned int jxl = max(0, -dx), jxh = min(0, -dx) + cols; // COLUMNS

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
			PREPROCESSING_DEF_CHECK_POINTER(src2, p, size);
			PREPROCESSING_DEF_CHECK_POINTER(dst, p, size);

			if(src2[p] > 1){
				dst[p] = eve_fp_divide32(src1[p], src2[p], FP32_FWL);
			}

			if (dst[p] == EVE_FP32_NAN){
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;

}

int preprocessing_arith_mean(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	int npix = 0;
	uint32_t sum2 = 0;
	uint32_t sum3 = 0;

	const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1);	//GTmp
	const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2);	//PixCnt
	int32_t* dst = preprocessing_vmem_getDataAddress(sdDst);			//Media Matrix

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

			if (src2[p]>0){
				sum2 = eve_fp_add32(sum2, src1[p]);
				sum3 = eve_fp_add32(sum2, eve_fp_multiply32(src1[p], src1[p], FP32_FWL));
				npix++;
			}
		}
	}

	dst[0] = eve_fp_divide32(sum2, eve_fp_int2s32(npix, FP32_FWL), FP32_FWL);			//return mean

	//Original ->  5*sqrt(sum3/npix-dst[0]*dst[0]);
	uint32_t fiveFixed = eve_fp_int2s32(5, FP32_FWL);
	uint32_t npixFixed = eve_fp_int2s32(npix, FP32_FWL);
	uint32_t sum3DividedByNpix = eve_fp_divide32(sum3, npixFixed, FP32_FWL);
	uint32_t pow2mean = eve_fp_multiply32(dst[0], dst[0], FP32_FWL);
	uint32_t divisionMinusPow = eve_fp_subtract32( sum3DividedByNpix,pow2mean );
	uint32_t sqrtOfDivisionMinusPow = eve_fp_double2s32( sqrt(eve_fp_signed32ToDouble( divisionMinusPow , FP32_FWL)), FP32_FWL);
	dst[1] = eve_fp_multiply32( fiveFixed, sqrtOfDivisionMinusPow, FP32_FWL);

	dst[2] = npixFixed;														//number of pixels
	dst[3] = sum2;//

	return status;
}

int preprocessing_arith_criba_fivesigma(){

	int status = PREPROCESSING_SUCCESSFUL;

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

int preprocessing_log10Image(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint32_t sdDst){

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

			//If the value is lower or equal to zero, do not modify it
            if (src[p] <= 0){
                dst[p] = src[p];
            }
            else{
                dst[p] = eve_fp_double2s32(
                        log10(eve_fp_signed32ToDouble(src[p], FP32_FWL)),
                        FP32_FWL);
            }
        }
    }

    return status;
}

int preprocessing_arith_doGetConst(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3, uint32_t sdSrc4,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, uint32_t sdDst1, uint32_t sdDst2){

	int status = PREPROCESSING_SUCCESSFUL;
	int stdimagesize = rows*cols;

	const int32_t* tmp1 = preprocessing_vmem_getDataAddress(sdTmp1);
	const int32_t* tmp2 = preprocessing_vmem_getDataAddress(sdTmp2);

	// Check whether given rows and columns are in a valid range.
	if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc3, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc4, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdDst1, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdDst2, rows, cols)))
	{
		return PREPROCESSING_INVALID_SIZE;
	}

	//Calculate ROIs of masks
	if((status = preprocessing_arith_ROI(sdSrc3 , rows, cols, -dx, -dy, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_ROI(sdSrc4 , rows, cols,  dx,  dy, sdTmp2)) != PREPROCESSING_SUCCESSFUL) return status;

	//Multiply ROIs to obtain mksDouble
	if((status = preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp3)) != PREPROCESSING_SUCCESSFUL) return status;

	if((status = preprocessing_zero(sdTmp1, ROWS, COLS, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_zero(sdTmp2, ROWS, COLS, sdTmp2)) != PREPROCESSING_SUCCESSFUL) return status;

	//Calculate ROI of images
	if((status = preprocessing_arith_ROI(sdSrc1 , rows, cols, -dx, -dy, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_ROI(sdSrc2 , rows, cols,  dx,  dy, sdTmp2)) != PREPROCESSING_SUCCESSFUL) return status;

	//Calculate Diff
	if((status = preprocessing_arith_subtractImages(sdTmp1, sdTmp2, rows, cols, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_multiplyImages(sdTmp1, sdTmp3, rows, cols, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;

	//Apply diff to const
	if((status = preprocessing_arith_addROI(sdDst1, sdTmp1, rows, cols, -dx, -dy, sdDst1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_substractROI(sdDst1, sdTmp1, rows, cols, dx, dy, sdDst1)) != PREPROCESSING_SUCCESSFUL) return status;

	//Apply mskDouble to pixCount
	if((status = preprocessing_arith_addROI(sdDst2, sdTmp3, rows, cols, -dx, -dy, sdDst2)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_addROI(sdDst2, sdTmp3, rows, cols,  dx,  dy, sdDst2)) != PREPROCESSING_SUCCESSFUL) return status;

	return status;
 }

//TODO Add disp (Dx, Dy)
int preprocessing_arith_iterate(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3,
		uint16_t rows, uint16_t cols, uint16_t loops, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;

	const int32_t* src1 = preprocessing_vmem_getDataAddress(sdSrc1); //Con
	const int32_t* src2 = preprocessing_vmem_getDataAddress(sdSrc2); //Mask of all images
	const int32_t* src3 = preprocessing_vmem_getDataAddress(sdSrc3); //PixCount
	int32_t* dst2 = preprocessing_vmem_getDataAddress(sdDst);		 //Gain

	// Check whether given rows and columns are in a valid range.
	if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc3, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdDst,  rows, cols)))
	{
		return PREPROCESSING_INVALID_SIZE;
	}

	for(uint16_t i = 0; i < loops; i++) {

		//doIteration(con, gain, tmp, pixCnt,disp,dimX, dimY);
	}

	return status;
}

int preprocessing_arith_doIterationTwoImages(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	// Check whether given rows and columns are in a valid range.
	if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc3, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp1, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp2, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp3, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
	{
		return PREPROCESSING_INVALID_SIZE;
	}

	//Calculate ROI masks
	if((status = preprocessing_arith_ROI(sdSrc2 , rows, cols, -dx, -dy, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_ROI(sdSrc3 , rows, cols,  dx,  dy, sdTmp2)) != PREPROCESSING_SUCCESSFUL) return status;

	//Calculate mskDouble
	if((status = preprocessing_arith_multiplyImages(sdTmp1, sdTmp2, rows, cols, sdTmp3)) != PREPROCESSING_SUCCESSFUL) return status;

	//Calculate ROI from gain
	if((status = preprocessing_arith_ROI(sdSrc1, rows, cols, -dx, -dy, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_ROI(sdSrc1, rows, cols,  dx,  dy, sdTmp2)) != PREPROCESSING_SUCCESSFUL) return status;

	//Modify GainTmp
	if((status = preprocessing_arith_multiplyImages(sdTmp1, sdTmp3, rows, cols, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_addROI(sdDst, sdTmp1, rows, cols, -dx, -dy, sdDst)) != PREPROCESSING_SUCCESSFUL) return status;

	if((status = preprocessing_arith_multiplyImages(sdTmp2, sdTmp3, rows, cols, sdTmp2)) != PREPROCESSING_SUCCESSFUL) return status;
	if((status = preprocessing_arith_addROI(sdDst, sdTmp2, rows, cols,  dx,  dy, sdDst)) != PREPROCESSING_SUCCESSFUL) return status;

	return status;
}


int preprocessing_arith_doIteration(uint32_t sdSrc1, uint32_t sdSrc2, uint32_t sdSrc3, uint32_t sdSrc4,
		uint32_t sdTmp1, uint32_t sdTmp2, uint32_t sdTmp3, uint32_t sdTmp4, uint32_t sdTmp5, uint32_t sdTmp6,
		uint16_t rows, uint16_t cols, int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;

	//TODO Change dispRows and dispCols value
	int dispRows = 9;
	int dispCols = 2;
	int no_of_image = 9;

	unsigned int sizeDisp = (unsigned int)(dispRows) * dispCols;
	unsigned int piq = 0;
	unsigned int pir = 0;

	const int32_t* src4 = preprocessing_vmem_getDataAddress(sdSrc4); //Disp

	// Check whether given rows and columns are in a valid range.
	if ((!preprocessing_vmem_isProcessingSizeValid(sdSrc1, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc2, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc3, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdSrc4, dispRows, dispCols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp1, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp2, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp3, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp4, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp5, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdTmp6, rows, cols))
			|| (!preprocessing_vmem_isProcessingSizeValid(sdDst, rows, cols)))
	{
		return PREPROCESSING_INVALID_SIZE;
	}

	//Creates a copy of Con
	preprocessing_arith_equalImages(sdSrc1, rows, cols, sdTmp1);

	for(unsigned short iq = 1; iq < no_of_image; iq++) {

		//Obtain iq mask
		preprocessing_getMask(sdSrc2, rows, cols, iq, sdTmp2);

		for(unsigned short ir = 0; ir < iq; ir++) {

			//Obtain ir mask
			preprocessing_getMask(sdSrc2, rows, cols, ir, sdTmp3);

			//Calculate relative offset
			piq = iq*dispCols;
			pir = ir*dispCols;

			// Check for valid pointer position.
			PREPROCESSING_DEF_CHECK_POINTER(src4, piq, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src4, piq+1, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src4, pir, sizeDisp);
			PREPROCESSING_DEF_CHECK_POINTER(src4, pir+1, sizeDisp);

			int16_t dx = src4[piq] - src4[pir];
			int16_t dy = src4[piq + 1] - src4[pir + 1];

			if((status = preprocessing_arith_doIterationTwoImages(sdDst, sdTmp2, sdTmp3, sdTmp4, sdTmp5, sdTmp6, rows, cols, dx, dy, sdTmp1)) != PREPROCESSING_SUCCESSFUL) return status;

		}
	}

	preprocessing_arith_equalImages(sdSrc3, rows, cols, sdTmp4);



	return status;
}
