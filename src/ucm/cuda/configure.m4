#
# Copyright (c) 2001-2018. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
#
# See file LICENSE for terms.
#

UCX_CHECK_CUDA
AS_IF([test "x$cuda_happy" = "xyes"], [ucm_modules="${ucm_modules}:cuda"])
AC_CONFIG_FILES([src/ucm/cuda/Makefile])
