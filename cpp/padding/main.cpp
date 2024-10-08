#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdlib.h>

#include "util.h"

struct Entry
{
    uint32_t value;
    uint32_t padding[0];
};

auto fill_array(Entry *entries, int N) -> std::chrono::microseconds
{
    auto start = std::chrono::high_resolution_clock::now();
    const int size = N;
    for (auto i = 0; i < size; i++)
    {
        entries[i].value = i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

uint64_t result = 0;

auto sum(const Entry *arr, int N) -> std::chrono::microseconds
{

    auto start = std::chrono::high_resolution_clock::now();
    const int size = N;
    for (auto i = 0; i < size; i++)
    {
        const auto value = arr[i].value;
        result += value;
    }
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

int main(int argc, char *argv[])
{
    auto N = to_gb(1) / sizeof(Entry);
    std::cout << "Entry size: " << sizeof(Entry) << " bytes" << std::endl;
    std::cout << "N = " << N << std::endl;
    std::cout << "Total array size: " << format_bytes(N * sizeof(Entry)) << std::endl;

    Entry *arr = static_cast<Entry *>(malloc(N * sizeof(Entry)));

    auto duration = fill_array(arr, N);
    std::cout << "Fill (cold): " << format_time(duration.count()) << std::endl;
    duration = sum(arr, N);
    std::cout << "Sum (warm): " << format_time(duration.count()) << std::endl;
    std::cout << result << std::endl;
}
