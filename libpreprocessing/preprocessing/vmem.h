/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Preprocessing - a library for image pre-processing.
 *
 * Copyright (C) 2016, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains declarations of the virtual SDRAM map for
 * pre-processing operations.
 */

#ifndef PREPROCESSING_VMEM_H
#define PREPROCESSING_VMEM_H

//#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h> //added for gcc  compatibility
/**
 * This is the maximum number of entries in the virtual SDRAM map.
 */
#define PREPROCESSING_VMEM_MAX_ENTRIES 16

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Delete all entries of the virtual SDRAM map.
     *
     * @return 0 on success, -1 on failure.
     */
    int preprocessing_vmem_deleteAll(void);

    /**
     * Set an entry in the virtual SDRAM map /a vmem_memoryMap after the given
     * parameters have been verified.
     *
     * @param sdram     the virtual SDRAM data address.
     * @param size      the size of the data in bytes.
     * @param datasetId the identifier of the dataset the data belongs to.
     * @param data      the pointer to the real data.
     *
     * @return 0 on success, -1 otherwise.
     */
    int preprocessing_vmem_setEntry(uint32_t sdram, uint32_t size,
            uint32_t datasetId, void* data);

    /**
     * Delete an existing entry of the virtual SDRAM map.
     *
     * @param sdram the virtual SDRAM data address of the desired entry.
     *
     * @return 0 on success, -1 otherwise.
     */
    int preprocessing_vmem_deleteEntry(uint32_t sdram);

    /**
     * Get the size of an existing entry in the virtual SDRAM map.
     *
     * @param sdram the virtual SDRAM address.
     * 
     * @return the related size on success, 0xFFFFFFFF on failure.
     */
    uint32_t preprocessing_vmem_getSize(uint32_t sdram);

    /**
     * Get the dataset ID of an existing entry in the virtual SDRAM map.
     *
     * @param sdram the virtual SDRAM address.
     * 
     * @return the related dataset ID on success, 0xFFFFFFFF on failure.
     */
    uint32_t preprocessing_vmem_getDatasetId(uint32_t sdram);

    /**
     * Get the real data address of an existing entry in the virtual SDRAM map.
     * Important note: Always check if the result pointer is 0 before further
     * usage.
     *
     * @param sdram the virtual SDRAM address.
     * 
     * @return the pointer to the data on success, 0 on failure.
     */
    void* preprocessing_vmem_getDataAddress(uint32_t sdram);

    /**
     * Check whether given rows and cols are inside a valid range considering
     * the size of the related virtual SDRAM map.
     *
     * @param sdram the virtual SDRAM address.
     * @param rows  the number of rows to process in the operation.
     * @param cols  the number of columns to process in the operation.
     *
     * @return true if rows * cols <= size, false otherwise.
     */
    bool preprocessing_vmem_isProcessingSizeValid(uint32_t sdram,
            uint16_t rows, uint16_t cols);

    /**
     * Print the content of the virtual SDRAM map /a vmem_memoryMap on the
     * console.
     */
    void preprocessing_vmem_print(void);

#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSING_VMEM_H */
