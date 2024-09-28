#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <immintrin.h> // For SSE2 Intrinsics
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "util.h"

bool sse2_strcmp_16(const char *str1, const char *str2)
{
    __m128i s1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(str1)); // Load 16 bytes from str1
    __m128i s2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(str2)); // Load 16 bytes from str2

    // Compare the 16-byte chunks
    __m128i cmp = _mm_cmpeq_epi8(s1, s2);

    // Check if all bytes are equal (if all bits are set to 1)
    return _mm_movemask_epi8(cmp) == 0xFFFF;
}

using Key = std::array<char, 16>;

struct LookupArray
{
    alignas(64) std::vector<Key> keys;
    alignas(64) std::vector<int32_t> values;
};

std::random_device rd;
std::mt19937 gen(rd());

auto generate_random(int N) -> int
{
    std::uniform_int_distribution<> distr(0, N - 1);
    return distr(gen);
}

auto generate_key(int index) { return "some_key/" + std::to_string(index); }

auto generate_lookup_keys(int N, int max) -> std::vector<Key>
{
    std::vector<Key> keys;
    keys.resize(N);

    for (auto i = 0; i < N; i++)
    {
        auto key = generate_key(generate_random(max));
        std::strncpy(keys[i].data(), key.c_str(), keys[i].size());
    }
    return keys;
}

auto generate_lookup_keys_string(int N, int max) -> std::vector<std::string>
{
    std::vector<std::string> keys;
    keys.resize(N);

    for (auto i = 0; i < N; i++)
    {
        keys[i] = generate_key(generate_random(max));
    }
    return keys;
}

auto generate_lookup_array(int N) -> LookupArray
{
    LookupArray arr;
    arr.keys.resize(N);
    arr.values.resize(N);

    for (uint32_t i = 0; i < N; i++)
    {
        auto key = generate_key(i);
        std::strncpy(arr.keys[i].data(), key.c_str(), 16);
        arr.values[i] = i;
    }
    return arr;
}

auto generate_lookup_dict(int N) -> std::unordered_map<std::string, int32_t>
{
    std::unordered_map<std::string, int32_t> map;
    map.reserve(N);

    for (int32_t i = 0; i < N; i++)
    {
        auto key = generate_key(i);
        map[key] = i;
    }
    return map;
}

auto lookup_using_array(LookupArray &lookup_array, std::vector<Key> &keys_to_sum) -> std::chrono::microseconds
{

    auto start = std::chrono::high_resolution_clock::now();
    int32_t total = 0;
    for (int k = 0; k < keys_to_sum.size(); k++)
    {
        const auto key = keys_to_sum[k];

        for (auto i = 0; i < lookup_array.keys.size(); i += 4)
        {
            bool is_equal1 = sse2_strcmp_16(lookup_array.keys[i].data(), key.data());
            bool is_equal2 = sse2_strcmp_16(lookup_array.keys[i + 1].data(), key.data());
            bool is_equal3 = sse2_strcmp_16(lookup_array.keys[i + 2].data(), key.data());
            bool is_equal4 = sse2_strcmp_16(lookup_array.keys[i + 3].data(), key.data());

            if (is_equal1)
            {
                total += lookup_array.values[i];
                break;
            }
            if (is_equal2)
            {
                total += lookup_array.values[i + 1];
                break;
            }
            if (is_equal3)
            {
                total += lookup_array.values[i + 2];
                break;
            }
            if (is_equal4)
            {
                total += lookup_array.values[i + 3];
                break;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "  Total from array: " << total << std::endl;

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

auto lookup_dict(std::unordered_map<std::string, int32_t> &dict, std::vector<std::string> &keys_to_sum)
    -> std::chrono::microseconds
{

    auto start = std::chrono::high_resolution_clock::now();
    int32_t total = 0;
    for (const auto &key : keys_to_sum)
    {
        total += dict[key];
    }

    std::cout << "  Total from dict: " << total << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

int main(int argc, char *argv[])
{
    auto lookup_table_size = 64;
    auto num_random_lookups = 1024 * 20;

    auto arr = generate_lookup_array(lookup_table_size);
    auto dict = generate_lookup_dict(lookup_table_size);

    auto lookup_keys = generate_lookup_keys(num_random_lookups, lookup_table_size - 1);
    std::vector<std::string> lookup_keys_string;
    lookup_keys_string.reserve(lookup_keys.size());
    for (auto &key : lookup_keys)
    {
        lookup_keys_string.push_back(std::string(key.data()));
    }

    for (auto num_times = 0; num_times < 1; num_times++)
    {
        flush_cache();
        auto duration = lookup_using_array(arr, lookup_keys);
        std::cout << "Array: " << format_time(duration.count()) << std::endl;

        flush_cache();
        duration = lookup_dict(dict, lookup_keys_string);
        std::cout << "Dict: " << format_time(duration.count()) << std::endl;
    }
}
