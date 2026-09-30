#include "mahler/early.hpp"
#include "mahler/sequence.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const std::string& context) {
    if (!condition) {
        throw std::runtime_error(context);
    }
}

void compare_independent(std::uint64_t maximum, const std::vector<mahler::EarlyResult>& results) {
    const std::string prefix = mahler::make_prefix(maximum);
    for (std::uint64_t number = 1; number <= maximum; ++number) {
        const std::string pattern = std::to_string(number);
        const auto natural = results[static_cast<std::size_t>(number)].natural_position;
        std::uint64_t first = natural;
        std::uint64_t count = 0;
        std::vector<std::uint64_t> positions;
        // Direct overlapping substring search: independent of numeric windows.
        for (std::size_t found = prefix.find(pattern); found != std::string::npos;
             found = prefix.find(pattern, found + 1)) {
            const auto position = static_cast<std::uint64_t>(found) + 1;
            if (position >= natural) {
                break;
            }
            if (count == 0) {
                first = position;
            }
            positions.push_back(position);
            ++count;
        }
        const auto& actual = results[static_cast<std::size_t>(number)];
        require(actual.first_position == first && actual.early_frequency == count,
                "independent substring comparison for " + pattern);
        require(actual.is_early() == (count != 0) && actual.advance_digits() == natural - first,
                "classification and distance for " + pattern);
        require(mahler::reconstruct_early_positions(number) == positions,
                "reconstruction positions for " + pattern);
    }
}

void known_cases(const std::vector<mahler::EarlyResult>& results) {
    for (const auto [number, first, frequency] : {
             std::array<std::uint64_t, 3>{1, 1, 0}, {10, 10, 0},
             {666, 122, 1}, {7891, 7, 2}, {891, 8, 2},
             {9910, 188, 4}, {1234, 1, 1}}) {
        const auto& record = results[static_cast<std::size_t>(number)];
        require(record.first_position == first && record.early_frequency == frequency,
                "known early case " + std::to_string(number));
    }
    const std::string prefix = mahler::make_prefix(9999);
    for (const auto& [number, expected] : {
             std::pair<std::uint64_t, std::vector<std::uint64_t>>{7891, {7, 6047}},
             {9910, {188, 2619, 2888, 35289}}}) {
        std::vector<std::uint64_t> found;
        const auto natural = results[static_cast<std::size_t>(number)].natural_position;
        const std::string pattern = std::to_string(number);
        for (std::size_t at = prefix.find(pattern); at != std::string::npos;
             at = prefix.find(pattern, at + 1)) {
            if (at + 1 >= natural) {
                break;
            }
            found.push_back(static_cast<std::uint64_t>(at) + 1);
        }
        require(found == expected, "known early positions " + pattern);
    }
}

void group_counts(const std::vector<mahler::EarlyResult>& results) {
    const std::uint64_t bounds[] = {1, 10, 100, 1000, 10000};
    const std::uint64_t expected[] = {0, 45, 630, 6896};
    for (std::size_t group = 0; group < 4; ++group) {
        std::uint64_t count = 0;
        for (auto number = bounds[group]; number < bounds[group + 1]; ++number) {
            count += results[static_cast<std::size_t>(number)].is_early() ? 1ULL : 0ULL;
        }
        require(count == expected[group], "OEIS group count " + std::to_string(group + 1));
    }
}

void million_crosscheck() {
    const auto results = mahler::scan_early(1'000'000);
    const std::uint64_t expected[] = {0, 45, 630, 6896, 73059, 757755, 0};
    std::uint64_t counts[7] = {};
    for (std::uint64_t number = 1; number <= 1'000'000; ++number) {
        const auto& result = results[static_cast<std::size_t>(number)];
        const auto positions = mahler::reconstruct_early_positions(number);
        require(positions.size() == result.early_frequency,
                "million reconstruction frequency " + std::to_string(number));
        require((positions.empty() ? result.natural_position : positions.front())
                    == result.first_position,
                "million reconstruction first position " + std::to_string(number));
        counts[mahler::decimal_digits(number) - 1] += result.is_early() ? 1ULL : 0ULL;
    }
    std::uint64_t total = 0;
    for (std::size_t group = 0; group < 7; ++group) {
        require(counts[group] == expected[group],
                "million external count for width " + std::to_string(group + 1));
        total += counts[group];
    }
    require(total == 838'385, "million early total");

    const std::string prefix = mahler::make_prefix(1'000'000);
    for (const auto number : {991ULL, 919ULL, 9193ULL, 9199ULL,
                              11121ULL, 9090ULL, 900900ULL}) {
        const std::string pattern = std::to_string(number);
        std::vector<std::uint64_t> reference;
        const auto natural = results[static_cast<std::size_t>(number)].natural_position;
        for (std::size_t at = prefix.find(pattern); at != std::string::npos;
             at = prefix.find(pattern, at + 1)) {
            if (at + 1 >= natural) {
                break;
            }
            reference.push_back(static_cast<std::uint64_t>(at) + 1);
        }
        require(mahler::reconstruct_early_positions(number) == reference,
                "selected independent search " + pattern);
    }
}

} // namespace

int main() {
    try {
        try {
            static_cast<void>(mahler::scan_early(0));
            throw std::runtime_error("zero maximum accepted");
        } catch (const std::invalid_argument&) {
        }
        try {
            static_cast<void>(mahler::scan_early(1'000'001));
            throw std::runtime_error("over-limit maximum accepted");
        } catch (const std::length_error&) {
        }
        const auto results = mahler::scan_early(9999);
        compare_independent(9999, results);
        known_cases(results);
        group_counts(results);
        million_crosscheck();
        std::cout << "Early checks passed: independent search, known positions, million crosscheck.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
