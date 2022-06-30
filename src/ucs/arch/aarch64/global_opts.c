/**
* Copyright (c) 2019. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
*
* See file LICENSE for terms.
*/

#if defined(__aarch64__)

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <ucs/arch/global_opts.h>
#include <ucs/config/parser.h>

ucs_config_field_t ucs_arch_global_opts_table[] = {
  {NULL}
};

void ucs_arch_print_memcpy_limits(ucs_arch_global_opts_t *config)
{
}

#endif
