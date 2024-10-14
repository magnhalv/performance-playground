
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

const int CACHE_LINE_SIZE = 64;
const int L1_SIZE = 32 * 1024;
const int L2_SIZE = 256 * 1024;
const int L3_SIZE = 16 * 1024 * 1024;

constexpr uint64_t GB_TO_BYTES = 1024ULL * 1024 * 1024;
constexpr uint64_t MB_TO_BYTES = 1024ULL * 1024;
constexpr uint64_t KB_TO_BYTES = 1024ULL;

constexpr uint64_t to_gb(double gb) { return static_cast<uint64_t>(gb * GB_TO_BYTES); }

constexpr uint64_t to_mb(double mb) { return static_cast<uint64_t>(mb * MB_TO_BYTES); }

constexpr uint64_t to_kb(double kb) { return static_cast<uint64_t>(kb * KB_TO_BYTES); }

#pragma optimize("", off)
inline void flush_cache()
{
    const size_t large_size = L3_SIZE;
    volatile uint8_t *dummy_data = new uint8_t[large_size];

    for (size_t i = 0; i < large_size; i++)
    {
        dummy_data[i] = dummy_data[i] + 1; // This will not be optimized away
    }
    auto total = 0;
    for (size_t i = 0; i < large_size; i++)
    {
        total += dummy_data[i];
    }

    //std::cout << total << std::endl;

    delete[] dummy_data;
}
#pragma optimize("", on)

inline std::string format_bytes(uint64_t bytes)
{
    const uint64_t KB = 1024;
    const uint64_t MB = KB * 1024;
    const uint64_t GB = MB * 1024;

    std::ostringstream oss;

    if (bytes >= GB)
    {
        // Convert to GB and format the output
        oss << std::fixed << std::setprecision(2) << (bytes / static_cast<double>(GB)) << " GB";
    }
    else if (bytes >= MB)
    {
        // Convert to MB and format the output
        oss << std::fixed << std::setprecision(2) << (bytes / static_cast<double>(MB)) << " MB";
    }
    else if (bytes >= KB)
    {
        // Convert to KB and format the output
        oss << std::fixed << std::setprecision(2) << (bytes / static_cast<double>(KB)) << " KB";
    }
    else
    {
        // Keep it in bytes
        oss << bytes << " bytes";
    }

    return oss.str();
}

inline std::string format_time(uint64_t microseconds)
{
    const uint64_t MICROSECONDS_IN_MILLISECOND = 1000;
    const uint64_t MICROSECONDS_IN_SECOND = 1000 * 1000;

    std::ostringstream oss;

    if (microseconds >= MICROSECONDS_IN_SECOND)
    {
        // Convert to seconds and format the output
        oss << std::fixed << std::setprecision(2) << (microseconds / static_cast<double>(MICROSECONDS_IN_SECOND))
            << " seconds";
    }
    else if (microseconds >= MICROSECONDS_IN_MILLISECOND)
    {
        // Convert to milliseconds and format the output
        oss << std::fixed << std::setprecision(2) << (microseconds / static_cast<double>(MICROSECONDS_IN_MILLISECOND))
            << " milliseconds";
    }
    else
    {
        // Keep it in microseconds
        oss << microseconds << " microseconds";
    }
    return oss.str();
}


// SIMD-optimized sum function using AVX2
/*

#include <immintrin.h> // For AVX2 intrinsics
auto simd_sum(const Entry *arr, int N) -> std::chrono::microseconds
{
    result = 0;
    auto start = std::chrono::high_resolution_clock::now();

    const int simdWidth = 8;
    __m256i resultVec = _mm256_setzero_si256(); 

    int i = 0;
    for (; i <= N - simdWidth; i += simdWidth)
    {
        // Load 8 uint32_t values into a SIMD register
        __m256i valuesVec = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&arr[i].value));

        // Add them to the result
        resultVec = _mm256_add_epi32(resultVec, valuesVec);
    }

    uint32_t resultArray[8];
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(resultArray), resultVec);

    uint64_t simdResult = 0;
    for (int j = 0; j < simdWidth; ++j) {
        simdResult += resultArray[j];
    }

    for (; i < N; ++i) {
        simdResult += arr[i].value;
    }

    result += simdResult;

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

    flush_cache();
    duration = simd_sum(arr, N);
    std::cout << "Sum: " << format_time(duration.count()) << std::endl;
    std::cout << result << std::endl;
*/
