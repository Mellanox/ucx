#ifndef LIBPERF_CUDA_H_
#define LIBPERF_CUDA_H_

#include <stddef.h>  // For size_t
#include <stdint.h>  // For uint64_t

#define NS_TO_SEC(ns) ((ns) / 1000000000.0)
#define NS_TO_MS(ns) ((ns) / 1000000.0)
#define NS_TO_US(ns) ((ns) / 1000.0)
#define NS_TO_NS(ns) (ns)
#define BYTES_TO_MB(bytes) ((bytes) / 1024.0 / 1024.0)

typedef unsigned long long ucx_perf_cuda_time_t;

//TODO: Replace with real packed batch API
typedef struct uct_gdaki_packed_batch {
    void* qp;
    uint64_t batch_length;
    uint64_t batch_total_size;
    uint8_t** src_buf;
    uint64_t* sizes;
    uint32_t* src_mkey;
    uint8_t** dst_buf;
    uint32_t* dst_mkey;
} uct_gdaki_packed_batch_t;

//TODO: Improve code resue by resusing CPU perftest code.
typedef struct ucx_perf_cuda_result {
    uint64_t           iters;
    unsigned long long elapsed_time;
    uint64_t           bytes;
    struct {
        double         percentile;
        double         moment_average; /* Average since last report */
        double         total_average;  /* Average of the whole test */
    }
    latency, bandwidth, msgrate;
} ucx_perf_cuda_result_t;

/**
 * Describes a performance test.
 */
typedef struct ucx_perf_params_cuda {
    uct_gdaki_packed_batch_t* batch;
    unsigned                  max_outstanding; /* Maximal number of outstanding sends */
    unsigned                  m_sends_outstanding;
    uint64_t                  warmup_iter;     /* Number of warm-up iterations */
    double                    warmup_time;     /* Approximately how long to warm-up */
    uint64_t                  max_iter;        /* Iterations limit, 0 - unlimited */
    ucx_perf_cuda_time_t      max_time;        /* Time limit (seconds), 0 - unlimited */
    ucx_perf_cuda_time_t      report_interval; /* Interval at which to call the report callback in nanoseconds */

} ucx_perf_params_cuda_t;

typedef struct ucx_perf_context_cuda {
    ucx_perf_params_cuda_t   params;

    /* Measurements */
    ucx_perf_cuda_time_t     start_time;      /* inaccurate end time (upper bound) */
    ucx_perf_cuda_time_t     end_time;        /* inaccurate end time (upper bound) */
    ucx_perf_cuda_time_t     prev_time;       /* time of previous iteration */
    uint64_t                 last_report;     /* last report to CPU */
    volatile int             test_completed;    // Signal test completion

    int                      active_buffer;
    /* Measurements of current/previous **report** */
    struct {
        uint64_t             msgs;    /* number of messages */
        uint64_t             bytes;   /* number of bytes */
        uint64_t             iters;   /* number of iterations */
        ucx_perf_cuda_time_t time;    /* inaccurate time (for median and report interval) */
    } current[2], prev[2];

    volatile int             results_ready;     // Signal CPU to calculate and print
} ucx_perf_context_cuda_t;

void inline ucx_perf_calc_cuda_result(ucx_perf_context_cuda_t *perf, ucx_perf_cuda_result_t *result)
{
    double precise_total_time = perf->current[0].time - perf->start_time;
    double precise_total_time_sec = NS_TO_SEC(precise_total_time);
    uint64_t total_iters = perf->current[0].iters + perf->current[1].iters;
    uint64_t total_bytes = perf->current[0].bytes + perf->current[1].bytes;
    uint64_t total_msgs = perf->current[0].msgs + perf->current[1].msgs;

    result->latency.total_average =
        precise_total_time / total_iters;

    result->bandwidth.total_average =
        BYTES_TO_MB(total_bytes) / precise_total_time_sec;

    result->msgrate.total_average =
        total_msgs / precise_total_time_sec;
}

void inline ucx_perf_calc_cuda_moment_result(ucx_perf_context_cuda_t *perf, int read_buf, ucx_perf_cuda_result_t *result) {
    double precise_time_diff_sec = NS_TO_SEC(perf->current[read_buf].time - perf->prev[read_buf].time);
    // printf("iters: %lu, bytes: %lu, msgs: %lu time: %.3f\n", perf->current[read_buf].iters - perf->prev[read_buf].iters, 
    //         perf->current[read_buf].bytes - perf->prev[read_buf].bytes, perf->current[read_buf].msgs - perf->prev[read_buf].msgs, precise_time_diff_sec);
    result->latency.moment_average =
        precise_time_diff_sec / (perf->current[read_buf].iters - perf->prev[read_buf].iters);
    
    result->bandwidth.moment_average =
        BYTES_TO_MB(perf->current[read_buf].bytes - perf->prev[read_buf].bytes) /
        precise_time_diff_sec;
    
    result->msgrate.moment_average =
        (perf->current[read_buf].msgs - perf->prev[read_buf].msgs) /
        precise_time_diff_sec;
}

void inline ucx_perf_cuda_report(ucx_perf_cuda_result_t *result)
{
    printf("Latency: %.3f ns\n", result->latency.moment_average);
    printf("Bandwidth: %.2f MB/s\n", result->bandwidth.moment_average);
    printf("Message rate: %.2f Mps\n", result->msgrate.moment_average);
}

#ifdef __CUDACC__

__device__ __inline__ unsigned long long gdaki_get_time_ns()
{
	unsigned long long globaltimer;
	// 64-bit GPU global nanosecond timer
	asm volatile("mov.u64 %0, %globaltimer;" : "=l"(globaltimer));
	return globaltimer;
}

__device__
static inline void ucx_perf_cuda_update(ucx_perf_context_cuda_t *perf,
                                        ucx_perf_cuda_time_t current_time,
                                        uint64_t iters,
                                        size_t bytes)
{
    perf->current[perf->active_buffer].time   = current_time; // TODO: capture time
    perf->current[perf->active_buffer].iters += iters;
    perf->current[perf->active_buffer].bytes += bytes;
    perf->current[perf->active_buffer].msgs  += 1;

    perf->prev_time = perf->current[perf->active_buffer].time;
}

#endif // __CUDACC__

#endif // LIBPERF_CUDA_H_
