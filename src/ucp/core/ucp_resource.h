/**
* Copyright (c) 2001-2019. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * See file LICENSE for terms.
 */


#ifndef UCP_CORE_RESOURCE_H_
#define UCP_CORE_RESOURCE_H_

#include <ucp/api/ucp.h>
#include <uct/api/uct.h>

BEGIN_C_DECLS

uct_md_h ucp_context_find_tl_md(ucp_context_h context, const char *md_name);

END_C_DECLS

#endif
