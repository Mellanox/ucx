#
# Copyright (c) 2001-2017. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# See file LICENSE for terms.
#

UCX_CHECK_CUDA

AS_IF([test "x$cuda_happy" = "xyes"], [uct_modules="${uct_modules}:cuda"])
uct_cuda_modules=""
m4_include([src/uct/cuda/gdr_copy/configure.m4])
AC_DEFINE_UNQUOTED([uct_cuda_MODULES], ["${uct_cuda_modules}"], [CUDA loadable modules])
AC_CONFIG_FILES([src/uct/cuda/Makefile])
