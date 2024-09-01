#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

auto fill_array(std::vector<int32_t> &entries, int size)
{
    entries.resize(size);

    for (auto i = 0; i < size; i++)
    {
        entries[i] = i;
    }
}

auto add_together_naive(std::vector<int32_t> arr, int num_times) -> std::chrono::microseconds
{
    std::chrono::microseconds duration(0);

    auto start = std::chrono::high_resolution_clock::now();
    int64_t total = 0;
    for (auto num = 0; num < num_times; num++)
    {
        for (auto i = 0; i < arr.size(); i++)
        {
            total += arr[i];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Total " << total << std::endl;
    return duration;
}

auto add_together_fast(std::vector<int32_t> arr, int num_times) -> std::chrono::microseconds
{
    // I wonder if we perhaps fuck up the hardware prefetcher here,
    // but make up for it by utilizing the L1 cache better.
    std::chrono::microseconds duration(0);
    auto start = std::chrono::high_resolution_clock::now();
    int64_t total = 0;
    int32_t chunk_size = (8) / sizeof(int32_t);
    for (auto i = 0; i < arr.size(); i = i + chunk_size)
    {
        for (auto num = 0; num < num_times; num++)
        {
            for (auto j = 0; j < chunk_size; j++)
            {
                total += arr[i + j];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Total " << total << std::endl;
    return duration;
}

int main(int argc, char *argv[])
{
    std::string type = "naive";
    if (argc == 2)
    {
        type = std::string(argv[1]);
    }

    // auto N = (37.3 * 1024 * 1024) / 4;
    auto N = (128 * 1024 * 1024) / 4;
    auto num_times = 30;
    std::vector<int32_t> arr;
    fill_array(arr, N);
    auto duration_naive = add_together_naive(arr, num_times);
    std::cout << "  Naive: " << duration_naive.count() << " microseconds." << std::endl;

    auto duration_fast = add_together_fast(arr, num_times);
    std::cout << "  Optimized: " << duration_fast.count() << " microseconds." << std::endl;
    std::cout << "  Speedup: "
              << static_cast<double>(duration_naive.count()) / static_cast<double>(duration_fast.count()) << std::endl;
}
