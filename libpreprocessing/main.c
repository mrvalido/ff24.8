


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
	int32_t *img03;
	int32_t *img04;
	int32_t *img05;
	int32_t *img06;
	int32_t *img07;
	int32_t *img08;
	int32_t *img09;
	int32_t *centers;
	int32_t *mean;
	int32_t *tmp1;
	int32_t *tmp2;
	int32_t *tmp3;




	//*********************
	int32_t *NANDFLASH;


	//*********************



/*
	int32_t *med7;
	int32_t *rad8;
*/
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
	uint16_t rows=2048;
	uint16_t cols=2048;
	uint16_t indice;
	uint32_t stdimagesize=rows*cols;
	uint32_t histsize=256;
	uint32_t stdCentersSize = 256;
	uint32_t stdMeanSize = 1;
	uint32_t numberOfMemoryInput = 16;


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

	NANDFLASH = (int32_t*) malloc(numberOfMemoryInput*stdimagesize*sizeof(int32_t));



	printf("Load images in Virtual RAM!\n");

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


	printf("Load images in NAND FLASH!\n");

	int dispRows = 9;
	int dispCols = 2;
	int **disp = (int **)malloc(dispRows * sizeof(int*));
	for(int i = 0; i < dispRows; i++) disp[i] = (int *)malloc(dispCols * sizeof(int));

	int MAXCHAR = 1000;
	FILE *fp2;
	char str[MAXCHAR];
	char* filename = "disp.txt";

	fp2 = fopen(filename, "r");
	if (fp2 == NULL){
		printf("Could not open file %s",filename);
		return 1;
	}
	int indexRow = 0, indexCol = 0;

	while (fgets(str, MAXCHAR, fp2) != NULL){
		char *ch;
		ch = strtok(str, " ");
		while (ch != NULL) {
			if(indexCol == 2) indexCol = 0;
			disp[indexRow][indexCol] = atoi(ch);
			indexCol++;
			ch = strtok(NULL, " ,");
		}
		indexRow++;
	}
	fclose(fp2);

	char fileName[12] = "im/im00.fits";

	for(unsigned int i = 0; i < 9; i++) {
		fileName[6] = 48 + i;
		printf("%s\n", fileName);
		FITS_getImage(fileName, inputimg, stdimagesize, &nkeys, &header);
		for (int j = 0; j < stdimagesize; j++)
			SDRAM[i*stdimagesize + j]=(int32_t)eve_fp_int2s32(inputimg[j], FP32_FWL );
	}

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
	//
	uint32_t	img03Sdram = img02Sdram + img02Size;
	uint32_t	img03Size = stdimagesize;
	uint32_t	img03DatasetId = 3;

	uint32_t	img04Sdram = img03Sdram + img03Size;
	uint32_t	img04Size = stdimagesize;
	uint32_t	img04DatasetId = 4;

	uint32_t	img05Sdram = img04Sdram + img04Size;
	uint32_t	img05Size = stdimagesize;
	uint32_t	img05DatasetId = 5;

	uint32_t	img06Sdram = img05Sdram + img05Size;
	uint32_t	img06Size = stdimagesize;
	uint32_t	img06DatasetId = 6;

	uint32_t	img07Sdram = img06Sdram + img06Size;
	uint32_t	img07Size = stdimagesize;
	uint32_t	img07DatasetId = 7;

	uint32_t	img08Sdram = img07Sdram + img07Size;
	uint32_t	img08Size = stdimagesize;
	uint32_t	img08DatasetId = 8;

	uint32_t	img09Sdram = img08Sdram + img08Size;
	uint32_t	img09Size = stdimagesize;
	uint32_t	img09DatasetId = 9;

	uint32_t    centersSdram = img09Sdram + img09Size;
	uint32_t	centersSize  = stdCentersSize;
	uint32_t	centersDatasetId = 10;

	uint32_t	meanSdram = centersSdram + centersSize;
	uint32_t	meanSize  = stdMeanSize;
	uint32_t	meanDatasetId = 11;

	uint32_t	tmp1Sdram = meanSdram + meanSize;
	uint32_t	tmp1Size = stdimagesize;
	uint32_t	tmp1DatasetId = 12;

	uint32_t	tmp2Sdram = tmp1Sdram + tmp1Size;
	uint32_t	tmp2Size = stdimagesize;
	uint32_t	tmp2DatasetId = 13;

	uint32_t	tmp3Sdram = tmp2Sdram + tmp2Size;
	uint32_t	tmp3Size = stdimagesize;
	uint32_t	tmp3DatasetId = 14;


	img01=(SDRAM+img01Sdram);
	img02=(SDRAM+img02Sdram);
	img03=(SDRAM+img03Sdram);
	img04=(SDRAM+img04Sdram);
	img05=(SDRAM+img05Sdram);
	img06=(SDRAM+img06Sdram);
	img07=(SDRAM+img07Sdram);
	img08=(SDRAM+img08Sdram);
	img09=(SDRAM+img09Sdram);
	centers=(SDRAM+centersSdram);
	mean=(SDRAM+meanSdram);
	tmp1=(SDRAM+tmp1Sdram);
	tmp2=(SDRAM+tmp2Sdram);
	tmp3=(SDRAM+tmp3Sdram);

	preprocessing_vmem_setEntry(img01Sdram, img01Size, img01DatasetId, img01);
	preprocessing_vmem_setEntry(img02Sdram, img02Size, img02DatasetId, img02);
	preprocessing_vmem_setEntry(img03Sdram, img03Size, img03DatasetId, img03);
	preprocessing_vmem_setEntry(img04Sdram, img04Size, img04DatasetId, img04);
	preprocessing_vmem_setEntry(img05Sdram, img05Size, img05DatasetId, img05);
	preprocessing_vmem_setEntry(img06Sdram, img06Size, img06DatasetId, img06);
	preprocessing_vmem_setEntry(img07Sdram, img07Size, img07DatasetId, img07);
	preprocessing_vmem_setEntry(img08Sdram, img08Size, img08DatasetId, img08);
	preprocessing_vmem_setEntry(img09Sdram, img09Size, img09DatasetId, img09);

	preprocessing_vmem_setEntry(centersSdram, centersSize, centersDatasetId, centers);
	preprocessing_vmem_setEntry(meanSdram, meanSize, meanDatasetId, mean);

	preprocessing_vmem_setEntry(tmp1Sdram, tmp1Size, tmp1DatasetId, tmp1);
	preprocessing_vmem_setEntry(tmp2Sdram, tmp2Size, tmp2DatasetId, tmp2);
	preprocessing_vmem_setEntry(tmp3Sdram, tmp3Size, tmp3DatasetId, tmp3);



	preprocessing_vmem_print();

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


	//for debug
	printf("Write to binary image to File finished!\n");
	fp=fopen("SumRoi.fits","wb");
	fwrite(tmp3,sizeof(int32_t),stdimagesize,fp);
	fclose(fp);
	//for debug end


	printf("Done!\n");
	return 1;

}
