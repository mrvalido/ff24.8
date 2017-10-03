/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Eve - a helpful library.
 *
 * Copyright (C) 2014, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains declarations of fixed point number conversions.
 */

#ifndef EVE_FIXED_POINT_H
#define EVE_FIXED_POINT_H

//#include <stddef.h>
//#include <gseosdll.h>

#ifndef GSEOS
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#endif

/**
 * These values describe the minimum, maximum and NAN of the 24.8 fixed point
 * representation. They are adjusted to signed integer 32 because 24.8 fixed
 * point is represented in 2's complement.
 * @{
 */
#define EVE_FP32_MAX INT32_MAX
#define EVE_FP32_MIN (INT32_MIN + 1)
#define EVE_FP32_NAN INT32_MIN
/**
 * }@
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Calculate decimal number from 8 bit signed fixed point number.
     *
     * @param fpVal 8 bit signed fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_signed8ToDouble(int8_t fpVal, unsigned int fractionBits);

    /**
     * Calculate decimal number from 8 bit unsigned fixed point number.
     *
     * @param fpVal 8 bit unsigned fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_unsigned8ToDouble(uint8_t fpVal, unsigned int fractionBits);

//    /**
//     * Calculate decimal number from 16 bit signed fixed point number.
//     *
//     * @param fpVal 16 bit signed fixed point number.
//     * @param fractionBits number of bits that hold the fractional part.
//     */
//    GSEOS_EXPORT double GSEOS_STDCALL eve_fp_signed16ToDouble(
//            int16_t fpVal, unsigned int fractionBits);
//
//    /**
//     * Calculate decimal number from 16 bit unsigned fixed point number.
//     *
//     * @param fpVal 16 bit unsigned fixed point number.
//     * @param fractionBits number of bits that hold the fractional part.
//     */
//    GSEOS_EXPORT double GSEOS_STDCALL eve_fp_unsigned16ToDouble(
//            uint16_t fpVal, unsigned int fractionBits);

    /**
     * Calculate decimal number from 32 bit signed fixed point number.
     *
     * @param fpVal 32 bit signed fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_signed32ToDouble(int32_t fpVal, unsigned int fractionBits);

    /**
     * Calculate decimal number from 32 bit unsigned fixed point number.
     *
     * @param fpVal 32 bit unsigned fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_unsigned32ToDouble(uint32_t fpVal,
            unsigned int fractionBits);

    /**
     * Calculate decimal number from 64 bit signed fixed point number.
     *
     * @param fpVal 64 bit signed fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_signed64ToDouble(int64_t fpVal, unsigned int fractionBits);

    /**
     * Calculate decimal number from 64 bit unsigned fixed point number.
     *
     * @param fpVal 64 bit unsigned fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_unsigned64ToDouble(uint64_t fpVal,
            unsigned int fractionBits);

    /**
     * Calculate decimal number from 64 bit signed fixed point number. Shorter
     * signed fixed pointer numbers are cast to 64 bit.
     *
     * @param fpVal 64 bit signed fixed point number.
     * @param bits original bit size of unsigned fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_signedToDouble(int64_t fpVal, unsigned int bits,
            unsigned int fractionBits);

    /**
     * Calculate decimal number from 64 bit unsigned fixed point number. 
     * Shorter unsigned fixed pointer numbers are cast to 64 bit.
     *
     * @param fpVal 64 bit unsigned fixed point number.
     * @param bits original bit size of unsigned fixed point number.
     * @param fractionBits number of bits that hold the fractional part.
     */
    double eve_fp_unsignedToDouble(uint64_t fpVal, unsigned int bits,
            unsigned int fractionBits);

