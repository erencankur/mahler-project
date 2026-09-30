#pragma once
#include <cstdint>
#include <vector>

namespace mahler {
struct Point { int x; int y; };
// 1=(0,0), 2=(1,0), 3=(1,1); mathematical y points upward.
[[nodiscard]] Point ulam_point(std::uint64_t number);
[[nodiscard]] unsigned ulam_side(std::uint64_t maximum);
[[nodiscard]] std::vector<bool> prime_flags(std::uint64_t maximum);
} // namespace mahler
