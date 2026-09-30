#include "mahler/ulam.hpp"
#include "mahler/sequence.hpp"
#include <stdexcept>

namespace mahler {
unsigned ulam_side(std::uint64_t maximum) {
    if (!maximum || maximum > max_prefix_integer)
        throw std::invalid_argument("Ulam range must be 1..1000000");
    unsigned side = 1;
    while (static_cast<std::uint64_t>(side) * side < maximum) side += 2;
    return side;
}
Point ulam_point(std::uint64_t number) {
    const auto side = ulam_side(number);
    const int r = static_cast<int>(side / 2);
    if (!r) return {0, 0};
    const auto offset = static_cast<int>(static_cast<std::uint64_t>(side) * side - number);
    const int edge = 2 * r;
    if (offset < edge) return {r - offset, -r};
    if (offset < 2 * edge) return {-r, -r + offset - edge};
    if (offset < 3 * edge) return {-r + offset - 2 * edge, r};
    return {r, r - (offset - 3 * edge)};
}
std::vector<bool> prime_flags(std::uint64_t maximum) {
    if (!maximum || maximum > max_prefix_integer)
        throw std::invalid_argument("prime range must be 1..1000000");
    std::vector<bool> flags(static_cast<std::size_t>(maximum + 1), true);
    flags[0] = flags[1] = false;
    for (std::uint64_t p = 2; p <= maximum / p; ++p)
        if (flags[static_cast<std::size_t>(p)])
            for (auto m = p * p; m <= maximum; m += p)
                flags[static_cast<std::size_t>(m)] = false;
    return flags;
}
} // namespace mahler
