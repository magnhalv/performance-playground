#include <chrono>
#include <cstdlib>
#include <cstring>
#include <immintrin.h>
#include <iostream>
#include <mm_malloc.h>
#include <ostream>
#include <string>
//
#define INDEX(i, j, N) ((i) * (N) + (j))

constexpr int NUM_TIMES = 20;

typedef double arr_t;
constexpr int SM = (64 / sizeof(arr_t));

auto fill_matrix(arr_t *m, int size)
{
    for (int i = 0; i < size; i++)
    {
        m[i] = static_cast<arr_t>(rand()) / RAND_MAX;
    }
}

auto naive(arr_t *res, arr_t *mult1, arr_t *mult2, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                auto m1 = mult1[INDEX(i, k, N)];
                auto m2 = mult2[INDEX(k, j, N)];
                res[INDEX(i, j, N)] += m1 * m2;
            }
        }
    }
}

auto block(arr_t *res, arr_t *mult1, arr_t *mult2, int N)
{
    for (int i = 0; i < N; i += SM)
    {
        for (int j = 0; j < N; j += SM)
        {
            for (int k = 0; k < N; k += SM)
            {
                for (int i2 = 0; i2 < SM; ++i2)
                {
                    arr_t *rres = &res[INDEX(i + i2, j, N)];
                    arr_t *rmul1 = &mult1[INDEX(i + i2, k, N)];
                    for (int k2 = 0; k2 < SM; ++k2)
                    {
                        arr_t *rmul2 = &mult2[INDEX(k + k2, j, N)];
                        auto m1 = rmul1[k2];
                        for (int j2 = 0; j2 < SM; ++j2)
                        {
                            auto m2 = rmul2[j2];
                            rres[j2] += m1 * m2;
                        }
                    }
                }
            }
        }
    }
}

auto simd(arr_t *res, arr_t *mult1, arr_t *mult2, int N)
{
    for (int i = 0; i < N; i += SM)
    {
        for (int j = 0; j < N; j += SM)
        {
            for (int k = 0; k < N; k += SM)
            {
                for (int i2 = 0; i2 < SM; i2++)
                {
                    arr_t *rres = &res[INDEX(i + i2, j, N)];
                    arr_t *rmul1 = &mult1[INDEX(i + i2, k, N)];
                    // Loading 1 double into a 4 double vector
                    for (int k2 = 0; k2 < SM; ++k2)
                    {
                        arr_t *rmul2 = &mult2[INDEX(k + k2, j, N)];

                        __m256d m1d = _mm256_set1_pd(rmul1[k2]);
                        for (int j2 = 0; j2 < SM; j2 += 4)
                        {
                            __m256d m2 = _mm256_load_pd(&rmul2[j2]);
                            __m256d r2 = _mm256_load_pd(&rres[j2]);
                            _mm256_store_pd(&rres[j2], _mm256_add_pd(_mm256_mul_pd(m2, m1d), r2));
                        }
                    }
                }
            }
        }
    }
}

auto alloc(size_t size) -> arr_t *
{
    void *ptr = nullptr;
    size_t alignment = 64;
    int result = posix_memalign(&ptr, alignment, size);

    if (result != 0)
    {
        std::cout << "Failed to align memory!" << std::endl;
        exit(1);
    }

    return static_cast<arr_t *>(ptr);
}

int main(int argc, char *argv[])
{

    std::string type = "naive";
    if (argc == 2)
    {
        type = std::string(argv[1]);
    }

    std::cout << "Running '" << type << "':" << std::endl;

    int dimensions[] = {64, 128, 256};

    for (auto N : dimensions)
    {
        const auto SIZE = N * N;

        arr_t *mult1 = alloc(sizeof(arr_t) * SIZE);
        arr_t *mult2 = alloc(sizeof(arr_t) * SIZE);
        arr_t *res = alloc(sizeof(arr_t) * SIZE);

        fill_matrix(mult1, SIZE);
        fill_matrix(mult2, SIZE);

        std::chrono::microseconds duration(0);
        for (int n = 0; n < NUM_TIMES; n++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            if (type == "block")
            {
                block(res, mult1, mult2, N);
            }
            else if (type == "simd")
            {
                simd(res, mult1, mult2, N);
            }
            else
            {
                naive(res, mult1, mult2, N);
            }
            auto end = std::chrono::high_resolution_clock::now();
            duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            memset(res, 0, sizeof(arr_t) * SIZE);
        }
        std::cout << "  N = " << N << ": " << duration.count() / NUM_TIMES << " microseconds." << std::endl;

        free(mult1);
        free(mult2);
        free(res);
    }
}
