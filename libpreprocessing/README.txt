#-- OVERVIEW --#

This is the SO/PHI pre-processing library that serves as a back-up solution to
the RFPGA configuration and therefore, emulates the RFPGA interface. It is a
collection of image processing functions used to analyze onboard image data.

The pre-processing library used fixed point 24.8 as image data type. Since the
handling of this data type is done by SO/PHI libeve inside the DPU S/W, the
pre-processing library also make use of it and therefore, depends on it. This
means:

1.) libpreprocessing can only be used together with libeve
2.) Be sure to provide fixed point 24.8 image data to obtain valid results


#-- GENERAL INSTRUCTIONS --#

The header files in the subfolder "preprocessing" should be included to gain
the full access to all provided pre-processing functions:

- "preprocessing/ana.h"
- "preprocessing/arith.h"
- "preprocessing/complex.h"
- "preprocessing/def.h"
- "preprocessing/fft.h"
- "preprocessing/fit.h"
- "preprocessing/hough.h"
- "preprocessing/vmem.h"

The pre-processing library provides its own return status codes that are
defined in "preprocessing/def.h":

- PREPROCESSING_SUCCESSFUL
- PREPROCESSING_INVALID_ADDRESS
- PREPROCESSING_INVALID_NUMBER
- PREPROCESSING_INVALID_SIZE
- PREPROCESSING_NO_MEMORY

These should be checked when a function is called to know whether it returned
successfully or not.


#-- INPUT DATA HANDLING --#

The pre-processing library emulates the pre-processing RFPGA configuration
(including the SDRAM) interface and therefore, image data must be handled by
the virtual SDRAM in "preprocessing/vmem.h" before a pre-processing function
can be used.

1.) Load image data to (virtual) SDRAM:
img1Sdram = 0;
img1Size = 2048 * 2048;
img1DatasetId = 1;
img2Sdram = img1Sdram + img1Size;
img2Size = 2048 * 2048;
img2DatasetId = 2;
img3Sdram = img2Sdram + img2Size;
img3Size = 2048* 2048;
img3DatasetId = 3;
preprocessing_vmem_setEntry(img1Sdram, img1Size, img1DatasetId, &img1);
preprocessing_vmem_setEntry(img2Sdram, img2Size, img2DatasetId, &img2);
preprocessing_vmem_setEntry(img3Sdram, img3Size, img3DatasetId, &img3);

Virtual SDRAM entries are created for input data 1, input data 2 and the result
data. The first parameter of the function is the address in the virtual SDRAM
at which the image data should be loaded. This will be used in the
pre-processing functions to access the input data. The second parameter is the
size of the input data in pixel. Be aware that the virtual address space from
the chosen address (parameter 1) plus the size will be reserved for the image
data. So, any other image data must be loaded to virtual SDRAM address + size
of last image data. The third parameter is the dataset ID that can be used to
identify the dataset to which the input data belongs to. The forth parameter is
the pointer to the actual input data.

2.) Process image data:
preprocessing_arith_addImages(img1Sdram, img2Sdram, rows, columns, img3Sdram);

The result of the processing function is stored directly to the memory
location that has been connected with virtual SDRAM entry img3Sdram.
