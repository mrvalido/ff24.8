/*
 * mydef.h
 *
 *  Created on: 13 jul. 2017
 *      Author: zaca
 */

#ifndef LIBPREPROCESSING_PREPROCESSING_MYDEF_H_
#define LIBPREPROCESSING_PREPROCESSING_MYDEF_H_

#define ROWS 2048
#define COLS 2048

#define XC (ROWS/2) // center coordinate of CCD
#define YC (COLS/2) // center coordinate of CCD

#define WIDTH_RADIO 8 //ancho del aro del circulo en pixels

#define STEP_HOUGH 1.0
#define CENTER_DIST 100 //
#define STEP_RADIO 1
#define LMAX_ROWS WIDTH_RADIO/STEP_RADIO //Numbers o terna
#define LMAX_COLS 3 				//x, y ,maxvalues

#define NUMBER_OF_IMAGES 9

#define IMIN 	0 << FP32_FWL
#define IMAX  	82000 << FP32_FWL
#define LOOPS_ITERA 10

#define MASK_INDEX  	NUMBER_OF_IMAGES
#define MASK_TMP_INDEX	(MASK_INDEX + 1)
#define CONS_INDEX		(MASK_TMP_INDEX + 1)
#define PIXCOUNT_INDEX 	(CONS_INDEX + 1)
#define DISP_INDEX 		(PIXCOUNT_INDEX + 1)
#define DISP_ROWS   	NUMBER_OF_IMAGES
#define DISP_COLS		2

#define CHECK_STATUS(x) 	if((status = x ) != PREPROCESSING_SUCCESSFUL){ printf("Status Error\n");  return status;}


#endif /* LIBPREPROCESSING_PREPROCESSING_MYDEF_H_ */
