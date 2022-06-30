/**
* Copyright (c) 2001-2019. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * See file LICENSE for terms.
 */

#include <ucs/sys/compiler.h>

extern int test_module_loaded;

UCS_STATIC_INIT {
    ++test_module_loaded;
}
