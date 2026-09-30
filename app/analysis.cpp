#include "batch.hpp"
#include "cli_util.hpp"
#include "mahler/early.hpp"
#include "mahler/sequence.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <unistd.h>

namespace {

namespace fs = std::filesystem;

struct CountPair {
    std::uint64_t total = 0;
    std::uint64_t early = 0;
};

struct GroupStats {
    std::uint64_t targets = 0;
    std::uint64_t early = 0;
    CountPair primes;
    CountPair composites;
    CountPair palindromes;
    CountPair emirps;
    CountPair fibonacci;
    std::map<std::uint64_t, std::uint64_t> frequency_histogram;
    std::uint64_t largest_advance_number = 0;
    std::uint64_t largest_advance = 0;
    std::uint64_t largest_frequency_number = 0;
    std::uint64_t largest_frequency = 0;
    std::uint64_t smallest_relative_first_number = 0;
    std::uint64_t smallest_relative_first = 0;
    std::uint64_t smallest_relative_natural = 1;
};

template<class Writer>
void write_atomic(const fs::path& target, Writer writer) {
    if (!target.parent_path().empty()) {
        fs::create_directories(target.parent_path());
    }
    const fs::path temporary = target.string() + ".tmp." + std::to_string(getpid());
    try {
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output) {
            throw std::runtime_error("could not open output file: " + temporary.string());
        }
        writer(output);
        output.flush();
        if (!output) {
            throw std::runtime_error("could not write output file: " + temporary.string());
        }
        output.close();
        if (!output) {
            throw std::runtime_error("could not close output file: " + temporary.string());
        }
        fs::rename(temporary, target);
    } catch (...) {
        std::error_code ignored;
        fs::remove(temporary, ignored);
        throw;
    }
}

std::vector<bool> prime_sieve(std::uint64_t maximum) {
    std::vector<bool> prime(static_cast<std::size_t>(maximum + 1), true);
    prime[0] = false;
    if (maximum >= 1) {
        prime[1] = false;
    }
    for (std::uint64_t divisor = 2; divisor <= maximum / divisor; ++divisor) {
        if (!prime[static_cast<std::size_t>(divisor)]) {
            continue;
        }
        for (std::uint64_t multiple = divisor * divisor; multiple <= maximum; multiple += divisor) {
            prime[static_cast<std::size_t>(multiple)] = false;
        }
    }
    return prime;
}

std::uint64_t reverse_decimal(std::uint64_t number) {
    std::uint64_t reversed = 0;
    while (number != 0) {
        reversed = reversed * 10 + number % 10;
        number /= 10;
    }
    return reversed;
}

bool palindrome(std::uint64_t number) {
    return number == reverse_decimal(number);
}

std::vector<bool> fibonacci_members(std::uint64_t maximum) {
    std::vector<bool> members(static_cast<std::size_t>(maximum + 1), false);
    std::uint64_t a = 0;
    std::uint64_t b = 1;
    while (b <= maximum) {
        members[static_cast<std::size_t>(b)] = true;
        const auto next = a + b;
        a = b;
        b = next;
    }
    return members;
}

bool has_rotation_certificate(std::uint64_t number) {
    const std::string digits = std::to_string(number);
    if (digits.size() < 2) {
        return false;
    }
    for (std::size_t split = 1; split < digits.size(); ++split) {
        const std::string rotated = digits.substr(split) + digits.substr(0, split);
        if (rotated.front() == '0' || rotated.back() == '9') {
            continue;
        }
        const auto candidate = parse_positive(rotated);
        if (candidate < number) {
            return true;
        }
    }
    return false;
}

unsigned trailing_nines(std::uint64_t number) {
    unsigned count = 0;
    while (number % 10 == 9) {
        ++count;
        number /= 10;
    }
    return count;
}

unsigned carry_length(std::uint64_t first_source, std::uint64_t last_source) {
    unsigned longest = 0;
    for (std::uint64_t source = first_source; source < last_source; ++source) {
        longest = std::max(longest, trailing_nines(source));
    }
    return longest;
}

std::uint64_t powers_of_ten(unsigned exponent) {
    std::uint64_t result = 1;
    for (unsigned i = 0; i < exponent; ++i) {
        result *= 10;
    }
    return result;
}

struct BlockStats {
    unsigned length = 0;
    std::uint64_t windows = 0;
    std::uint64_t minimum = 0;
    std::uint64_t maximum = 0;
    std::uint64_t most_frequent_block = 0;
    double delta = 0;
};

