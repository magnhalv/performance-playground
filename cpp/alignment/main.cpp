
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <random>
#include <string>

struct BadAlignment
{
    int32_t value;
    bool a;
    int64_t b;
    bool c;
    int64_t d;
    bool e;
    int64_t f;
    bool g;
    int64_t h;
};

struct MidAlignment
{
    bool a;
    int64_t b;
    bool c;
    int64_t d;
    int32_t value;
    bool e;
    int64_t f;
    bool g;
    int64_t h;
};

struct GoodAlignment
{
    int32_t value;
    bool a;
    bool c;
    bool e;
    bool g;
    int64_t b;
    int64_t d;
    int64_t f;
    int64_t h;
};

auto fill_array(std::vector<BadAlignment> &entries, int size)
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

int main(int argc, char *argv[])
{
    std::cout << "BadAlignment size: " << sizeof(BadAlignment) << std::endl;

    constexpr int NUM_TIMES = 30;
    constexpr int NUM_ITERATIONS = 100;

    for (auto n = 1; n < NUM_ITERATIONS; n++)
    {
        auto N = 30700 * n;
        auto bad_num_bytes = sizeof(BadAlignment) * N;
        auto bad1 = static_cast<BadAlignment *>(malloc(bad_num_bytes));
        auto bad2 = static_cast<BadAlignment *>(malloc(bad_num_bytes));

        for (auto i = 0; i < N; i++)
        {
            bad1[i].value = 1;
            bad2[i].value = 2;
        }

        std::chrono::microseconds duration(0);
        for (auto num = 0; num < NUM_TIMES + 1; num++)
        {
            int32_t result = 0;

            auto start = std::chrono::high_resolution_clock::now();
            for (auto i = 0; i < N; i++)
            {
                result += (bad1[i].value + bad2[i].value);
            }
            auto end = std::chrono::high_resolution_clock::now();
            if (num != 0)
            {
                duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            }
        }
        std::cout << "  N = " << N << ": " << duration.count() << " microseconds." << std::endl;
        free(bad1);
        free(bad2);
    }

    std::cout << "GoodAlignment size: " << sizeof(GoodAlignment) << std::endl;
    for (auto n = 1; n < NUM_ITERATIONS; n++)
    {
        auto N = 30700 * n;
        auto good_num_bytes = sizeof(GoodAlignment) * N;
        auto good1 = static_cast<GoodAlignment *>(malloc(good_num_bytes));
        auto good2 = static_cast<GoodAlignment *>(malloc(good_num_bytes));

        for (auto i = 0; i < N; i++)
        {
            good1[i].value = 1;
            good2[i].value = 2;
        }

        std::chrono::microseconds duration(0);
        for (auto num = 0; num < NUM_TIMES + 1; num++)
        {
            int32_t result = 0;

            auto start = std::chrono::high_resolution_clock::now();
            for (auto i = 0; i < N; i++)
            {
                result += (good1[i].value + good2[i].value);
            }
            auto end = std::chrono::high_resolution_clock::now();
            if (num != 0)
            {
                duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            }
        }
        std::cout << "  N = " << N << ": " << duration.count() << " microseconds." << std::endl;
        free(good1);
        free(good2);
    }

    std::cout << "MidAlignment size: " << sizeof(MidAlignment) << std::endl;
    for (auto n = 1; n < NUM_ITERATIONS; n++)
    {
        auto N = 30700 * n;
        auto mid_num_bytes = sizeof(MidAlignment) * N;
        auto mid1 = static_cast<MidAlignment *>(malloc(mid_num_bytes));
        auto mid2 = static_cast<MidAlignment *>(malloc(mid_num_bytes));

        for (auto i = 0; i < N; i++)
        {
            mid1[i].value = 1;
            mid2[i].value = 2;
        }

        std::chrono::microseconds duration(0);
        for (auto num = 0; num < NUM_TIMES + 1; num++)
        {
            int32_t result = 0;

            auto start = std::chrono::high_resolution_clock::now();
            for (auto i = 0; i < N; i++)
            {
                result += (mid1[i].value + mid2[i].value);
            }
            auto end = std::chrono::high_resolution_clock::now();
            if (num != 0)
            {
                duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            }
        }
        std::cout << "  N = " << N << ": " << duration.count() << " microseconds." << std::endl;
        free(mid1);
        free(mid2);
    }
}
