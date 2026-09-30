#include "mahler/early.hpp"
#include "mahler/sequence.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <vector>

namespace mahler {
namespace {

constexpr std::array<std::uint64_t, 9> powers = {
    1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000};

std::uint64_t decimal_value(const std::string& digits, unsigned begin, unsigned length) {
    std::uint64_t value = 0;
    for (unsigned i = 0; i < length; ++i) {
        value = value * 10 + static_cast<unsigned>(digits[begin + i] - '0');
    }
    return value;
}

bool matches_from(std::uint64_t source, unsigned suffix_length, const std::string& target) {
    std::string candidate = std::to_string(source);
    candidate.erase(0, candidate.size() - suffix_length);
    for (auto next = source + 1; candidate.size() < target.size(); ++next) {
        candidate += std::to_string(next);
    }
    return candidate.compare(0, target.size(), target) == 0;
}

} // namespace

std::vector<std::uint64_t> reconstruct_early_positions(std::uint64_t number) {
    if (number == 0) {
        throw std::invalid_argument("target number must be positive");
    }
    if (number > max_prefix_integer) {
        throw std::length_error("reconstruction target exceeds the 1000000 limit");
    }
    const std::string target = std::to_string(number);
    const unsigned width = static_cast<unsigned>(target.size());
    const auto natural = natural_position(number);
    std::vector<std::uint64_t> positions;

    // An early match must start in a source a<number and cross its end.
    // s digits of the target are the suffix of a; the next t digits constrain
    // the prefix of a+1. Intersect these two decimal conditions exactly.
    for (unsigned suffix = 1; suffix < width; ++suffix) {
        const auto suffix_value = decimal_value(target, 0, suffix);
        const auto step = powers[suffix];
        const auto following = width - suffix;
        for (unsigned source_width = suffix; source_width <= width; ++source_width) {
            for (unsigned next_width = source_width; next_width <= source_width + 1; ++next_width) {
                const unsigned prefix_width = std::min(following, next_width);
                if (target[suffix] == '0') {
                    continue; // A positive source number has no leading zero.
                }
                const auto prefix_value = decimal_value(target, suffix, prefix_width);
                const auto factor = powers[next_width - prefix_width];
                const auto next_low = prefix_value * factor;
                const auto next_high = (prefix_value + 1) * factor - 1;

                // Restrict a's width, a+1's width, a<number, and the
                // observed prefix of a+1, before applying a mod 10^s.
                const auto low = std::max({powers[source_width - 1],
                                           powers[next_width - 1] - 1,
                                           next_low - 1});
                const auto high = std::min({powers[source_width] - 1,
                                            powers[next_width] - 2,
                                            number - 1,
                                            next_high - 1});
                if (low > high) {
                    continue;
                }
                const auto remainder = low % step;
                const auto adjustment = (suffix_value + step - remainder) % step;
                for (auto source = low + adjustment; source <= high; source += step) {
                    if (!matches_from(source, suffix, target)) {
                        continue;
                    }
                    const auto position = natural_position(source) + source_width - suffix;
                    if (position < natural) {
                        positions.push_back(position);
                    }
                }
            }
        }
    }
    std::sort(positions.begin(), positions.end());
    positions.erase(std::unique(positions.begin(), positions.end()), positions.end());
    return positions;
}

} // namespace mahler
