#pragma once

#include <cstdint>
#include <vector>

namespace mahler {

struct EarlyResult {
    std::uint64_t natural_position;
    std::uint64_t first_position;
    std::uint64_t early_frequency;

    [[nodiscard]] bool is_early() const noexcept { return early_frequency != 0; }
    [[nodiscard]] std::uint64_t advance_digits() const noexcept {
        return natural_position - first_position;
    }
};

// Scan concatenation 1..maximum. Entry zero is unused. Matches are classified
// by their starting position, including overlaps and source-number crossings.
// The supported target range is 1..1,000,000.
[[nodiscard]] std::vector<EarlyResult> scan_early(std::uint64_t maximum);

// Experimental per-target engine. Reconstructs possible source numbers from
// the target's suffix/prefix fragments and returns distinct early positions.
[[nodiscard]] std::vector<std::uint64_t> reconstruct_early_positions(std::uint64_t number);

} // namespace mahler
