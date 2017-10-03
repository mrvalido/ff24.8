/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Eve - a helpful library.
 *
 * Copyright (C) 2014, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains implementation of fixed point number conversions.
 */

#include "eve/fixed_point.h"

#include <stdlib.h>
#include <math.h>

/* PUBLIC IMPLEMENTATION *****************************************************/

double eve_fp_signed8ToDouble(int8_t fpVal, unsigned int fractionBits)
{
    return eve_fp_signedToDouble(fpVal, 8, fractionBits);
}

/*****************************************************************************/

double eve_fp_unsigned8ToDouble(uint8_t fpVal, unsigned int fractionBits)
{
    return eve_fp_unsignedToDouble(fpVal, 8, fractionBits);
}

/*****************************************************************************/
//
//GSEOS_EXPORT double GSEOS_STDCALL eve_fp_signed16ToDouble(int16_t fpVal,
//        unsigned int fractionBits)
//{
//    return eve_fp_signedToDouble(fpVal, 16, fractionBits);
//}
//
///*****************************************************************************/
//
//GSEOS_EXPORT double GSEOS_STDCALL eve_fp_unsigned16ToDouble(uint16_t fpVal,
//        unsigned int fractionBits)
//{
//    return eve_fp_unsignedToDouble(fpVal, 16, fractionBits);
//}

/*****************************************************************************/

double eve_fp_signed32ToDouble(int32_t fpVal, unsigned int fractionBits)
{
    return eve_fp_signedToDouble(fpVal, 32, fractionBits);
}

/*****************************************************************************/

double eve_fp_unsigned32ToDouble(uint32_t fpVal, unsigned int fractionBits)
{
    return eve_fp_unsignedToDouble(fpVal, 32, fractionBits);
}

/*****************************************************************************/

double eve_fp_signed64ToDouble(int64_t fpVal, unsigned int fractionBits)
{
    return eve_fp_signedToDouble(fpVal, 64, fractionBits);
}

/*****************************************************************************/

double eve_fp_unsigned64ToDouble(uint64_t fpVal, unsigned int fractionBits)
{
    return eve_fp_unsignedToDouble(fpVal, 64, fractionBits);
}

/*****************************************************************************/

double eve_fp_signedToDouble(int64_t fpVal, unsigned int bits,
        unsigned int fractionBits)
{
    // MSB is sign bit and cannot be a fraction bit.
    if (fractionBits >= bits)
    {
        fractionBits = bits - 1;
    }

    // Number of fractions bits shall not be greater than number of double type
    // fraction bits.
    if (fractionBits > 52)
    {
        fractionBits = 52; 
    }

    // MSB is sign bit and not considered in original size bit mask.
    if (bits > 64)
    {
        bits = 64;
    }

    // Ignore original sign bit in original size bit mask: 
    // (bits - 1) shifts to the left.
    int64_t bitMaskWhole = ((int64_t)(1) << (bits - 1)) - 1;
    int64_t whole = 0;
    
    int64_t bitMaskFrac = ((int64_t)(1) << fractionBits) - 1;
    int64_t fraction = fpVal & bitMaskFrac;
    
    int sign = fpVal < 0 ? -1 : 1;

    if (sign == 1)
    {
        whole = ((fpVal & bitMaskWhole) >> fractionBits) * sign;
    }
    else
    {
        whole = (((~fpVal & bitMaskWhole) >> fractionBits) + 1) * sign;
    }

    return (double)(whole) + (double)(fraction) / (double)(bitMaskFrac + 1);
}

/*****************************************************************************/

double eve_fp_unsignedToDouble(uint64_t fpVal, unsigned int bits,
        unsigned int fractionBits)
{
    // MSB can be a fraction bit.
    if (fractionBits > bits)
    {
        fractionBits = bits;
    }
    
    // Number of fractions bits shall not be greater than number of double type
    // fraction bits.
    if (fractionBits > 52)
    {
        fractionBits = 52; 
    }

    // For 64 bit only: MSB cannot be considered referring to bit shift for 
    // creating valid bit mask.
    if (bits > 63)
    {
        bits = 63;
    }

    uint64_t bitMaskWhole = ((uint64_t)(1) << bits) - 1;
    uint64_t whole = (fpVal & bitMaskWhole) >> fractionBits;
    
    uint64_t bitMaskFrac = ((uint64_t)(1) << fractionBits) - 1;
    uint64_t fraction = fpVal & bitMaskFrac;

    return (double)(whole) + (double)(fraction) / (double)(bitMaskFrac + 1);
}

/*****************************************************************************/
//
//GSEOS_EXPORT int16_t GSEOS_STDCALL eve_fp_double2s16(double value,
//		unsigned int fractionBits)
//{
//    int16_t result
//        = (int16_t)(fabs(value) * (1 << fractionBits));
//
//    if (value < 0)
//    {
//        result = (int16_t)(-result);
//    }
//
//    return result;
//}
//
///*****************************************************************************/
//
//GSEOS_EXPORT uint16_t GSEOS_STDCALL eve_fp_double2u16(double value,
//		unsigned int fractionBits)
//{
//    uint16_t result
//        = (uint16_t)(fabs(value) * (1 << fractionBits));
//
//    return result;
//}

/*****************************************************************************/

int32_t eve_fp_double2s32(double value, unsigned int fractionBits)
{
    int32_t result
        = (int32_t)(fabs(value) * (1 << fractionBits));

    if (value < 0)
    {
        result = (int32_t)(-result);
    }
    
    return result;
}

/*****************************************************************************/

uint32_t eve_fp_double2u32(double value, unsigned int fractionBits)
{
    uint32_t result
        = (uint32_t)(fabs(value) * (1 << fractionBits));

    return result;
}

/*****************************************************************************/

int32_t eve_fp_int2s32(int value, unsigned int fractionBits)
{
    int64_t safeFp = (int64_t)(value) << fractionBits;

    if ((safeFp >= EVE_FP32_MIN) && (safeFp <= EVE_FP32_MAX))
    {
        return (int32_t)(safeFp);
    }

    return EVE_FP32_NAN;
}

/*****************************************************************************/

int eve_fp_compare32(const void* fp1, const void* fp2)
{
    if (*((const int32_t*)(fp1)) < *((const int32_t*)(fp2)))
    {
        return -1;
    }
    else if (*((const int32_t*)(fp1)) > *((const int32_t*)(fp2)))
    {
        return 1;
    }

    return 0;
}
