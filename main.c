#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Declare the external assembly function
extern void imgCvtGrayInttoFloat(int height, int width, const int* input, float* output);

// Reference implementation in C for validation
void imgCvtGrayInttoFloat_C(int height, int width, const int* input, float* output) {
    int total_pixels = height * width;
    for (int i = 0; i < total_pixels; i++) {
        output[i] = (float)input[i] / 255.0f;
    }
}

// Generate random input pixels in [0, 255]
void init_pixels(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 256;
    }
}

// Validate the correctness of Assembly output vs C output
int verify_correctness(int size, const float* asm_out, const float* c_out) {
    for (int i = 0; i < size; i++) {
        if (fabsf(asm_out[i] - c_out[i]) > 1e-5f) {
            return 0; // Failed
        }
    }
    return 1; // Passed
}

// Helper function to time the assembly routine 30 times
void run_benchmark(int height, int width) {
    int size = height * width;
    int* input = (int*)malloc(size * sizeof(int));
    float* asm_output = (float*)malloc(size * sizeof(float));
    float* c_output = (float*)malloc(size * sizeof(float));

    if (!input || !asm_output || !c_output) {
        printf("Memory allocation failed for size %dx%d\n", height, width);
        return;
    }

    init_pixels(input, size);

    // Warm-up run and correctness check
    imgCvtGrayInttoFloat(height, width, input, asm_output);
    imgCvtGrayInttoFloat_C(height, width, input, c_output);
    int is_correct = verify_correctness(size, asm_output, c_output);

    // Execute 30 times to find the average time
    int iterations = 30;
    double total_elapsed_sec = 0.0;

    for (int i = 0; i < iterations; i++) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        imgCvtGrayInttoFloat(height, width, input, asm_output);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        total_elapsed_sec += elapsed;
    }

    double average_ms = (total_elapsed_sec / iterations) * 1000.0;

    printf("%10d | %6dx%-6d | %15s | %18.6f ms\n",
        size, height, width, is_correct ? "PASSED" : "FAILED", average_ms);

    free(input);
    free(asm_output);
    free(c_output);
}

int main() {
    srand(12345); // Seed for reproducible results

    printf("--- PART 1: Correctness Verification (3x4 sample image) ---\n");
    int sample_height = 3;
    int sample_width = 4;
    int sample_input[] = {
        64,  89,  114, 84,
        140, 166, 191, 84,
        216, 242, 38,  84
    };
    int sample_size = sample_height * sample_width;
    float sample_output_asm[12] = { 0 };
    float sample_output_c[12] = { 0 };

    imgCvtGrayInttoFloat(sample_height, sample_width, sample_input, sample_output_asm);
    imgCvtGrayInttoFloat_C(sample_height, sample_width, sample_input, sample_output_c);

    printf("Input integer matrix:\n");
    for (int r = 0; r < sample_height; r++) {
        for (int c = 0; c < sample_width; c++) {
            printf("%4d ", sample_input[r * sample_width + c]);
        }
        printf("\n");
    }

    printf("\nAssembly converted single-precision float matrix:\n");
    for (int r = 0; r < sample_height; r++) {
        for (int c = 0; c < sample_width; c++) {
            printf("%7.2f ", sample_output_asm[r * sample_width + c]);
        }
        printf("\n");
    }

    int verified = verify_correctness(sample_size, sample_output_asm, sample_output_c);
    printf("\nResult Check vs C execution: %s\n\n", verified ? "SUCCESS" : "FAIL");

    printf("--- PART 2: Performance Profiling (30-run average per size) ---\n");
    printf("%10s | %13s | %15s | %18s\n", "Total Pixels", "Dimensions", "Correctness Check", "Avg Run Time");
    printf("------------------------------------------------------------------------\n");
    run_benchmark(10, 10);
    run_benchmark(100, 100);
    run_benchmark(1000, 1000);
    printf("------------------------------------------------------------------------\n");

    return 0;
}