


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
	int32_t *tmp7;
	int32_t *tmp8;



	int32_t *min, *max;
	int32_t M=0;
	int32_t m=0;
	int32_t Scale;
	int32_t threshold=0;
	int32_t newThresh=0;
	double dif_threshold;
	Scale=eve_fp_int2s32(255, FP32_FWL );
	max=&M;
	min=&m;
	int *inputimg;
	int number_image=1;
	int numberOfImages = 9;
	uint16_t indice;
	uint32_t stdimagesize=ROWS*COLS;
	uint32_t histsize=256;
	uint32_t stdDispSize = DISP_ROWS*DISP_COLS;
	uint32_t stdMeanSize = 1;
	uint32_t numberOfMemoryInput = 16;

	int status = PREPROCESSING_SUCCESSFUL;

	FILE *fp;
	int nkeys;
	char **header;
	printf ("Start!\n");

	/*
	 * Memory allocation
	 * Corresponds to part of copying images to SDRAM, total size of virtual RAM
	 */
	SDRAM = (int32_t*) malloc(numberOfMemoryInput*stdimagesize*sizeof(int32_t));
	inputimg = (int*) malloc((uint32_t)stdimagesize*sizeof(int));			//Only one image

	printf("Load images in Virtual RAM!\n");

	/*
	for(unsigned int i = 0; i < number_image; i++) {
		//imageName[8] = 48 + i;//for "./im/im0X.fits" , "./hr/im0X.fits"; set
		FITS_getImage("rectangle100.fits", inputimg, stdimagesize, &nkeys, &header);
		for (int j = 0; j < stdimagesize; j++)
			//SDRAM[j]=(int32_t)eve_fp_int2s32(inputimg[j], FP32_FWL );
			SDRAM[j]=inputimg[j];

		FITS_getImage("rectangle200.fits", inputimg, stdimagesize, &nkeys, &header);
		for (int j = stdimagesize; j < stdimagesize*2; j++)
			//SDRAM[j]=(int32_t)eve_fp_int2s32(inputimg[j], FP32_FWL );
			SDRAM[j]=inputimg[j-stdimagesize];
	}

	 */
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

	uint32_t    dispSdram = img02Sdram + img02Size;
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

	uint32_t	tmp7Sdram = tmp6Sdram + tmp6Size;
	uint32_t	tmp7Size = stdimagesize;
	uint32_t	tmp7DatasetId = 14;

	uint32_t	tmp8Sdram = tmp7Sdram + tmp7Size;
	uint32_t	tmp8Size = stdimagesize;
	uint32_t	tmp8DatasetId = 15;

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
	tmp7=(SDRAM+tmp7Sdram);
	tmp8=(SDRAM+tmp8Sdram);

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
	preprocessing_vmem_setEntry(tmp7Sdram, tmp7Size, tmp7DatasetId, tmp7);
	preprocessing_vmem_setEntry(tmp8Sdram, tmp8Size, tmp8DatasetId, tmp8);


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
	//TODO - Poner log10 dentro de mask
	printf("Creating mask of all images\n");
	readNAND(entriesOfNAND[MASK_INDEX], ROWS, COLS, masksSdram);
	for(int i=0; i < NUMBER_OF_IMAGES; i++){
		readNAND(entriesOfNAND[i], ROWS, COLS, img01Sdram);
		preprocessing_arith_maskImages(img01Sdram, ROWS, COLS, i, IMIN, IMAX, masksSdram);
	    preprocessing_log10Image(img01Sdram, ROWS, COLS, img01Sdram);
	    writeNAND(img01Sdram, ROWS, COLS, entriesOfNAND[i]);
	}

	printf("Mask created successfully!\n");






	for(unsigned short iq = 1; iq < NUMBER_OF_IMAGES; iq++) {
		printf("--------------------------\n");
		printf("Calculate image %d with:\n", iq);
		printf("--------------------------\n");
		readNAND(entriesOfNAND[iq], ROWS, COLS, img01Sdram);

			for(unsigned short ir = 0; ir < iq; ir++) {
				printf("\t -Image %d\n", ir);

				readNAND(entriesOfNAND[ir], ROWS, COLS, img02Sdram);

				//Calculate Disp
				int dy = (disp[iq*DISP_COLS]   - disp[ir*DISP_COLS])/FP32_BINARY_TRUE;
				int dx = (disp[iq*DISP_COLS+1] - disp[ir*DISP_COLS+1])/FP32_BINARY_TRUE;

				//Calculate mask of each image
				if((status = preprocessing_getMask(masksSdram, ROWS, COLS, iq, tmp1Sdram) ) != PREPROCESSING_SUCCESSFUL) return status;
				if((status = preprocessing_getMask(masksSdram, ROWS, COLS, ir, tmp2Sdram) ) != PREPROCESSING_SUCCESSFUL) return status;

				if((status = preprocessing_arith_doGetConst(img01Sdram, img02Sdram, tmp1Sdram, tmp2Sdram, tmp3Sdram, tmp4Sdram, tmp5Sdram, ROWS, COLS, dx, dy, consSdram, pixConSdram) ) != PREPROCESSING_SUCCESSFUL) return status;

				preprocessing_zero(tmp1Sdram, ROWS, COLS, tmp1Sdram);
				preprocessing_zero(tmp2Sdram, ROWS, COLS, tmp2Sdram);
				preprocessing_zero(tmp3Sdram, ROWS, COLS, tmp3Sdram);
				preprocessing_zero(tmp4Sdram, ROWS, COLS, tmp4Sdram);
				preprocessing_zero(tmp5Sdram, ROWS, COLS, tmp5Sdram);
			}
	}


	writeImageToFile(cons, "const.fits", -1, 0, stdimagesize );
	writeImageToFile(pixCon, "pixCon.fits", -1, 0, stdimagesize );



/*
	printf("Cleaning tmps\n");
	preprocessing_zero(tmp1Sdram, rows, cols, tmp1Sdram);
	preprocessing_zero(tmp2Sdram, rows, cols, tmp2Sdram);
	preprocessing_zero(tmp3Sdram, rows, cols, tmp3Sdram);
	printf("After Cleaning tmps\n");

	unsigned int dx= 100;
	unsigned int dy= 100;

	printf("ROI\n");
	preprocessing_arith_ROI(img01Sdram, rows, cols, dx, dy, tmp1Sdram);
	preprocessing_arith_ROI(img02Sdram, rows, cols, -dx, -dy, tmp2Sdram);

	printf("Sum ROI\n");
	preprocessing_arith_addROI(tmp3Sdram, tmp1Sdram, rows, cols, dx, dy, tmp3Sdram);
	preprocessing_arith_addROI(tmp3Sdram, tmp2Sdram, rows, cols, -dx, -dy, tmp3Sdram);

	printf("After Sum ROI\n");
*/

	printf("Done!\n");
	return 1;

}
