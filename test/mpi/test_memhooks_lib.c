/**
* Copyright (c) 2001-2015. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * See file LICENSE for terms.
 */

#include <sys/mman.h>
#include <stddef.h>


void *memhook_test_lib_call_mmap(size_t size)
{
    return mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
}


