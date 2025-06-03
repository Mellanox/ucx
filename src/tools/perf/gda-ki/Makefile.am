#
# Copyright (c) NVIDIA CORPORATION & AFFILIATES, 2001-2024. ALL RIGHTS RESERVED.
# See file LICENSE for terms.
#

if HAVE_NVCC

# Build shared library
lib_LTLIBRARIES = libperf_gdaki.la

# Library sources
libperf_gdaki_la_SOURCES = \
    gdaki_benchmark.h \
    libperf_cuda.h \
    gdaki_mem_handle.h \
    gdaki_mem_handle.cu \
    bw_benchmark.cu \
    lat_benchmark.cu

# Add include directory
gdakidir = $(includedir)/gda-ki
gdaki_HEADERS = gdaki_benchmark.h

# Compile CUDA files with NVCC wrapper
SUFFIXES = .cu
NVCC_WRAP = $(top_srcdir)/buildlib/tools/nvcc_wrap.sh
.cu.lo:
	$(LIBTOOL) --tag=CXX --mode=compile $(NVCC_WRAP) -c $< -o $@ -DHAVE_CONFIG_H  $(BASE_CPPFLAGS)

# Library version info (major:minor:revision)
libperf_gdaki_la_LDFLAGS = -version-info 0:0:0

# Include paths
libperf_gdaki_la_CPPFLAGS = $(BASE_CPPFLAGS) -I$(top_srcdir)/src

# Link with CUDA runtime, GDRcopy and C++ standard library
libperf_gdaki_la_LIBADD = -lcuda -lcudart -lstdc++

if HAVE_GDR_COPY
libperf_gdaki_la_LDFLAGS += $(GDR_COPY_LDFLAGS)
libperf_gdaki_la_CPPFLAGS += $(GDR_COPY_CPPFLAGS)
libperf_gdaki_la_LIBADD += $(GDR_COPY_LIBS)
endif

# Make sure wrapper script is executable
EXTRA_DIST = gdaki_benchmark.h gdaki_mem_handle.h gdaki_mem_handle.cu bw_benchmark.cu lat_benchmark.cu nvcc_wrap.sh

endif # HAVE_NVCC
