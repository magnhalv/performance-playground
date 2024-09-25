#include <assert.h>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <xmmintrin.h>

#include "util.h"

struct Entry
{
    int32_t salary = 0;
    int32_t pension = 0;
    int32_t stocks = 0;
    int32_t num_slack_entries = 0;
};

auto fill_array(Entry *entries, int N) -> std::chrono::microseconds
{
    auto start = std::chrono::high_resolution_clock::now();
    const int size = N;
    for (auto i = 0; i < size; i++)
    {
        entries[i].salary = i;
        entries[i].pension = i * 2;
        entries[i].stocks = i * 3;
        entries[i].num_slack_entries = i * 4;
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

uint64_t result = 0;

auto sum_slow(const Entry *arr, int N) -> std::chrono::microseconds
{

    auto start = std::chrono::high_resolution_clock::now();
    const int size = N;
    Entry total = {};
    for (auto i = 0; i < size; i++)
    {
        const auto salary = arr[i].salary;
        total.salary += salary;
    }
    for (auto i = 0; i < size; i++)
    {
        const auto pension = arr[i].pension;
        total.pension += pension;
    }
    for (auto i = 0; i < size; i++)
    {
        const auto stocks = arr[i].stocks;
        total.stocks += stocks;
    }
    for (auto i = 0; i < size; i++)
    {
        const auto num_slack_entries = arr[i].num_slack_entries;
        total.num_slack_entries += num_slack_entries;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Total {" << total.salary << "," << total.pension << "," << total.stocks << "," << total.num_slack_entries << "}" << std::endl;
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

auto sum_fast(const Entry *arr, int N, int num_times) -> std::chrono::microseconds
{

    auto start = std::chrono::high_resolution_clock::now();
    const int size = N;
    Entry total = {};

    for (auto iteration = 0; iteration < num_times; iteration++)
    {
        for (auto i = 0; i < size; i++)
        {
            const auto salary = arr[i].salary;
            const auto pension = arr[i].pension;
            const auto stocks = arr[i].stocks;
            const auto num_slack_entries = arr[i].num_slack_entries;
            total.salary += salary;
            total.pension += pension;
            total.stocks += stocks;
            total.num_slack_entries += num_slack_entries;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Total {" << total.salary << "," << total.pension << "," << total.stocks << "," << total.num_slack_entries << "}" << std::endl;

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

auto working_set(const Entry *arr, int num_iterations, int num_working_sets, int working_set_size)
    -> std::chrono::microseconds
{

    auto start = std::chrono::high_resolution_clock::now();
    const int total_size = num_working_sets * working_set_size;
    Entry total = {};
    for (auto i = 0; i < total_size; i += working_set_size)
    {
        for (auto iteration = 0; iteration < num_iterations; iteration++)
        {
            for (auto j = 0; j < working_set_size; j++)
            {
                const auto index = i + j;
                const auto salary = arr[index].salary;
                const auto pension = arr[index].pension;
                const auto stocks = arr[index].stocks;
                const auto num_slack_entries = arr[index].num_slack_entries;
                total.salary += salary;
                total.pension += pension;
                total.stocks += stocks;
                total.num_slack_entries += num_slack_entries;
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Total {" << total.salary << "," << total.pension << "," << total.stocks << "," << total.num_slack_entries << "}" << std::endl;

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

// Shows how working_set might potentially be optimized by the compiler when using /O2
auto working_set_opt(const Entry* arr, int num_iterations, int num_working_sets, int working_set_size) 
    -> std::chrono::microseconds

{
    auto start = std::chrono::high_resolution_clock::now();
    const int total_size = num_working_sets * working_set_size;
    Entry total = {};  // Initialize total to zero

    int i = 0;
    // Loop over the working sets
    while (i < total_size)
    {
        int iteration = 0;
        // Loop for the number of iterations
        while (iteration < num_iterations)
        {
            // Loop unrolling for better cache utilization and fewer memory accesses
            for (int j = 0; j < working_set_size; j += 4)
            {
                // Use explicit array index rather than pointer dereferencing
                const int index0 = i + j;
                const int index1 = i + j + 1;
                const int index2 = i + j + 2;
                const int index3 = i + j + 3;

                // Group operations to reduce memory loads and optimize cache usage
                total.salary += arr[index0].salary;
                total.pension += arr[index0].pension;
                total.stocks += arr[index0].stocks;
                total.num_slack_entries += arr[index0].num_slack_entries;

                if (index1 < total_size)
                {
                    total.salary += arr[index1].salary;
                    total.pension += arr[index1].pension;
                    total.stocks += arr[index1].stocks;
                    total.num_slack_entries += arr[index1].num_slack_entries;
                }

                if (index2 < total_size)
                {
                    total.salary += arr[index2].salary;
                    total.pension += arr[index2].pension;
                    total.stocks += arr[index2].stocks;
                    total.num_slack_entries += arr[index2].num_slack_entries;
                }

                if (index3 < total_size)
                {
                    total.salary += arr[index3].salary;
                    total.pension += arr[index3].pension;
                    total.stocks += arr[index3].stocks;
                    total.num_slack_entries += arr[index3].num_slack_entries;
                }
            }

            ++iteration;
        }
        i += working_set_size;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Total {" << total.salary << "," << total.pension << "," << total.stocks << "," << total.num_slack_entries << "}" << std::endl;

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cout << "Specify slow|fast|working_set" << std::endl;
        return 1;
    }

    auto N = to_mb(1024) / sizeof(Entry);
    std::cout << "Entry size: " << sizeof(Entry) << " bytes" << std::endl;
    std::cout << "N = " << N << std::endl;
    std::cout << "Total array size: " << format_bytes(N * sizeof(Entry)) << std::endl;

    Entry *arr = static_cast<Entry *>(malloc(N * sizeof(Entry)));

    auto duration = fill_array(arr, N);
    std::cout << "Fill (cold): " << format_time(duration.count()) << std::endl;

    if (std::strcmp(argv[1], "slow") == 0)
    {
        duration = sum_slow(arr, N);
        std::cout << "sum_slow (warm): " << format_time(duration.count()) << std::endl;
    }
    auto num_iterations = 40;

    if (std::strcmp(argv[1], "fast") == 0)
    {
        duration = sum_fast(arr, N, num_iterations);
        std::cout << "sum_fast (warm): " << format_time(duration.count()) << std::endl;
    }

    if (std::strcmp(argv[1], "working_set") == 0)
    {
        auto working_set_bytes = argc == 3 ? to_kb(std::stoi(argv[2])) : to_kb(32);
        auto working_set_size = working_set_bytes / sizeof(Entry);
        assert(N % working_set_size == 0);
        auto num_working_sets = N / working_set_size;
        duration = working_set(arr, num_iterations, num_working_sets, working_set_size);
        std::cout << "working_set_size: " << format_bytes(working_set_bytes) << std::endl;
        std::cout << "working_set(warm): " << format_time(duration.count()) << std::endl;
    }

    if (std::strcmp(argv[1], "working_set_opt") == 0)
    {
        auto working_set_bytes = argc == 3 ? to_kb(std::stoi(argv[2])) : to_kb(32);
        auto working_set_size = working_set_bytes / sizeof(Entry);
        assert(N % working_set_size == 0);
        auto num_working_sets = N / working_set_size;
        duration = working_set_opt(arr, num_iterations, num_working_sets, working_set_size);
        std::cout << "working_set_size: " << format_bytes(working_set_bytes) << std::endl;
        std::cout << "working_set_opt(warm): " << format_time(duration.count()) << std::endl;
    }

    return 0;
}
