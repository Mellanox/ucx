#
# Copyright (c) NVIDIA CORPORATION & AFFILIATES, 2001-2024. ALL RIGHTS RESERVED.
# See file LICENSE for terms.
#

if HAVE_NVCC

# Build shared library
lib_LTLIBRARIES = libucx_cuda.la

# Library sources
libucx_cuda_la_SOURCES = \
    hello_cuda.h \
    hello_cuda_kernel.cu

# Compile CUDA files with NVCC wrapper
SUFFIXES = .cu
NVCC_WRAP = $(top_srcdir)/buildlib/tools/nvcc_wrap.sh
# TODO: define rule for .cu.lo, .cu.o and improve this rule to use predefined opts.
.cu.lo:
	$(LIBTOOL) --tag=CXX --mode=compile $(NVCC_WRAP) "$(NVCC)" -c $< -o $@ -DHAVE_CONFIG_H  $(BASE_CPPFLAGS)

# Library version info (major:minor:revision)
libucx_cuda_la_LDFLAGS = -version-info 0:0:0

# Include paths
libucx_cuda_la_CPPFLAGS = \
    -I$(top_srcdir)/src

# Link with CUDA runtime
libucx_cuda_la_LIBADD = -lcuda -lcudart

# Install the header
include_HEADERS = hello_cuda.h

# Make sure wrapper script is executable
EXTRA_DIST = hello_cuda.h hello_cuda_kernel.cu nvcc_wrap.sh

endif # HAVE_NVCC 
