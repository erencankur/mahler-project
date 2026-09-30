#include "mahler/sequence.hpp"

#include <limits>
#include <stdexcept>

namespace mahler {
namespace {

constexpr auto max_value = std::numeric_limits<std::uint64_t>::max();

std::uint64_t checked_add(std::uint64_t a, std::uint64_t b) {
    if (a > max_value - b) {
        throw std::overflow_error("natural position exceeds uint64_t range");
    }
    return a + b;
}

std::uint64_t checked_multiply(std::uint64_t a, std::uint64_t b) {
    if (b != 0 && a > max_value / b) {
        throw std::overflow_error("natural position exceeds uint64_t range");
    }
    return a * b;
}

} // namespace

unsigned decimal_digits(std::uint64_t number) noexcept {
    unsigned width = 1;
    while (number >= 10) {
        number /= 10;
        ++width;
    }
    return width;
}

DigitLocation locate_digit(std::uint64_t position) {
    if (position == 0) {
        throw std::invalid_argument("digit position must be positive");
    }

    unsigned width = 1;
    std::uint64_t first = 1;
    std::uint64_t remaining = position;
    while (true) {
        const auto count = first * 9;
        // An overflowing block is larger than any supported position: the
        // requested digit must lie in it. Never form that overflowing product.
        if (count > max_value / width || remaining <= count * width) {
            break;
        }
        remaining -= count * width;
        first *= 10;
        ++width;
    }

    const auto offset = remaining - 1;
    return {first + offset / width,
            static_cast<unsigned>(offset % width), width};
}

std::uint8_t digit_at(std::uint64_t position) {
    const auto location = locate_digit(position);
    const auto digits = std::to_string(location.source_number);
    return static_cast<std::uint8_t>(digits[location.digit_offset] - '0');
}

std::uint64_t natural_position(std::uint64_t number) {
    if (number == 0) {
        throw std::invalid_argument("target number must be positive");
    }

    const auto width = decimal_digits(number);
    std::uint64_t first = 1;
    std::uint64_t position = 1;
    for (unsigned digits = 1; digits < width; ++digits) {
        const auto block = checked_multiply(checked_multiply(9, first), digits);
        position = checked_add(position, block);
        first = checked_multiply(first, 10);
    }
    return checked_add(position, checked_multiply(number - first, width));
}

std::string make_prefix(std::uint64_t last_integer) {
    if (last_integer == 0) {
        throw std::invalid_argument("prefix endpoint must be positive");
    }
    if (last_integer > max_prefix_integer) {
        throw std::length_error("prefix endpoint exceeds the 1000000 limit");
    }

    const auto length = natural_position(last_integer) - 1 + decimal_digits(last_integer);
    std::string prefix;
    prefix.reserve(static_cast<std::size_t>(length));
    for (std::uint64_t number = 1; number <= last_integer; ++number) {
        prefix += std::to_string(number);
    }
    return prefix;
}

} // namespace mahler