//    /**
//     * Convert a double-precision floating point number into an unsigned short
//     * with the given precision.
//     *
//     * @param value the value to convert.
//     * @param fractionBits the number of bits reserved for the fraction part.
//     */
//    GSEOS_EXPORT int16_t GSEOS_STDCALL eve_fp_double2s16(double value,
//		    unsigned int fractionBits);
//    GSEOS_EXPORT uint16_t GSEOS_STDCALL eve_fp_double2u16(double value,
//		    unsigned int fractionBits);

    /**
     * Convert a double-precision floating point number into an unsigned
     * integer with the given precision.
     *
     * @param value the value to convert.
     * @param fractionBits the number of bits reserved for the fraction part.
     */
    int32_t eve_fp_double2s32(double value, unsigned int fractionBits);
    uint32_t eve_fp_double2u32(double value, unsigned int fractionBits);

    /**
     * Convert an integer number to a 32 bit fixed point number.
     *
     * @param value the value to convert.
     * @param fractionBits the number of bits reserved for the fraction part.
     */
    int32_t eve_fp_int2s32(int value, unsigned int fractionBits);

    /**
     * Compare two 32 bit fixed point numbers against each other.
     * Note: Input parameters are pointers so this function is compatible with
     *       qsort.
     *
     * @param fp1 pointer to fixed point number.
     * @param fp2 pointer to fixed point number.
     *
     * @return -1 if *fp1 < *fp2, 0 if *fp1 == *fp2, 1 if *fp1 > *fp2.
     */
    int eve_fp_compare32(const void* fp1, const void* fp2);

    /**
     * Calculate sum of two 32 bit fixed point numbers and detect a possible
     * overflow or underflow, respectively.
     *
     * @param a the first summand. 
     * @param b the second summand.
     *
     * @return sum on success, EVE_FP32_MIN on overflow or underflow.
     */
    static inline int32_t eve_fp_add32(int32_t a, int32_t b)
    {
        int64_t safeSum = (int64_t)(a) + b;

        // Return sums in integer 32 range only. 
        if ((safeSum >= EVE_FP32_MIN) && (safeSum <= EVE_FP32_MAX))
        {
            return (int32_t)(safeSum);
        }

        return EVE_FP32_NAN;
    };

    /**
     * Calculate subtraction of two 32 bit fixed point numbers and detect a
     * possible overflow or underflow, respectively.
     *
     * @param a the minuend. 
     * @param b the subtrahend.
     *
     * @return difference on success, EVE_FP32_NAN on overflow or underflow.
     */
    static inline int32_t eve_fp_subtract32(int32_t a, int32_t b)
    {
        int64_t safeDiff = (int64_t)(a) - b;

        // Return differences in integer 32 range only. 
        if ((safeDiff >= EVE_FP32_MIN) && (safeDiff <= EVE_FP32_MAX))
        {
            return (int32_t)(safeDiff);
        }

        return EVE_FP32_NAN;
    };

    /**
     * Calculate multiplication of two 32 bit fixed point numbers and detect a
     * possible overflow or underflow, respectively.
     *
     * @param a   the multiplier. 
     * @param b   the multiplicand.
     * @param fwl the fractional part word length (number of bits).
     *
     * @return product on success, EVE_FP32_NAN on overflow or underflow.
     */
    static inline int32_t eve_fp_multiply32(int32_t a, int32_t b,
            unsigned int fwl)
    {
        if (fwl >= 31)
        {
            return EVE_FP32_NAN;
        }

        int64_t safeProd = ((int64_t)(a) * b) >> fwl;

        // Return products in integer 32 range only. 
        if ((safeProd >= EVE_FP32_MIN) && (safeProd <= EVE_FP32_MAX))
        {
            return (int32_t)(safeProd);
        }

        return EVE_FP32_NAN;
    };

    /**
     * Calculate division of two 32 bit fixed point numbers.
     *
     * @param a   the dividend. 
     * @param b   the divisor.
     * @param fwl the fractional part word length (number of bits).
     *
     * @return quotient on success, EVE_FP32_NAN on divisor of 0.
     */
    static inline int32_t eve_fp_divide32(int32_t a, int32_t b,
            unsigned int fwl)
    {
        unsigned int exceededBits = 0;

        // Division by 0 is not allowed.
        if ((b == 0) || (fwl >= 31))
        {
            return EVE_FP32_NAN;
        }

        int64_t safeQuot = (int64_t)(a);

        // Consider 2's complement representation.
        if (a < 0)
        {
            safeQuot = (~safeQuot) + 1;
        }

        // Determine lost fraction bits by checking how many bits are out of
        // range.
        for (unsigned int n = 0; n < fwl; n++)
        {
            if (((safeQuot >> (31 - fwl)) & (1 << n)) >> n)
            {
                exceededBits = n + 1;
            }
        }
        
        // Shift by left bits in range before divison to preserve highest
        // precision possible.
        safeQuot = ((safeQuot << (fwl - exceededBits)) / b) << exceededBits;

        // Consider 2's complement representation.
        if (a < 0)
        {
            safeQuot = (~safeQuot) + 1;
        }

        return (int32_t)(safeQuot);
    };

#ifdef __cplusplus
}
#endif

#endif /* EVE_FIXED_POINT_H */