BlockStats analyze_blocks(const std::string& prefix, unsigned length) {
    const auto states = powers_of_ten(length);
    std::vector<std::uint64_t> counts(static_cast<std::size_t>(states), 0);
    std::uint64_t value = 0;
    for (unsigned index = 0; index < length; ++index) {
        value = value * 10 + static_cast<unsigned>(prefix[index] - '0');
    }
    ++counts[static_cast<std::size_t>(value)];
    const auto modulus = powers_of_ten(length - 1);
    for (std::size_t index = length; index < prefix.size(); ++index) {
        value = (value % modulus) * 10 + static_cast<unsigned>(prefix[index] - '0');
        ++counts[static_cast<std::size_t>(value)];
    }
    const auto [minimum_it, maximum_it] = std::minmax_element(counts.begin(), counts.end());
    const auto windows = static_cast<std::uint64_t>(prefix.size() - length + 1);
    const auto expected = static_cast<double>(windows) / static_cast<double>(states);
    const auto deviation = std::max(static_cast<double>(*maximum_it) - expected,
                                    expected - static_cast<double>(*minimum_it))
        / static_cast<double>(windows);
    return {length, windows, *minimum_it, *maximum_it,
            static_cast<std::uint64_t>(maximum_it - counts.begin()), deviation};
}

void write_pair(std::ostream& out, const CountPair& pair) {
    out << "{\"total\":" << pair.total << ",\"early\":" << pair.early << '}';
}

void write_histogram(std::ostream& out, const std::map<std::uint64_t, std::uint64_t>& histogram) {
    out << '[';
    bool first = true;
    for (const auto& [frequency, count] : histogram) {
        if (!first) {
            out << ',';
        }
        first = false;
        out << "{\"frequency\":" << frequency << ",\"targets\":" << count << '}';
    }
    out << ']';
}

} // namespace

