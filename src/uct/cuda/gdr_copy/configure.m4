#
# Copyright (c) 2001-2017. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# Copyright (C) Advanced Micro Devices, Inc. 2019. ALL RIGHTS RESERVED.
# See file LICENSE for terms.
#

UCX_CHECK_GDRCOPY

AS_IF([test "x$gdrcopy_happy" = "xyes"], [uct_cuda_modules="${uct_cuda_modules}:gdrcopy"])
AC_CONFIG_FILES([src/uct/cuda/gdr_copy/Makefile])
