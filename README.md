# Mahler Project

[Türkçe](README.tr.md)

A C++20 mathematical research project for the decimal Champernowne constant, also known as Mahler's number:

```text
0.123456789101112131415...
```

The working foundation provides direct digit lookup and natural-position queries. The planned research examines early-bird status and early-occurrence frequencies for every integer from 1 through 1,000,000, with prime analysis and Ulam spiral figures.

## Current capabilities

- Find any digit at a positive `uint64_t` position without constructing preceding digits.
- Locate its source integer and zero-based digit offset through JSON output.
- Find a positive integer's natural starting position, with checked overflow.
- Generate bounded prefixes through the C++ library for subsequent validation and scanning.
- Produce text or JSON query output.

Early-bird classification, first-occurrence searches, frequencies, batch CSV/JSON exports, and figures are **not implemented yet**. `inspect` currently reports natural position only. An absent early-bird field does not mean the number is punctual.

## Build on macOS

Requirements: Xcode Command Line Tools (or Xcode), CMake 3.20 or newer, and a C++20 compiler. The initial supported environment is Apple Clang on macOS. No Python, Gnuplot, or PNG library is required for this phase.

If the command-line developer tools are missing, install them using `xcode-select --install`. Obtain CMake from its [official download page](https://cmake.org/download/) or your package manager.

Run from the repository root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Usage

```sh
./build/mahler digit 2020
# 7

./build/mahler digit 2020 --format json
# {"schema_version":1,"position":"2020","digit":7,"source_number":"710","digit_offset":0,"digit_count":3}

./build/mahler inspect 9910
# number: 9910
# digit_count: 4
# natural_position: 38530

./build/mahler inspect 9910 --format json
# {"schema_version":1,"number":"9910","digit_count":4,"natural_position":"38530"}

./build/mahler --help
./build/mahler --version
```

Positions start at 1 and exclude the initial `0.`. JSON serializes position and target/source integers as decimal strings to preserve precision in consumers such as JavaScript. Digits, offsets, widths, and schema versions are JSON numbers.

## Validation

Core checks cover report examples, every position in an independently constructed prefix through 10,000, large positions and natural-position overflow, and the 5,888,896-digit prefix through 1,000,000. CLI checks cover exact text/JSON outputs, invalid input, and exit codes. These checks validate foundational queries, **not a million-target early-bird scan**.

To run additional memory and arithmetic checks with Apple Clang:

```sh
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DMAHLER_ENABLE_SANITIZERS=ON
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure
```

## Mathematics and development plan

- [Mathematical and API specification](docs/en/SPECIFICATION.md)
- [Bilingual roadmap, research references, and graphics decisions](ROADMAP.md)
- [OEIS A033307: Champernowne digits](https://oeis.org/A033307)
- [OEIS A117804: natural positions](https://oeis.org/A117804)
- [OEIS A116700: early-bird numbers](https://oeis.org/A116700)

The previous mathematical study is retained in the sibling `../legacy-math-project/` archive. The application has no runtime dependency on that archive and does not modify it.

## Release status and licensing

This is the development foundation for `0.1.0`, not a completed research release. A license will be selected before public release; no open-source license has been granted yet.
