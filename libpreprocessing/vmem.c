/*
 * Project: Solar Orbiter - PHI (SO/PHI)
 * Module:  Preprocessing - a library for image pre-processing.
 *
 * Copyright (C) 2016, Max Planck Institute for Solar System Research
 */

/**
 * @file
 *
 * This file contains implementation of the virtual SDRAM map for
 * pre-processing operations.
 */

#include "preprocessing/vmem.h"

#include "preprocessing/def.h"

/* from stdc */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h> //added for gcc  compatibility

/* PRIVATE INTERFACE *********************************************************/

/**
 * This structure describes the parameters that are necessary to map a data
 * address to a virtual SDRAM address in order to simulate the SDRAM access
 * manner.
 */
struct vmem_MemoryMap
{
    /**
     * This is the virtual data address in SDRAM.
     */
    uint32_t sdram;

    /**
     * This is the size of the data in bytes.
     */
    uint32_t size;

    /**
     * This is the identifier of the dataset to which the data belongs to.
     */
    uint32_t datasetId;

    /**
     * This is the real data address that is meant to be mapped to a virtual
     * data address in SDRAM.
     */
    void* data;
}
vmem_memoryMap[PREPROCESSING_VMEM_MAX_ENTRIES] =
{
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

/**
 * Get the index of an existing entry in the virtual SDRAM map
 * \a vmem_memoryMap.
 *
 * @param sdram the virtual SDRAM address.
 * 
 * @return the index on success, PREPROCESSING_VMEM_MAX_ENTRIES on failure.
 */
static unsigned int vmem_getIndex(uint32_t sdram);

/* PUBLIC IMPLEMENTATION *****************************************************/

int preprocessing_vmem_deleteAll(void)
{
    memset(&vmem_memoryMap[0], 0, sizeof(vmem_memoryMap));
    //preprocessing_vmem_print();

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

int preprocessing_vmem_setEntry(uint32_t sdram, uint32_t size,
        uint32_t datasetId, void* data)
{
    unsigned int index = PREPROCESSING_VMEM_MAX_ENTRIES;

    if (data == 0)
    {
        printf("Invalid data pointer.\n");
        return -PREPROCESSING_INVALID_ADDRESS;
    }

    // Do not allow data addresses that are not divisible by four on the Leon3.
    if ((uintptr_t)(data) % 4 != 0)
    {
        printf("Real data addresses not divisible by 4 are not allowed: "
                "%p.\n", data);
        return -PREPROCESSING_INVALID_ADDRESS;
    }

    // Find the next empty entry in the memory map.
    for (unsigned int i = 0; i < PREPROCESSING_VMEM_MAX_ENTRIES; i++)
    {
        // An empty entry contains a data address of 0.
        if ((vmem_memoryMap[i].data == 0)
                && (index == PREPROCESSING_VMEM_MAX_ENTRIES))
        {
            index = i;
        }
        // Check whether address and size are in an empty address space.
        else if ((sdram > vmem_memoryMap[i].sdram)
                && (sdram < vmem_memoryMap[i].sdram + vmem_memoryMap[i].size))
        {
            printf("SDRAM address %u is in between existing image data of "
                    "size %u at %u, store images first.\n",
                    (unsigned int)(sdram),
                    (unsigned int)(vmem_memoryMap[i].size),
                    (unsigned int)(vmem_memoryMap[i].sdram));
            return -PREPROCESSING_INVALID_ADDRESS;
        }
        else if ((sdram < vmem_memoryMap[i].sdram)
                && (sdram + size > vmem_memoryMap[i].sdram))
        {
            printf("SDRAM address %u and size %u will overwrite existing "
                    "image data of size %u, store images first.\n",
                    (unsigned int)(sdram), (unsigned int)(size),
                    (unsigned int)(vmem_memoryMap[i].sdram));
            return -PREPROCESSING_INVALID_ADDRESS;
        }
        // Overwrite an existing entry if SDRAM address and size match.
        else if (vmem_memoryMap[i].sdram == sdram)
        {
            if (vmem_memoryMap[i].size == size)
            {
                index = i;
            }
            else if (sdram != 0)
            {
                return -PREPROCESSING_INVALID_ADDRESS;
            }
        }
    }

    if (index == PREPROCESSING_VMEM_MAX_ENTRIES)
    {
        printf("No free entries left in memory map. Delete first.\n");
        return -PREPROCESSING_NO_MEMORY;
    }

    // Set the new memory map entry.
    vmem_memoryMap[index].sdram = sdram;
    vmem_memoryMap[index].size = size;
    vmem_memoryMap[index].datasetId = datasetId;
    vmem_memoryMap[index].data = (data);
    //preprocessing_vmem_print();
    
    return (int)(index);
}

/*****************************************************************************/

int preprocessing_vmem_deleteEntry(uint32_t sdram)
{
    unsigned int index = 0;

    for (; index < PREPROCESSING_VMEM_MAX_ENTRIES; index++)
    {
        if (vmem_memoryMap[index].sdram == sdram)
        {
            break;
        }
    }

    if (index == PREPROCESSING_VMEM_MAX_ENTRIES)
    {
        printf("Cannot find SDRAM entry for address 0x%08x.\n",
                (unsigned int)(sdram));
        return PREPROCESSING_INVALID_ADDRESS;
    }

    memset(&vmem_memoryMap[index], 0, sizeof(vmem_memoryMap[index]));
    //preprocessing_vmem_print();

    return PREPROCESSING_SUCCESSFUL;
}

/*****************************************************************************/

uint32_t preprocessing_vmem_getSize(uint32_t sdram)
{
    unsigned int index = vmem_getIndex(sdram);
    
    if (index == PREPROCESSING_VMEM_MAX_ENTRIES)
    {
        printf("Cannot find SDRAM entry for address 0x%08x.\n",
                (unsigned int)(sdram));
        return 0xFFFFFFFF;
    }

    return vmem_memoryMap[index].size;
}

/*****************************************************************************/

uint32_t preprocessing_vmem_getDatasetId(uint32_t sdram)
{
    unsigned int index = vmem_getIndex(sdram);
    
    if (index == PREPROCESSING_VMEM_MAX_ENTRIES)
    {
        printf("Cannot find SDRAM entry for address 0x%08x.\n",
                (unsigned int)(sdram));
        return 0xFFFFFFFF;
    }

    return vmem_memoryMap[index].datasetId;
}

/*****************************************************************************/

void* preprocessing_vmem_getDataAddress(uint32_t sdram)
{
    unsigned int index = vmem_getIndex(sdram);
    
    if (index == PREPROCESSING_VMEM_MAX_ENTRIES)
    {
        printf("Cannot find SDRAM entry for address 0x%08x.\n",
                (unsigned int)(sdram));
        return 0;
    }

    return (void*)(vmem_memoryMap[index].data);
}

/*****************************************************************************/

bool preprocessing_vmem_isProcessingSizeValid(uint32_t sdram, uint16_t rows,
        uint16_t cols)
{
    uint32_t size = preprocessing_vmem_getSize(sdram);

    if (size == 0xFFFFFFFF)
    {
        return false;
    }
   else if (((uint32_t)(rows) * cols ) > size)
    	// else if (((uint32_t)(rows) * cols ) > size)
    {
        printf("Invalid processing size %u for related size %u of source "
                "address %u\n", rows * cols,
                (unsigned int)(size), (unsigned int)(sdram));
        return false;
    }

    return true;
}

/*****************************************************************************/

void preprocessing_vmem_print(void)
{
    printf("\t  Entry   |   SDRAM   |   Size    |  Dataset  |  RAM      \n");

    for (unsigned int index = 0; index < PREPROCESSING_VMEM_MAX_ENTRIES;
            index++)
    {
        printf("\t%9u | %9u | %9u | %9u | 0x%8p\n", index,
                (unsigned int)(vmem_memoryMap[index].sdram),
                (unsigned int)(vmem_memoryMap[index].size),
                (unsigned int)(vmem_memoryMap[index].datasetId),
                vmem_memoryMap[index].data);
    }
}

/* PRIVATE IMPLEMENTATION ****************************************************/

static unsigned int vmem_getIndex(uint32_t sdram)
{
    unsigned int index = 0;
    
    for (; index < PREPROCESSING_VMEM_MAX_ENTRIES; index++)
    {
        if (vmem_memoryMap[index].sdram == sdram)
        {
            break;
        }
    }

    return index;
}
