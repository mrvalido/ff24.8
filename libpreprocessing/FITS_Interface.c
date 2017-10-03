/*
 * FITS_Interface.c
 *
 *  Created on: Jul 4, 2016
 *      Author: albert
 */



#include "fitsio.h"

/*
 * Inputs:
 * char *path - path to file to be read
 * int **array - 2D array that will hold the image
 * int imagesize - size of the image along one axis (!!!ATTENTION!!!)
 */
//int FITS_getImage(char *path, int **array, int imagesize, int *nkeys, char ***cardso)
//{
//	/*
//	 * FITSIO related declarations
//	 */
//	int status = 0;  /* MUST initialize status */
//	fitsfile *imageToProc;
//	char**cards;
//
//	/*
//	 * Image related declarations
//	 */
//	int *imageToProcDataRow;
//
//	/*
//	 * Processing related declarations
//	 */
//	int i,j;
//	int keysn;
//
//	/*
//	 * Memory allocation
//	 */
//	imageToProcDataRow = (int*) malloc(imagesize*imagesize*sizeof(int));
//
//	/*
//	 * Init arrays
//	 */
//	for (i=0;i<imagesize;i++)
//	{
//		for (j=0;j<imagesize;j ++)
//		{
//			imageToProcDataRow[i*imagesize+j]=0;
//		}
//
//	}
//
//	/*
//	 * Read image
//	 */
//	fits_open_file(&imageToProc, path+0, READONLY, &status);
//	fits_read_img(imageToProc, TINT, 1, imagesize*imagesize, NULL, imageToProcDataRow, NULL, &status);
//
//	/*
//	 * Get size of header & allocate memory for it
//	 */
//	fits_get_hdrspace(imageToProc, &keysn, NULL, &status);
//
//	cards = (char**) malloc(keysn*sizeof(char*));
//
//	for (i = 0; i < keysn; i++)
//	{
//		cards[i] = (char*) malloc(80*sizeof(char));
//	}
//
//	/*
//	 * Get header cards
//	 */
//	for (i = 0; i < keysn; i++) {
//		fits_read_record(imageToProc, i+1, cards[i], &status);
//	}
//
//	*nkeys = keysn;
//	*cardso = cards;
//
//	/*
//	 * Arrange image into 2D array
//	 */
//	for (i=0;i<imagesize;i++)
//	{
//		for (j=0;j<imagesize;j++)
//		{
//			array[i][j] = imageToProcDataRow[i*imagesize+j];
//		}
//	}
//
//	/*
//	 * Close file, free memory
//	 */
//	free(imageToProcDataRow);
//
//	return status;
//}

int FITS_getImage(char *path, int *array, int imagesize, int *nkeys, char ***cardso)
{
	/*
	 * FITSIO related declarations
	 */
	int status = 0;  /* MUST initialize status */
	fitsfile *imageToProc;
	char**cards;

	/*
	 * Image related declarations
	 */
	int *imageToProcDataRow;

	/*
	 * Processing related declarations
	 */
	int i,j;
	int keysn;

	/*
	 * Memory allocation
	 */


	/*
	 * Read image
	 */
	fits_open_file(&imageToProc, path+0, READONLY, &status);
	fits_read_img(imageToProc, TINT, 1, imagesize, NULL, array, NULL, &status);


	/*
	 * Get size of header & allocate memory for it
	 */
	fits_get_hdrspace(imageToProc, &keysn, NULL, &status);

	cards = (char**) malloc(keysn*sizeof(char*));

	for (i = 0; i < keysn; i++)
	{
		cards[i] = (char*) malloc(80*sizeof(char));
	}

	/*
	 * Get header cards
	 */
	for (i = 0; i < keysn; i++) {
		fits_read_record(imageToProc, i+1, cards[i], &status);
	}

	*nkeys = keysn;
	*cardso = cards;


	/*
	 * Close file, free memory
	 */


	return status;
}
/*
 * Inputs:
 * int **array - 2D array that holds the image
 * char *path - path where the file will be saved
 * int imagesize - size of the image along one axis (!!!ATTENTION!!!)
 * char *pathForHeader - !!!TEMPORARY SOLUTION!!! - A file that we use to make the header of the new fits.
 */
int FITS_saveImage(int32_t **array, char *path, int imagesizex, int imagesizey, int nkeys, char ***cards)
{
	fitsfile *fptr; /* pointer to the FITS file; defined in fitsio.h */
	int status;
	long fpixel = 1, naxis = 2, nelements;
	long naxes[2] = { imagesizey, imagesizex };
	status = 0; /* initialize status before calling fitsio routines */
	int i;
	printf("Pipeline finished 1!\n");
	/*
	 * delete previous image
	 */
	status = remove(path);


	/*
	 * Create fits and image
	 */
	fits_create_file(&fptr, path, &status); /* create new file */
	/* Create the primary array image - must be LONG_IMG, as there is no INT_IMG */
	fits_create_img(fptr, LONG_IMG, naxis, naxes, &status);
	printf("Pipeline finished2222!\n");
	/*
	 * Write header of image
	 */


	for (i = 0; i < nkeys; i++)
	{
		fits_write_record(fptr, (*cards)[i], &status);
	}


	/*
	 * Write image
	 */
	nelements = naxes[0] * naxes[1]; /* number of pixels to write */

	/* Write the array of integers to the image */

	fits_write_img(fptr, TINT, fpixel, nelements, array[0], &status);

	fits_close_file(fptr, &status); /* close the file */
	fits_report_error(stderr, status); /* print out any error messages */
	return status;
}
