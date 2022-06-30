/**
* Copyright (c) 2017. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * See file LICENSE for terms.
 */

#ifndef UCT_GDR_COPY_IFACE_H
#define UCT_GDR_COPY_IFACE_H

#include <uct/base/uct_iface.h>


#define UCT_GDR_COPY_IFACE_DEFAULT_BANDWIDTH (6911.0 * UCS_MBYTE)


#define UCT_GDR_COPY_IFACE_OVERHEAD (0)


typedef uint64_t uct_gdr_copy_iface_addr_t;


typedef struct uct_gdr_copy_iface {
    uct_base_iface_t            super;
    uct_gdr_copy_iface_addr_t   id;
} uct_gdr_copy_iface_t;


typedef struct uct_gdr_copy_iface_config {
    uct_iface_config_t      super;
} uct_gdr_copy_iface_config_t;

#endif
