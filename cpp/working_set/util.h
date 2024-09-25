
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

const int CACHE_LINE_SIZE = 64;
const int L1_SIZE = 32 * 1024;
const int L2_SIZE = 256 * 1024;
const int L3_SIZE = 16 * 1024 * 1024;


constexpr uint64_t GB_TO_BYTES = 1024ULL * 1024 * 1024;
constexpr uint64_t MB_TO_BYTES = 1024ULL * 1024;
constexpr uint64_t KB_TO_BYTES = 1024ULL;

constexpr uint64_t to_gb(double gb) {
    return static_cast<uint64_t>(gb * GB_TO_BYTES);
}

constexpr uint64_t to_mb(double mb) {
    return static_cast<uint64_t>(mb * MB_TO_BYTES);
}

constexpr uint64_t to_kb(double kb) {
    return static_cast<uint64_t>(kb * KB_TO_BYTES);
}

std::string format_bytes(uint64_t bytes) {
    const uint64_t KB = 1024;
    const uint64_t MB = KB * 1024;
    const uint64_t GB = MB * 1024;

    std::ostringstream oss;

    if (bytes >= GB) {
        // Convert to GB and format the output
        oss << std::fixed << std::setprecision(2) << (bytes / static_cast<double>(GB)) << " GB";
    } else if (bytes >= MB) {
        // Convert to MB and format the output
        oss << std::fixed << std::setprecision(2) << (bytes / static_cast<double>(MB)) << " MB";
    } else if (bytes >= KB) {
        // Convert to KB and format the output
        oss << std::fixed << std::setprecision(2) << (bytes / static_cast<double>(KB)) << " KB";
    } else {
        // Keep it in bytes
        oss << bytes << " bytes";
    }

    return oss.str();
}

std::string format_time(uint64_t microseconds) {
    const uint64_t MICROSECONDS_IN_MILLISECOND = 1000;
    const uint64_t MICROSECONDS_IN_SECOND = 1000 * 1000;

    std::ostringstream oss;

    if (microseconds >= MICROSECONDS_IN_SECOND) {
        // Convert to seconds and format the output
        oss << std::fixed << std::setprecision(2) << (microseconds / static_cast<double>(MICROSECONDS_IN_SECOND)) << " seconds";
    } else if (microseconds >= MICROSECONDS_IN_MILLISECOND) {
        // Convert to milliseconds and format the output
        oss << std::fixed << std::setprecision(2) << (microseconds / static_cast<double>(MICROSECONDS_IN_MILLISECOND)) << " milliseconds";
    } else {
        // Keep it in microseconds
        oss << microseconds << " microseconds";
    }
    return oss.str();
}

