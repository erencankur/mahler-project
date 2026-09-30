#pragma once

#include <cstdint>
#include <string>

namespace mahler {

inline constexpr std::uint64_t max_prefix_integer = 1'000'000;

struct DigitLocation {
    std::uint64_t source_number;
    unsigned digit_offset; // Zero-based from the left within the source number.
    unsigned digit_count;
};

[[nodiscard]] unsigned decimal_digits(std::uint64_t number) noexcept;

// Positions exclude the initial "0." and start at 1. Zero throws invalid_argument.
[[nodiscard]] DigitLocation locate_digit(std::uint64_t position);
[[nodiscard]] std::uint8_t digit_at(std::uint64_t position);

// Throws overflow_error if the natural starting position cannot fit uint64_t.
[[nodiscard]] std::uint64_t natural_position(std::uint64_t number);

// Concatenate 1..last_integer. Limited to max_prefix_integer in this release.
// Zero throws invalid_argument; exceeding the limit throws length_error.
[[nodiscard]] std::string make_prefix(std::uint64_t last_integer);

} // namespace mahler
