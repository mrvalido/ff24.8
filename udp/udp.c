/*
 * udp.c
 *
 *  Created on: 02 feburary. 2018
 *      Author: dennis
 */

#include "udp.h"



/* PUBLIC IMPLEMENTATION *****************************************************/

void udp_createNANDFLASH(int32_t *NANDFLASH, int32_t **entriesOfNAND,
		int stdimagesize, int numberOfImages){

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
	uint32_t	maskTmp	 = maskNand + stdimagesize;
	uint32_t	cons	 = maskTmp  + stdimagesize;
	uint32_t	gain	 = cons 	+ stdimagesize;
	uint32_t	pixCount = gain 	+ stdimagesize;
	uint32_t	dispNand = pixCount + stdimagesize;

	entriesOfNAND[0]  = (NANDFLASH+img1Nand);
	entriesOfNAND[1]  = (NANDFLASH+img2Nand);
	entriesOfNAND[2]  = (NANDFLASH+img3Nand);
	entriesOfNAND[3]  = (NANDFLASH+img4Nand);
	entriesOfNAND[4]  = (NANDFLASH+img5Nand);
	entriesOfNAND[5]  = (NANDFLASH+img6Nand);
	entriesOfNAND[6]  = (NANDFLASH+img7Nand);
	entriesOfNAND[7]  = (NANDFLASH+img8Nand);
	entriesOfNAND[8]  = (NANDFLASH+img9Nand);
	entriesOfNAND[9]  = (NANDFLASH+maskNand);
	entriesOfNAND[10] = (NANDFLASH+maskTmp);
	entriesOfNAND[11] = (NANDFLASH+cons);
	entriesOfNAND[12] = (NANDFLASH+gain);
	entriesOfNAND[13] = (NANDFLASH+pixCount);
	entriesOfNAND[14] = (NANDFLASH+dispNand);

	//READ DISP
	int MAXCHAR = 1000;
	FILE *fp2;
	char str[MAXCHAR];
	char* filename = "im/disp.txt";

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

/*****************************************************************************/

int udp_loadImage(int32_t *nandSrc, uint16_t rows, uint16_t cols,
        uint32_t sdDst)
{
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

/*****************************************************************************/

int udp_storeImage(uint32_t sdSrc, uint16_t rows, uint16_t cols,
        int32_t *nandDst)
{
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

/*****************************************************************************/


int udp_getMask(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint16_t index, uint32_t sdDst)
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

int udp_maskImagesLog10(uint32_t sdSrc,
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

int udp_createROI(uint32_t sdSrc, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;
	unsigned int roiPoint = 0;

	//Calculate window edges
	unsigned int jyl = udp_max16(0, -dy);				//Row
	unsigned int jyh = udp_min16(0, -dy) + rows; 	//Row
	unsigned int jxl = udp_max16(0, -dx); 				//Column
	unsigned int jxh = udp_min16(0, -dx) + cols; 	//Column

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

int udp_addROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;
	unsigned int roiPoint = 0;

	// Calculate window edges
	unsigned int jyl = udp_max16(0, -dy), jyh = udp_min16(0, -dy) + rows; 	// ROWS
	unsigned int jxl = udp_max16(0, -dx), jxh = udp_min16(0, -dx) + cols; 		// COLUMNS

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

int udp_substractROI(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols,
		   int16_t dx, int16_t dy, uint32_t sdDst){

	int status = PREPROCESSING_SUCCESSFUL;
	unsigned int size = (unsigned int)(rows) * cols;
	unsigned int p = 0;
	unsigned int roiPoint = 0;

	// Calculate window edges
	unsigned int jyl = udp_max16(0, -dy), jyh = udp_min16(0, -dy) + rows; 	// ROWS
	unsigned int jxl = udp_max16(0, -dx), jxh = udp_min16(0, -dx) + cols; 		// COLUMNS

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

int udp_normalize(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst){

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

int udp_mean(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst){

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

int udp_fivesigma(uint32_t sdSrc, uint16_t rows, uint16_t cols, uint32_t mean, uint32_t fiveSigma, uint32_t sdDst){

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
	dst[1] = eve_fp_int2s32(npix, FP32_FWL);

	if (dst[0] == EVE_FP32_NAN || dst[1] == EVE_FP32_NAN){
		status = PREPROCESSING_INVALID_NUMBER;
	}

	return status;
}

int udp_flatfield(uint32_t sdSrc1, uint32_t sdSrc2, uint16_t rows, uint16_t cols, uint32_t sdDst){

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
				double n = pow(10.0, eve_fp_signed32ToDouble( src1[p] , FP32_FWL));
				dst[p] = udp_double2s32rounded( n, FP32_FWL);
			}

			if (dst[p] == EVE_FP32_NAN){
				status = PREPROCESSING_INVALID_NUMBER;
			}
		}
	}

	return status;
}

int16_t udp_max16(int16_t a, int16_t b){
	if(a >= b) return a;
	return b;
}

int16_t udp_min16(int16_t a, int16_t b){
	if(a <= b) return a;
	return b;
}

int32_t udp_double2s32rounded(double value, unsigned int fractionBits)
{
    int32_t result
        = (int32_t)(round(fabs(value) * (1 << fractionBits)));

    if (value < 0)
    {
        result = (int32_t)(-result);
    }

    return result;
}

