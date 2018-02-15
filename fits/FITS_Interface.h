/*
 * FITS_Interface.h
 *
 *  Created on: Jul 4, 2016
 *      Author: albert
 */

#ifndef FITS_INTERFACE_H_
#define FITS_INTERFACE_H_



#endif /* FITS_INTERFACE_H_ */


/*
 * Inputs:
 * char *path - path to file to be read
 * int **array - 2D array that will hold the image
 * int imagesize - size of the image along one axis (!!!ATTENTION!!!)
 */
//int FITS_getImage(char *path, int **array, int imagesize, int *nkeys, char **cards);
int FITS_getImage(char *path, int *array, int imagesize, int *nkeys, char ***cardso);

/*
 * Inputs:
 * int **array - 2D array that holds the image
 * char *path - path where the file will be saved
 * int imagesize - size of the image along one axis (!!!ATTENTION!!!)
 * char *pathForHeader - !!!TEMPORARY SOLUTION!!! - A file that we use to make the header of the new fits.
 */
int FITS_saveImage(int32_t **array, char *path, int imagesizex, int imagesizey, int nkeys, char ***cards);

//int FITS_getImageForHeaderUpdate(char*path, fitsfile *fptr);
//int FITS_saveImageAfterHeaderUpdate(fitsfile *fptr, char*path);
