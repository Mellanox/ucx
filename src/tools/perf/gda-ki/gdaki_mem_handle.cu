#include "gdaki_mem_handle.h"
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_GDRAPI_H
#include <gdrapi.h>
#endif

#define PAGE_ROUND_UP(x, n)     (((x) + ((n) - 1)) & ~((n) - 1))

struct gdaki_mem_handle {
    void*    gpu_ptr;
    void*    cpu_ptr;
    size_t   size;
#ifdef HAVE_GDRAPI_H
    gdr_t    gdr;
    gdr_mh_t mh;
#endif
    bool     owns_gpu_mem;
};

gdaki_mem_handle_t gdaki_mem_create(void* gpu_ptr, size_t size) {
#ifndef HAVE_GDRAPI_H
    printf("GDRcopy is not available\n");
    return NULL;
#else
    int ret;
    gdaki_mem_handle_t handle = (gdaki_mem_handle_t)calloc(1, sizeof(*handle));
    if (!handle) {
        return NULL;
    }

    handle->size = PAGE_ROUND_UP(size, GPU_PAGE_SIZE);

    // Allocate GPU memory if not provided
    if (gpu_ptr == NULL) {
        cudaError_t cuda_err = cudaMalloc(&handle->gpu_ptr, handle->size);
        if (cuda_err != cudaSuccess) {
            fprintf(stderr, "Failed to allocate GPU memory: %s\n", 
                    cudaGetErrorString(cuda_err));
            free(handle);
            return NULL;
        }
        handle->owns_gpu_mem = true;
        cuda_err = cudaMemset(handle->gpu_ptr, 0, handle->size);
        if (cuda_err != cudaSuccess) {
            fprintf(stderr, "Failed to initialize GPU memory: %s\n", 
                    cudaGetErrorString(cuda_err));
            cudaFree(handle->gpu_ptr);
            free(handle);
            return NULL;
        }
    } else {
        assert(size % GPU_PAGE_SIZE == 0);
        handle->gpu_ptr = gpu_ptr;
        handle->owns_gpu_mem = false;
    }

    // Initialize GDRcopy
    handle->gdr = gdr_open();
    if (!handle->gdr) {
        fprintf(stderr, "GDRcopy initialization failed\n");
        goto cleanup;
    }

    // Pin and map the buffer
    ret = gdr_pin_buffer(handle->gdr, (unsigned long)handle->gpu_ptr, handle->size, 0, 0, &handle->mh);
    if (ret) {
        fprintf(stderr, "GDRcopy pin buffer failed\n");
        gdr_close(handle->gdr);
        goto cleanup;
    }

    ret = gdr_map(handle->gdr, handle->mh, &handle->cpu_ptr, handle->size);
    if (ret) {
        fprintf(stderr, "GDRcopy map failed\n");
        gdr_unpin_buffer(handle->gdr, handle->mh);
        gdr_close(handle->gdr);
        goto cleanup;
    }

    return handle;

cleanup:
    if (handle->owns_gpu_mem) {
        cudaFree(handle->gpu_ptr);
    }
    free(handle);
    return NULL;
#endif
}

void* gdaki_mem_get_ptr(gdaki_mem_handle_t handle) {
#ifndef HAVE_GDRAPI_H
    return NULL;
#else
    return handle ? handle->cpu_ptr : NULL;
#endif
}

void* gdaki_mem_get_gpu_ptr(gdaki_mem_handle_t handle) {
#ifndef HAVE_GDRAPI_H
    return NULL;
#else
    return handle ? handle->gpu_ptr : NULL;
#endif
}

void gdaki_mem_destroy(gdaki_mem_handle_t handle) {
#ifdef HAVE_GDRAPI_H
    if (!handle) {
        return;
    }

    gdr_unmap(handle->gdr, handle->mh, handle->cpu_ptr, handle->size);
    gdr_unpin_buffer(handle->gdr, handle->mh);
    gdr_close(handle->gdr);
    
    if (handle->owns_gpu_mem) {
        cudaFree(handle->gpu_ptr);
    }
    
    free(handle);
#endif
} 