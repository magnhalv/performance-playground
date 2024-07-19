#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>

typedef int32_t arr_t;

struct Entry
{
    arr_t value;
    arr_t padding[255];
};

auto fill_array(std::vector<Entry> &entries, int size)
{
    entries.resize(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 10);

    for (auto &entry : entries)
    {
        entry.value = distr(gen);
    }
}

auto add_together(size_t size, int num_times) -> std::chrono::microseconds
{
    std::chrono::microseconds duration(0);
    std::vector<Entry> arr1;
    std::vector<Entry> arr2;

    fill_array(arr1, size);
    fill_array(arr2, size);

    for (auto num = 0; num < num_times + 1; num++)
    {
        arr_t result = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for (auto i = 0; i < size; i++)
        {
            result += (arr1[i].value + arr2[i].value);
        }
        auto end = std::chrono::high_resolution_clock::now();
        if (num != 0)
        {
            duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        }
    }
    return duration / num_times;
}

int main(int argc, char *argv[])
{
    std::string type = "naive";
    if (argc == 2)
    {
        type = std::string(argv[1]);
    }

    std::cout << "Running with entry size: " << sizeof(Entry) << std::endl;

    for (auto n = 1; n < 12; n++)
    {
        auto N = 30700 * n;
        auto duration = add_together(N, 30);
        std::cout << "  N = " << N << ": " << duration.count() << " microseconds." << std::endl;
    }
}
