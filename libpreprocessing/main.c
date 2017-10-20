


#include "preprocessing/def.h"
#include "preprocessing/mydef.h"
#include "preprocessing/vmem.h"
#include "preprocessing/ana.h"
#include "preprocessing/arith.h"
#include "preprocessing/flatfield.h"



/* from libeve */
#include "../libeve/eve/fixed_point.h"

/* from std c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "FITS_Interface.h"

void writeImageToFile(int32_t *img, char *fileName, int positionIndex,  int index, uint32_t stdimagesize ){
	FILE *fp;
	char file[20];
	strcpy(file,fileName);

	if(positionIndex >= 0){
		if(index < 10){
			file[positionIndex] = 48 + index;
		}else{
			int decimal = index/10;
			int subDecimal = index-decimal*10;
			file[positionIndex-1] = 48 + decimal;
			file[positionIndex] = 48 + subDecimal;
		}
	}

	fp=fopen(file,"wb");
	fwrite(img,sizeof(int32_t),stdimagesize,fp);
	fclose(fp);

	printf("File %s written successfully!\n", file);
}


/*
 * * * * * * *
 * * MAIN  * *
 * * * * * * *
 */


int main()
{

	int32_t *SDRAM;
	int32_t *img01;
	int32_t *img02;
	int32_t *disp;
	int32_t *mean;
	int32_t *masks;
	int32_t *cons;
	int32_t *pixCon;
	int32_t *tmp1;
	int32_t *tmp2;
	int32_t *tmp3;
	int32_t *tmp4;
	int32_t *tmp5;
	int32_t *tmp6;

	uint32_t stdimagesize=ROWS*COLS;
	uint32_t stdDispSize = DISP_ROWS*DISP_COLS;
	uint32_t stdMeanSize = 1;
	uint32_t numberOfMemoryInput = 16;

	int status = PREPROCESSING_SUCCESSFUL;

	printf ("Start!\n");

	/*
	 * Memory allocation
	 * Corresponds to part of copying images to SDRAM, total size of virtual RAM
	 */
	SDRAM = (int32_t*) malloc(numberOfMemoryInput*stdimagesize*sizeof(int32_t));

	printf("Load images in Virtual RAM!\n");

	/*
	 * * * * * * * * *
	 * memory mapping*
	 * * * * * * * * *
	 */

	//	1.) Load image data to (virtual) SDRAM:
	uint32_t 	img01Sdram = 0;
	uint32_t 	img01Size = stdimagesize;
	uint32_t 	img01DatasetId = 1;

	uint32_t 	img02Sdram = img01Sdram + img01Size;
	uint32_t 	img02Size = stdimagesize;
	uint32_t 	img02DatasetId = 2;

	uint32_t  dispSdram = img02Sdram + img02Size;
	uint32_t	dispSize  = stdDispSize;
	uint32_t	dispDatasetId = 3;

	uint32_t	meanSdram = dispSdram + dispSize;
	uint32_t	meanSize  = stdMeanSize;
	uint32_t	meanDatasetId = 4;

	uint32_t	masksSdram = meanSdram + meanSize;
	uint32_t	masksSize = stdimagesize;
	uint32_t	masksDatasetId = 5;

	uint32_t	consSdram = masksSdram + masksSize;
	uint32_t	consSize = stdimagesize;
	uint32_t	consDatasetId = 6;

	uint32_t	pixConSdram = consSdram + consSize;
	uint32_t	pixConSize = stdimagesize;
	uint32_t	pixConDatasetId = 7;

	uint32_t	tmp1Sdram = pixConSdram + pixConSize;
	uint32_t	tmp1Size = stdimagesize;
	uint32_t	tmp1DatasetId = 8;

	uint32_t	tmp2Sdram = tmp1Sdram + tmp1Size;
	uint32_t	tmp2Size = stdimagesize;
	uint32_t	tmp2DatasetId = 9;

	uint32_t	tmp3Sdram = tmp2Sdram + tmp2Size;
	uint32_t	tmp3Size = stdimagesize;
	uint32_t	tmp3DatasetId = 10;

	uint32_t	tmp4Sdram = tmp3Sdram + tmp3Size;
	uint32_t	tmp4Size = stdimagesize;
	uint32_t	tmp4DatasetId = 11;

	uint32_t	tmp5Sdram = tmp4Sdram + tmp4Size;
	uint32_t	tmp5Size = stdimagesize;
	uint32_t	tmp5DatasetId = 12;

	uint32_t	tmp6Sdram = tmp5Sdram + tmp5Size;
	uint32_t	tmp6Size = stdimagesize;
	uint32_t	tmp6DatasetId = 13;

	img01=(SDRAM+img01Sdram);
	img02=(SDRAM+img02Sdram);
	disp=(SDRAM+dispSdram);
	mean=(SDRAM+meanSdram);
	masks=(SDRAM+masksSdram);
	cons=(SDRAM+consSdram);
	pixCon=(SDRAM+pixConSdram);
	tmp1=(SDRAM+tmp1Sdram);
	tmp2=(SDRAM+tmp2Sdram);
	tmp3=(SDRAM+tmp3Sdram);
	tmp4=(SDRAM+tmp4Sdram);
	tmp5=(SDRAM+tmp5Sdram);
	tmp6=(SDRAM+tmp6Sdram);

	preprocessing_vmem_setEntry(img01Sdram, img01Size, img01DatasetId, img01);
	preprocessing_vmem_setEntry(img02Sdram, img02Size, img02DatasetId, img02);

	preprocessing_vmem_setEntry(dispSdram, dispSize, dispDatasetId, disp);
	preprocessing_vmem_setEntry(meanSdram, meanSize, meanDatasetId, mean);
	preprocessing_vmem_setEntry(masksSdram, masksSize, masksDatasetId, masks);
	preprocessing_vmem_setEntry(consSdram, consSize, consDatasetId, cons);
	preprocessing_vmem_setEntry(pixConSdram, pixConSize, pixConDatasetId, pixCon);

	preprocessing_vmem_setEntry(tmp1Sdram, tmp1Size, tmp1DatasetId, tmp1);
	preprocessing_vmem_setEntry(tmp2Sdram, tmp2Size, tmp2DatasetId, tmp2);
	preprocessing_vmem_setEntry(tmp3Sdram, tmp3Size, tmp3DatasetId, tmp3);
	preprocessing_vmem_setEntry(tmp4Sdram, tmp4Size, tmp4DatasetId, tmp4);
	preprocessing_vmem_setEntry(tmp5Sdram, tmp5Size, tmp5DatasetId, tmp5);
	preprocessing_vmem_setEntry(tmp6Sdram, tmp6Size, tmp6DatasetId, tmp6);

	preprocessing_vmem_print();

	//NAND FLASH Memory
	int32_t *NANDFLASH;
	int32_t numberOfEntriesNAND = 128;
	int32_t **entriesOfNAND = (int32_t **) malloc(numberOfEntriesNAND*sizeof(int32_t *));
	NANDFLASH = (int32_t*) malloc(numberOfEntriesNAND*stdimagesize*sizeof(int32_t));

	createNANDFLASH(NANDFLASH, entriesOfNAND, stdimagesize, NUMBER_OF_IMAGES);
	//END NAND FLASH Memory


	printf("Read Disp from NAND to VRAM\n");
	readNAND(entriesOfNAND[DISP_INDEX], DISP_ROWS, DISP_COLS, dispSdram);

	//Create Mask of all images
	printf("Creating mask of all images\n");
	readNAND(entriesOfNAND[MASK_INDEX], ROWS, COLS, masksSdram);
	for(int i=0; i < NUMBER_OF_IMAGES; i++){
		readNAND(entriesOfNAND[i], ROWS, COLS, img01Sdram);
		CHECK_STATUS(preprocessing_arith_maskImagesLog10(img01Sdram, ROWS, COLS, i, IMIN, IMAX, masksSdram))
	    writeNAND(img01Sdram, ROWS, COLS, entriesOfNAND[i]);
	}

	printf("Mask created successfully!\n");


	//CONST
	printf("\n------------------------------------------------\n");
	printf("---------------Calculating Const---------------\n");
	printf("------------------------------------------------\n");
	unsigned int sizeDisp = DISP_ROWS * DISP_COLS;
	unsigned int piq = 0;
	unsigned int pir = 0;

	for(unsigned short iq = 1; iq < NUMBER_OF_IMAGES; iq++) {
		printf("--------------------------\n");
		printf("Calculate image %d with:\n", iq);
		printf("--------------------------\n");
		readNAND(entriesOfNAND[iq], ROWS, COLS, img01Sdram);

			for(unsigned short ir = 0; ir < iq; ir++) {
				printf("\t -Image %d\n", ir);

				readNAND(entriesOfNAND[ir], ROWS, COLS, img02Sdram);

				//Calculate point
				piq = iq*DISP_COLS;
				pir = ir*DISP_COLS;

				// Check for valid pointer position.
				PREPROCESSING_DEF_CHECK_POINTER(disp, piq, sizeDisp);
				PREPROCESSING_DEF_CHECK_POINTER(disp, piq+1, sizeDisp);
				PREPROCESSING_DEF_CHECK_POINTER(disp, pir, sizeDisp);
				PREPROCESSING_DEF_CHECK_POINTER(disp, pir+1, sizeDisp);

				//Calculate Disp
				int dy = (int)eve_fp_subtract32(disp[piq], disp[pir])/FP32_BINARY_TRUE;
				int dx = (int)eve_fp_subtract32(disp[piq + 1], disp[pir + 1])/FP32_BINARY_TRUE;

				//Calculate mask of each image
				CHECK_STATUS(preprocessing_getMask(masksSdram, ROWS, COLS, iq, tmp1Sdram) )
				CHECK_STATUS(preprocessing_getMask(masksSdram, ROWS, COLS, ir, tmp2Sdram) )

				CHECK_STATUS(preprocessing_arith_doGetConst(img01Sdram, img02Sdram, tmp1Sdram, tmp2Sdram, tmp3Sdram, tmp4Sdram, tmp5Sdram, ROWS, COLS, dx, dy, consSdram, pixConSdram) )

				CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp1Sdram))
				CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp2Sdram))
				CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp3Sdram))
				CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp4Sdram))
				CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp5Sdram))
			}
	}
	printf("\n------------------------------------------------\n");
	printf("---------Const calculates successfully---------\n");
	printf("------------------------------------------------\n");
	//END CONST

	CHECK_STATUS(preprocessing_arith_equalImages(consSdram, ROWS, COLS, tmp1Sdram))
	CHECK_STATUS(preprocessing_arith_normalicer(tmp1Sdram, pixConSdram, ROWS, COLS, tmp1Sdram))


	//ITERA
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp2Sdram))
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp3Sdram))
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp4Sdram))
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp5Sdram))
	CHECK_STATUS(preprocessing_zero(ROWS, COLS, tmp6Sdram))

	printf("\n------------------------------------------------\n");
	printf("-----------------Calculate Itera-----------------\n");
	printf("------------------------------------------------\n");
	CHECK_STATUS(preprocessing_arith_iterate(consSdram, masksSdram, pixConSdram, dispSdram,
			tmp2Sdram, tmp3Sdram, tmp4Sdram, tmp5Sdram, tmp6Sdram,
			ROWS, COLS, LOOPS_ITERA, tmp1Sdram))
	printf("\n------------------------------------------------\n");
	printf("----------Itera calculated successfully----------\n");
	printf("------------------------------------------------\n");

	//END ITERA

	writeImageToFile(cons, "const.fits", -1, 0, stdimagesize );
	writeImageToFile(pixCon, "pixCon.fits", -1, 0, stdimagesize );
	writeImageToFile(tmp1, "Gain.fits", -1, 0, stdimagesize );

	printf("Done!\n");
	return 1;

}
