# Mahler Project

[Türkçe](README.tr.md)

A C++20 mathematical research project for the decimal Champernowne constant, also known as Mahler's number:

```text
0.123456789101112131415...
```

The C++ program provides direct digit lookup, early-bird searches, reproducible CSV/JSON exports, mathematical analyses, and Ulam figures for targets through 1,000,000.

## Current capabilities

- Find any digit at a positive `uint64_t` position without constructing preceding digits.
- Locate its source integer and zero-based digit offset through JSON output.
- Find a positive integer's natural starting position, with checked overflow.
- Generate bounded prefixes through the C++ library for subsequent validation and scanning.
- Produce text or JSON query output.
- Compute first occurrence, early-bird status, early frequency, and advance distance with `early`.
- Cross-check numeric-window scanning against an independent per-target reconstruction engine.
- Export target summaries, optional early occurrence records, and a run manifest with checksums.
- Measure both engines without including file output in core timing.
- Analyze frequencies, special number sets, source-boundary mechanisms, rotation certificates, and finite digit blocks.

- Generate seven Ulam layers as small SVGs or optional libpng PNGs.
- Produce bilingual statistical SVG/PNG/PDF charts with Gnuplot.

`inspect` reports natural position only; use `early` for early-bird properties.

## Run it on macOS

Requirements: Xcode Command Line Tools (or Xcode), CMake 3.20 or newer, and a C++20 compiler. The initial supported environment is Apple Clang on macOS. Core calculations and SVG spirals need no Python, Gnuplot, or PNG library. Optional PNG export requires libpng; statistical rendering requires Gnuplot 6.x and jq.

Install the command-line developer tools if needed, then install CMake. Homebrew is a convenient macOS route:

```sh
xcode-select --install
brew install cmake
```

Clone, build, test, and make a first query. These commands work from a new Terminal window; the `xcode-select` installer may require reopening it after installation.

```sh
git clone https://github.com/erencankur/mahler-project.git
cd mahler-project
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/mahler digit 2020
# 7
```

Use `./build/mahler --help` to display the current command contract. Rebuild after a source change with `cmake --build build --parallel`; CMake regenerates its build files when necessary. For a clean rebuild, remove only the generated build directory:

```sh
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

To install the executable outside the repository, choose a writable prefix:

```sh
cmake --install build --prefix /tmp/mahler-install
/tmp/mahler-install/bin/mahler digit 2020
```

## Commands and research workflow

All positions start at 1 and exclude the initial `0.`. Each `<number>` and `<position>` is a positive decimal integer. The early-bird analysis range is deliberately capped at 1,000,000.

| Command | What it does |
| --- | --- |
| `digit <position>` | Finds the digit at a sequence position without making the preceding sequence. |
| `inspect <number>` | Finds the number's natural start. |
| `early <number>` | Reports first start, early status, count of early starts, and advance. |
| `scan --max N --output FILE` | Writes one summary record for every target `1..N`; optionally writes every early occurrence. |
| `legacy-csv --max N --output-dir DIR` | Writes filtered all-early and prime-early lists, designed to replace the historical list format. |
| `analyze --max N --output FILE` | Writes aggregate mathematical statistics as JSON. |
| `plot-data --max N --output-dir DIR` | Writes compact aggregate CSV tables for plotting. |
| `ulam --max N --layer NAME --output FILE` | Draws one Ulam layer as SVG, or PNG when graphics support is enabled. |
| `benchmark --max N` | Measures the primary or independent reconstruction engine. |

```sh
# Direct questions: plain text is easy to read; JSON is for programs.
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

./build/mahler early 9910 --format json
# {"schema_version":1,"number":"9910","digit_count":4,"natural_position":"38530","first_position":"188","is_early":true,"early_frequency":4,"advance_digits":"38342"}

# Complete target table and a separate table of early occurrences.
./build/mahler scan --max 1000000 --format csv --output results/summary-1000000.csv --occurrences results/occurrences-1000000.csv
./build/mahler scan --max 1000000 --format json --output results/summary-1000000.json

# Historical-list-compatible outputs: all early birds and the prime subset.
./build/mahler legacy-csv --max 1000000 --output-dir results/legacy-compatible

# Aggregates and plotting source tables.
./build/mahler benchmark --max 1000000 --engine window --repeat 3
./build/mahler analyze --max 1000000 --output results/analysis-1000000.json
./build/mahler ulam --max 25 --layer combined --cell-size 16 --output results/ulam-25.svg
./build/mahler plot-data --max 1000000 --output-dir results/plot-data

