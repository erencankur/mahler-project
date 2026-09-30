#pragma once

#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <system_error>

inline std::uint64_t parse_positive(std::string_view value) {
    std::uint64_t result = 0;
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
    if (parsed.ec == std::errc::result_out_of_range) {
        throw std::overflow_error("input exceeds uint64_t range");
    }
    if (value.empty() || parsed.ec != std::errc{} ||
        parsed.ptr != value.data() + value.size() || result == 0) {
        throw std::invalid_argument("expected a positive decimal integer without signs or spaces");
    }
    return result;
}
