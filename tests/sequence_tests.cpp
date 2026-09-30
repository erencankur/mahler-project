#include "mahler/sequence.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template<class Exception, class Function>
void require_throws(Function function, const std::string& message) {
    try {
        function();
    } catch (const Exception&) {
        return;
    }
    throw std::runtime_error(message);
}

void known_examples() {
    for (const auto [position, expected] : {
             std::pair<std::uint64_t, unsigned>{7, 7}, {9, 9}, {10, 1}, {11, 0},
             {24, 1}, {189, 9}, {190, 1}, {191, 0}, {192, 0},
             {2020, 7}, {2889, 9}, {2890, 1}, {3719, 2}, {1'000'000, 1}}) {
        require(mahler::digit_at(position) == expected, "known digit at " + std::to_string(position));
    }
    for (const auto [number, expected] : {
             std::pair<std::uint64_t, std::uint64_t>{1, 1}, {10, 10}, {100, 190},
             {1000, 2890}, {9910, 38530}, {10000, 38890}, {100000, 488890},
             {1'000'000, 5'888'890}}) {
        require(mahler::natural_position(number) == expected, "known natural position");
    }
    require(mahler::make_prefix(13) == "12345678910111213", "small prefix");
}

void independent_reference() {
    // This oracle appends decimal strings and counts their lengths, rather than
    // using digit-group arithmetic or the production prefix generator.
    std::string reference;
    std::uint64_t position = 1;
    for (std::uint64_t number = 1; number <= 10000; ++number) {
        const auto text = std::to_string(number);
        require(mahler::natural_position(number) == position, "reference natural position");
        reference += text;
        position += text.size();
    }
    require(mahler::make_prefix(10000) == reference, "reference prefix");
    for (std::size_t i = 0; i < reference.size(); ++i) {
        require(mahler::digit_at(i + 1) == reference[i] - '0', "reference digit");
    }
}

void large_position_reference() {
    constexpr auto maximum = std::numeric_limits<std::uint64_t>::max();
    // Independent closed form from OEIS A117804, evaluated with wider arithmetic
    // only in the test oracle. Production code remains standard uint64_t C++.
    std::uint64_t power = 1;
    for (unsigned width = 1; width <= 19; ++width) {
        const std::uint64_t candidates[] = {power, power + 1, power + power / 2};
        for (const auto number : candidates) {
            const auto expected = static_cast<__uint128_t>(width) * number + 1
                - (static_cast<__uint128_t>(power) * 10 - 1) / 9;
            if (expected > maximum) {
                require_throws<std::overflow_error>([=] {
                    static_cast<void>(mahler::natural_position(number));
                }, "natural-position overflow");
                continue;
            }
            require(mahler::natural_position(number) == expected, "large closed-form position");
            const auto text = std::to_string(number);
            const auto position = static_cast<std::uint64_t>(expected);
            for (unsigned offset = 0; offset < width && offset <= maximum - position; ++offset) {
                const auto location = mahler::locate_digit(position + offset);
                require(location.source_number == number && location.digit_offset == offset,
                        "large location");
                require(mahler::digit_at(position + offset) == text[offset] - '0', "large digit");
            }
        }
        if (width < 19) {
            power *= 10;
        }
    }

    constexpr std::uint64_t last_number = 1'029'360'799'201'087'511;
    const auto last = mahler::locate_digit(maximum);
    require(last.source_number == last_number && last.digit_offset == 16 && last.digit_count == 19,
            "last uint64 position");
    require(mahler::digit_at(maximum) == 5, "last uint64 digit");
    require(mahler::natural_position(last_number) == maximum - 16, "last representable natural start");
    require_throws<std::overflow_error>([] {
        static_cast<void>(mahler::natural_position(last_number + 1));
    }, "next natural start must overflow");
}

void invalid_inputs_and_prefix_limit() {
    require_throws<std::invalid_argument>([] {
        static_cast<void>(mahler::digit_at(0));
    }, "zero digit position");
    require_throws<std::invalid_argument>([] {
        static_cast<void>(mahler::natural_position(0));
    }, "zero target");
    require_throws<std::invalid_argument>([] {
        static_cast<void>(mahler::make_prefix(0));
    }, "zero prefix endpoint");
    require_throws<std::length_error>([] {
        static_cast<void>(mahler::make_prefix(mahler::max_prefix_integer + 1));
    }, "prefix resource limit");
    const auto prefix = mahler::make_prefix(mahler::max_prefix_integer);
    require(prefix.size() == 5'888'896, "million prefix length");
    require(prefix.ends_with("9999991000000"), "million prefix endpoint");
    require(mahler::decimal_digits(0) == 1, "zero digit width helper");
}

} // namespace

int main() {
    try {
        known_examples();
        independent_reference();
        large_position_reference();
        invalid_inputs_and_prefix_limit();
        std::cout << "Core checks passed: examples, independent prefix, large positions, input limits.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