./build/mahler --help
./build/mahler --version
```

Positions start at 1 and exclude the initial `0.`. JSON serializes position and target/source integers as decimal strings to preserve precision in consumers such as JavaScript. Digits, offsets, widths, and schema versions are JSON numbers.

### CSV and JSON files

Write experimental output below `results/`; it is ignored by Git so full million-target files never enter a source commit. `scan` creates its manifest beside the summary unless `--manifest` supplies a different path. The manifest includes the range, machine/build context, record counts, timings, and FNV-1a-64 checksums.

| Output | Rows at `--max 1000000` | Use |
| --- | ---: | --- |
| `summary-1000000.csv` | 1,000,000 | Full target-level table, including non-early numbers. |
| `occurrences-1000000.csv` | 2,688,255 | Each early start, with source-number boundary information. |
| `legacy-compatible/early-birds.csv` | 838,385 | All early targets, in target order. |
| `legacy-compatible/prime-early-birds.csv` | 66,388 | Prime early targets, in target order. |
| `analysis-1000000.json` | one JSON document | Frequency, prime, mechanism, and finite-block summaries. |
| `plot-data/*.csv` | ten aggregate tables | Reproducible inputs for the statistical charts. |

The two legacy-compatible CSV files use `number,digit_count,first_position,natural_position,early_frequency,advance_digits`. Every row is early; `early_frequency` counts distinct starts before the natural one, including overlaps. Their `manifest.json` checks both files. See the complete [CSV export guide](docs/LEGACY_CSV_EXPORTS.md), including a small example and field definitions.

To verify a complete export independently, use the included standard-library script after creating the summary and occurrence files:

```sh
python3 scripts/verify_exports.py \
  --summary results/summary-1000000.csv \
  --occurrences results/occurrences-1000000.csv
```

## Validation

Core checks cover an independently constructed prefix through 10,000, large positions, and overflow. Early-bird checks compare all early positions against direct substring searches through 9,999, compare both engines' first positions and frequencies for every target through one million, and check selected carry cases independently. The one-million scan found **838,385** early birds and **2,688,255** early occurrence positions. Batch tests verify output schemas, repetition, and invalid options. The optional [export verifier](scripts/verify_exports.py) checks complete CSV/JSON outputs and every occurrence against the digit sequence. Results and measurements are in the [Phase 3 report](docs/PHASE3_REPORT.md).

The [Phase 4 analysis](docs/PHASE4_REPORT.md) documents prime and special-subset denominators, frequency distributions, mechanisms, and finite block statistics. [Phase 5](docs/PHASE5_REPORT.md) adds a million-coordinate geometry test, decoded PNG checks, aggregate verification, and figures.

## Figures

![Early proportions by digit length](figures/charts/early-by-digits-en.svg)

![Combined Ulam spiral, one million targets](figures/charts/ulam-combined-1000000.png)

Blue: early only; orange: prime only; green: both; light gray: neither; dark gray: outside 1..1,000,000. The spiral starts at 1 in the center, steps right, then up. It contains **66,388** prime early birds. See the [figure catalog and data](figures/README.md) for scales, source tables, and all seven layers.

```sh
# Optional full graphics build and reproduction
brew install libpng gnuplot jq
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DMAHLER_ENABLE_GRAPHICS=ON
cmake --build build --parallel
bash scripts/render_figures.sh
```

Full outputs stay under ignored `results/figures/`; selected figures and small source tables are versioned. No Python is required to generate them.

To run additional memory and arithmetic checks with Apple Clang:

```sh
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DMAHLER_ENABLE_SANITIZERS=ON
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure
```

## Mathematics and development plan

- [Mathematical and API specification](docs/en/SPECIFICATION.md)
- [Bilingual roadmap, research references, and graphics decisions](ROADMAP.md)
- [Phase 2 verification and archived-list comparison](docs/PHASE2_REPORT.md)
- [Phase 3 dataset, checksums, and measurements](docs/PHASE3_REPORT.md)
- [Phase 4 mathematical analysis and examples](docs/PHASE4_REPORT.md)
- [Phase 5 figures, geometry, algorithms, and interpretation](docs/PHASE5_REPORT.md)
- [Contributing](CONTRIBUTING.md)
- [Changelog](CHANGELOG.md)
- [OEIS A033307: Champernowne digits](https://oeis.org/A033307)
- [OEIS A117804: natural positions](https://oeis.org/A117804)
- [OEIS A116700: early-bird numbers](https://oeis.org/A116700)

The previous mathematical study is retained in the sibling `../legacy-math-folders/` archive. The application has no runtime dependency on that archive and does not modify it.

## Release status and licensing

Mahler Project `v0.1.0` is released under the [MIT License](LICENSE). The tagged release is available on [GitHub](https://github.com/erencankur/mahler-project/releases/tag/v0.1.0).
