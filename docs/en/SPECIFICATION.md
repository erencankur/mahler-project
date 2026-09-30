# Mathematical and API specification

[Türkçe](../tr/SPECIFICATION.md)

Date: 2026-09-30. Phase 0 specification and Phase 1 implementation contract.

## Sequence and position conventions

`S = concat(1,2,3,...)` uses standard decimal representations without leading zeros or separators. Position 1 contains digit 1. The initial `0.` of the Champernowne constant is not part of `S`. `d_n` means a digit position, not the nth concatenated integer.

Define `k(m)` as the number of decimal digits of positive integer `m` and:

```text
B(0) = 0
B(k) = sum(j=1..k, 9 * 10^(j-1) * j)
P(m) = 1 + B(k(m)-1) + k(m) * (m - 10^(k(m)-1))
```

`P(m)` is the start of `m` at its natural place, irrespective of earlier occurrences. The equivalent identity `P(m)=k(m)*m+1-(10^k(m)-1)/9` is recorded in [OEIS A117804](https://oeis.org/A117804). Production arithmetic uses checked digit-group sums to avoid overflow of a larger intermediate product when the final position is representable.

For direct lookup, choose `k` with `B(k-1)<n≤B(k)`, then:

```text
r = n - B(k-1) - 1
source_number = 10^(k-1) + floor(r/k)
digit_offset = r mod k
```

The offset is zero-based from the left. `digit_at(n)` extracts that source digit. Complexity is `O(log n)` digit-group operations with constant storage for fixed-width integers. An overflowing group size is treated as larger than the requested position, never evaluated with unsigned wraparound.

## Early-occurrence definitions for Phase 2

For positive `m`, let `F(m)` be its earliest matching starting position and:

```text
is_early(m) = F(m) < P(m)
E(m) = number of matching starts p < P(m)
A(m) = P(m) - F(m)
```

Count distinct starts, including overlapping occurrences. Exclude the natural occurrence and later occurrences from `E(m)`. Matches may cross multiple source boundaries. A leading-zero window is not the standard decimal representation of a positive target.

The occurrence comparison is on its starting position. Source widths, boundary counts, and carry chains are occurrence-level statistics, not target counts. For non-early targets, `F=P`, `E=0`, and `A=0`.

The fixed research range is `1≤m≤1,000,000`. Its prefix includes the entire source integer 1,000,000 and contains 5,888,896 digits. Each target's natural occurrence is therefore present. First and early occurrences are complete for this target range; total frequency in the infinite word is not calculated.

## Implemented library API

Headers: `include/mahler/sequence.hpp` and `include/mahler/early.hpp`. Namespace: `mahler`.

| API | Result and limits |
| --- | --- |
| `decimal_digits(number)` | Width of any `uint64_t`; the helper returns 1 for zero |
| `locate_digit(position)` | Source integer, zero-based offset, and width for `1..UINT64_MAX` |
| `digit_at(position)` | Integer digit 0–9 for `1..UINT64_MAX` |
| `natural_position(number)` | Natural start if representable as `uint64_t` |
| `make_prefix(last_integer)` | Decimal concatenation `1..last_integer`, for `1..1,000,000` |
| `scan_early(maximum)` | Summary records indexed by target for `1..maximum`, where `1≤maximum≤1,000,000` |
| `reconstruct_early_positions(number)` | Sorted distinct early starts for one target, `1..1,000,000`; secondary research engine |

Zero is rejected by all APIs other than the digit-width helper with `std::invalid_argument`. A prefix/early endpoint over one million throws `std::length_error`. A nonrepresentable natural position throws `std::overflow_error`. Allocation errors propagate to callers. `EarlyResult` contains `natural_position`, `first_position`, `early_frequency`, and derived `is_early()` and `advance_digits()`.

`UINT64_MAX = 18446744073709551615`. The largest target with a representable natural start is `1029360799201087511`, whose start is `18446744073709551599`. The digit at `UINT64_MAX` is 5, at offset 16 within that target. A target beyond this natural-position limit is rejected even if its numeric value fits `uint64_t`.

## CLI and JSON schema version 1

```text
mahler digit <position> [--format text|json]
mahler inspect <number> [--format text|json]
mahler early <number> [--format text|json]  # 1..1000000
mahler scan --max <number> --output <file> [--format csv|json]
            [--occurrences <csv-file>] [--manifest <json-file>]
mahler benchmark --max <number> [--engine window|reconstruct] [--repeat <count>]
mahler analyze --max <number> --output <json-file>
mahler ulam --max <number> --layer <name> --output <svg|png-file> [--cell-size <1..16>]
mahler plot-data --max <number> --output-dir <directory>
mahler legacy-csv --max <number> --output-dir <directory>
mahler --help
mahler --version
```

Default output is text. Arguments use positive decimal integers without signs, whitespace, separators, fractions, or suffixes. Leading zeros are accepted and normalized. Options follow the numeric argument. Unknown or duplicate options are errors.

Exit codes: 0 success; 1 runtime/output error; 2 usage or invalid input; 3 input/natural-position overflow. Successful output goes to stdout; errors go to stderr with no partial result on stdout. CLI labels and errors are English; documentation is bilingual.

| Command | JSON fields |
| --- | --- |
| `digit` | `schema_version` (integer), `position` (decimal string), `digit` (integer), `source_number` (decimal string), `digit_offset` (integer), `digit_count` (integer) |
| `inspect` | `schema_version` (integer), `number` (decimal string), `digit_count` (integer), `natural_position` (decimal string) |
| `early` | `schema_version` (integer), `number` (decimal string), `digit_count` (integer), `natural_position` (decimal string), `first_position` (decimal string), `is_early` (boolean), `early_frequency` (integer), `advance_digits` (decimal string) |

Potentially large positions, targets, and advance distances are decimal strings so consumers can parse them losslessly. `early_frequency` is an integer within the bounded target range. `inspect` retains its Phase 1 schema; use `early` for early-bird fields.

## Implemented dataset contract

`scan` requires `1≤maximum≤1,000,000` and `--output`. Its summary has `number`, `digit_count`, `natural_position`, `first_position`, `is_early`, `early_frequency`, and `advance_digits`; each target has one row, including punctual targets. The default format is CSV. JSON is one object with `schema_version:1`, `maximum` (decimal string), `record_count` (integer), and a `records` array. Special-number fields are reserved for later analysis.

CSV uses UTF-8, English field names, decimal integers without locale separators, and `true`/`false` booleans. JSON preserves target numbers, positions, and advance distances as decimal strings; widths and frequencies are integers. The dataset schema version is independently declared as 1. Uncomputed special-number properties are absent. `--occurrences` optionally writes separate CSV records with `number`, `position`, `first_source`, `last_source`, and `first_source_digit_offset` in target/position order. It records only starts before the natural position. This output uses reconstruction and checks each target's count and first position against the window scan.

The default manifest path is `<output>.manifest.json`; `--manifest` overrides it. Paths must be distinct. The manifest records range, indexing, version, compiler/build type, platform/model, engine choices, counts, phase timings, process peak resident memory, and FNV-1a 64 file checksums. FNV is a reproducibility checksum rather than a cryptographic signature. `benchmark` measures the core engine alone over 1–20 repeats; its result checksum makes comparisons meaningful. Output and checksum read times are excluded from `core_ms`.

`legacy-csv` writes `early-birds.csv`, `prime-early-birds.csv`, and a checksum manifest under `--output-dir`. Both lists are sorted by target and use the columns `number`, `digit_count`, `first_position`, `natural_position`, `early_frequency`, and `advance_digits`; every row is early by construction. The prime list uses the same Eratosthenes sieve as analysis and graphics. Its name denotes compatibility with the two historical list categories: it does not read, modify, or reproduce files in the archive. See [the CSV export guide](../LEGACY_CSV_EXPORTS.md).

`analyze` writes schema version 1 JSON for the same bounded range. It includes counts by target digit width, the complete early-frequency histogram per width, prime/composite/palindrome/emirp/Fibonacci denominators and early counts, extreme first-occurrence values, simple rotation-certificate coverage, occurrence mechanisms, and finite digit-block statistics for lengths 1–3. Primality comes from a Sieve of Eratosthenes; an emirp is a prime with a different reversed decimal prime. `crossed_boundaries` is `last_source-first_source`; `maximum_trailing_nines` is the largest carry length among crossed source increments. Block deviation is `delta_q=max_w |count(w)/(T-q+1)-10^-q|`, with all overlapping blocks including leading zeros. These are finite-prefix descriptions, not normality proofs.

## Source and verification policy

- Digit sequence: [OEIS A033307](https://oeis.org/A033307).
- Natural positions: [OEIS A117804](https://oeis.org/A117804).
- Early-bird membership: [OEIS A116700](https://oeis.org/A116700).
- External count target: [OEIS A160234](https://oeis.org/A160234).
- Research sources and selected graphics tools: [ROADMAP.md](../../ROADMAP.md).

OEIS conventions must be inspected per sequence. An initial zero and a zero-based index can cancel each other's apparent offset; do not adjust positions blindly. The roadmap records literature access on 2026-09-30; online datasets may be extended later.

Normality is an asymptotic property. Finite block deviation `delta_q(T)` and early-frequency `E(m)` are different quantities; neither finite scanning nor graphics proves a general normality theorem. Existing results, new computational findings, and hypotheses will be labeled separately.

Core validation uses independent string concatenation through 10,000, known examples and boundaries, and a wider-arithmetic closed-form oracle for large positions. Only the test oracle uses the Apple Clang/GCC `__uint128_t` extension; production code uses standard C++20 fixed-width arithmetic. Early validation compares all positions with direct substring search through 9,999, compares first positions and frequencies between both engines through one million, and verifies selected carry cases by direct search. Batch tests and the optional standard-library [export verifier](../../scripts/verify_exports.py) check output consistency; see the [Phase 2](../PHASE2_REPORT.md) and [Phase 3](../PHASE3_REPORT.md) reports. Legacy files remain read-only.