void run_analyze_command(int argc, char** argv) {
    std::optional<std::uint64_t> maximum;
    std::optional<fs::path> output;
    if (argc != 6) {
        throw std::invalid_argument("analyze requires --max and --output with option values");
    }
    for (int i = 2; i < argc; i += 2) {
        const std::string_view option = argv[i];
        const std::string value = argv[i + 1];
        if (option == "--max" && !maximum) {
            maximum = parse_positive(value);
            if (*maximum > mahler::max_prefix_integer) {
                throw std::invalid_argument("analysis maximum exceeds the 1000000 limit");
            }
        } else if (option == "--output" && !output && !value.empty()) {
            output = value;
        } else {
            throw std::invalid_argument("unknown, duplicate, or invalid analyze option");
        }
    }
    if (!maximum || !output) {
        throw std::invalid_argument("analyze requires --max and --output");
    }

    const auto results = mahler::scan_early(*maximum);
    const auto primes = prime_sieve(*maximum);
    const auto fibonacci = fibonacci_members(*maximum);
    const auto max_width = mahler::decimal_digits(*maximum);
    std::vector<GroupStats> groups(max_width + 1);
    std::uint64_t rotation_certificates = 0;
    std::uint64_t early_rotation_certificates = 0;
    for (std::uint64_t number = 1; number <= *maximum; ++number) {
        const auto width = mahler::decimal_digits(number);
        auto& group = groups[width];
        const auto& result = results[static_cast<std::size_t>(number)];
        const auto early = result.is_early();
        ++group.targets;
        group.early += early ? 1ULL : 0ULL;
        ++group.frequency_histogram[result.early_frequency];
        const auto advance = result.advance_digits();
        if (advance > group.largest_advance) {
            group.largest_advance = advance;
            group.largest_advance_number = number;
        }
        if (result.early_frequency > group.largest_frequency) {
            group.largest_frequency = result.early_frequency;
            group.largest_frequency_number = number;
        }
        if (early && (group.smallest_relative_first_number == 0
                || result.first_position * group.smallest_relative_natural
                    < group.smallest_relative_first * result.natural_position)) {
            group.smallest_relative_first_number = number;
            group.smallest_relative_first = result.first_position;
            group.smallest_relative_natural = result.natural_position;
        }
        if (primes[static_cast<std::size_t>(number)]) {
            ++group.primes.total;
            group.primes.early += early ? 1ULL : 0ULL;
        } else if (number > 1) {
            ++group.composites.total;
            group.composites.early += early ? 1ULL : 0ULL;
        }
        if (palindrome(number)) {
            ++group.palindromes.total;
            group.palindromes.early += early ? 1ULL : 0ULL;
        }
        const auto reversed = reverse_decimal(number);
        if (primes[static_cast<std::size_t>(number)] && reversed != number && reversed <= *maximum
            && primes[static_cast<std::size_t>(reversed)]) {
            ++group.emirps.total;
            group.emirps.early += early ? 1ULL : 0ULL;
        }
        if (fibonacci[static_cast<std::size_t>(number)]) {
            ++group.fibonacci.total;
            group.fibonacci.early += early ? 1ULL : 0ULL;
        }
        if (has_rotation_certificate(number)) {
            ++rotation_certificates;
            early_rotation_certificates += early ? 1ULL : 0ULL;
            if (!early) {
                throw std::runtime_error("rotation certificate failed for " + std::to_string(number));
            }
        }
    }

    std::vector<std::uint64_t> source_width(max_width + 1, 0);
    std::vector<std::uint64_t> boundaries(max_width + 1, 0);
    std::vector<std::uint64_t> carries(max_width + 1, 0);
    std::uint64_t occurrences = 0;
    for (std::uint64_t number = 1; number <= *maximum; ++number) {
        const auto positions = mahler::reconstruct_early_positions(number);
        const auto& expected = results[static_cast<std::size_t>(number)];
        if (positions.size() != expected.early_frequency) {
            throw std::runtime_error("analysis engines disagree at target " + std::to_string(number));
        }
        for (const auto position : positions) {
            const auto first = mahler::locate_digit(position);
            const auto last = mahler::locate_digit(position + mahler::decimal_digits(number) - 1);
            ++source_width[first.digit_count];
            const auto crossed = last.source_number - first.source_number;
            if (crossed >= boundaries.size()) {
                boundaries.resize(static_cast<std::size_t>(crossed + 1), 0);
            }
            ++boundaries[static_cast<std::size_t>(crossed)];
            const auto carry = carry_length(first.source_number, last.source_number);
            if (carry >= carries.size()) {
                carries.resize(static_cast<std::size_t>(carry + 1), 0);
            }
            ++carries[carry];
            ++occurrences;
        }
    }

    std::uint64_t early_total = 0;
    for (unsigned width = 1; width <= max_width; ++width) {
        early_total += groups[width].early;
    }
    const auto prefix = mahler::make_prefix(*maximum);
    std::vector<BlockStats> blocks;
    for (unsigned length = 1; length <= 3 && length <= prefix.size(); ++length) {
        blocks.push_back(analyze_blocks(prefix, length));
    }
    write_atomic(*output, [&](std::ostream& out) {
        out << "{\n  \"schema_version\":1,\n"
            << "  \"maximum\":\"" << *maximum << "\",\n"
            << "  \"target_count\":" << *maximum << ",\n"
            << "  \"early_target_count\":";
        out << early_total << ",\n  \"early_occurrence_count\":" << occurrences << ",\n"
            << "  \"by_digit_count\":[\n";
        for (unsigned width = 1; width <= max_width; ++width) {
            const auto& group = groups[width];
            if (width != 1) {
                out << ",\n";
            }
            out << "    {\"digits\":" << width << ",\"targets\":" << group.targets
                << ",\"early\":" << group.early << ",\"punctual\":" << group.targets - group.early
                << ",\"primes\":";
            write_pair(out, group.primes);
            out << ",\"composites\":";
            write_pair(out, group.composites);
            out << ",\"palindromes\":";
            write_pair(out, group.palindromes);
            out << ",\"emirps\":";
            write_pair(out, group.emirps);
            out << ",\"fibonacci\":";
            write_pair(out, group.fibonacci);
            out << ",\"frequency_histogram\":";
            write_histogram(out, group.frequency_histogram);
            out << ",\"largest_advance\":{\"number\":\"" << group.largest_advance_number
                << "\",\"digits\":\"" << group.largest_advance << "\"}"
                << ",\"largest_frequency\":{\"number\":\"" << group.largest_frequency_number
                << "\",\"frequency\":" << group.largest_frequency << "}"
                << ",\"smallest_relative_first\":{\"number\":\"" << group.smallest_relative_first_number
                << "\",\"first_position\":\"" << group.smallest_relative_first
                << "\",\"natural_position\":\"" << group.smallest_relative_natural << "\"}}";
        }
        out << "\n  ],\n  \"rotation_certificates\":{\"targets\":" << rotation_certificates
            << ",\"early_targets\":" << early_rotation_certificates << "},\n"
            << "  \"occurrence_mechanisms\":{\"source_digit_count\":[";
        for (std::size_t i = 1; i < source_width.size(); ++i) {
            if (i != 1) out << ',';
            out << "{\"value\":" << i << ",\"occurrences\":" << source_width[i] << '}';
        }
        out << "],\"crossed_boundaries\":[";
        for (std::size_t i = 1; i < boundaries.size(); ++i) {
            if (i != 1) out << ',';
            out << "{\"value\":" << i << ",\"occurrences\":" << boundaries[i] << '}';
        }
        out << "],\"maximum_trailing_nines\":[";
        for (std::size_t i = 0; i < carries.size(); ++i) {
            if (i != 0) out << ',';
            out << "{\"value\":" << i << ",\"occurrences\":" << carries[i] << '}';
        }
        out << "]},\n  \"digit_blocks\":[";
        for (std::size_t index = 0; index < blocks.size(); ++index) {
            const auto& block = blocks[index];
            if (index != 0) out << ',';
            out << "{\"length\":" << block.length << ",\"windows\":\"" << block.windows
                << "\",\"minimum_count\":" << block.minimum << ",\"maximum_count\":" << block.maximum
                << ",\"most_frequent_block\":\"" << std::setw(static_cast<int>(block.length)) << std::setfill('0')
                << block.most_frequent_block << std::setfill(' ') << "\",\"delta\":"
                << std::fixed << std::setprecision(12) << block.delta << '}';
        }
        out << "]\n}\n";
    });
    std::cout << "analysis: " << output->string() << '\n'
              << "targets: " << *maximum << '\n'
              << "early_targets: " << early_total << '\n'
              << "early_occurrences: " << occurrences << '\n';
}
