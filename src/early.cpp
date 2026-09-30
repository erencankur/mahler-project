#include "mahler/early.hpp"
#include "mahler/sequence.hpp"

#include <stdexcept>
#include <string>

namespace mahler {

std::vector<EarlyResult> scan_early(std::uint64_t maximum) {
    if (maximum == 0) {
        throw std::invalid_argument("scan maximum must be positive");
    }
    if (maximum > max_prefix_integer) {
        throw std::length_error("scan maximum exceeds the 1000000 limit");
    }

    std::vector<EarlyResult> results(static_cast<std::size_t>(maximum + 1));
    std::uint64_t position = 1;
    for (std::uint64_t number = 1; number <= maximum; ++number) {
        results[static_cast<std::size_t>(number)] = {position, position, 0};
        position += decimal_digits(number);
    }

    const std::string prefix = make_prefix(maximum);
    const unsigned max_width = decimal_digits(maximum);
    for (std::size_t start = 0; start < prefix.size(); ++start) {
        if (prefix[start] == '0') {
            continue;
        }
        std::uint64_t value = 0;
        for (unsigned width = 1; width <= max_width && width <= prefix.size() - start; ++width) {
            value = value * 10 + static_cast<unsigned>(prefix[start + width - 1] - '0');
            if (value > maximum) {
                break;
            }
            auto& record = results[static_cast<std::size_t>(value)];
            const auto occurrence = static_cast<std::uint64_t>(start) + 1;
            if (occurrence < record.natural_position) {
                if (record.early_frequency == 0) {
                    record.first_position = occurrence;
                }
                ++record.early_frequency;
            }
        }
    }
    return results;
}

} // namespace mahler
