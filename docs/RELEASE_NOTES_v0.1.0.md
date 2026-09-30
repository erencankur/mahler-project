# Mahler Project v0.1.0

**EN:** Initial public release of a C++20 research tool for the decimal Champernowne constant (Mahler's number) and early-bird numbers. It provides direct digit lookup, natural positions, exact early-occurrence frequencies for `1..1,000,000`, reproducible exports and analyses, and Ulam/figure tooling.

**TR:** Ondalık Champernowne sabiti (Mahler sayısı) ve erkenci sayılar için C++20 araştırma aracının ilk public sürümü. Doğrudan rakam sorgusu, doğal konum, `1..1.000.000` aralığında kesin erken görünüm frekansları, yeniden üretilebilir dışa aktarımlar/analizler ve Ulam/grafik araçları sunar.

## Highlights / Öne çıkanlar

- Arithmetic `digit` and `inspect` queries without constructing preceding digits.
- Verified `early`, `scan`, `benchmark`, `analyze`, `plot-data`, and `ulam` commands.
- Two independently cross-checked early-occurrence engines; the one-million scan reports 838,385 early-bird targets and 2,688,255 early positions.
- Bilingual specifications, phase reports, selected figures, source tables, and reproducibility records.
- macOS GitHub Actions coverage for the core build and optional libpng PNG export.

## Build / Derleme

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/mahler early 9910 --format json
```

For optional PNG export, configure with `-DMAHLER_ENABLE_GRAPHICS=ON` after installing libpng. The full graphics workflow is documented in [Phase 5](PHASE5_REPORT.md); the clean macOS workflow is in the [release checklist](RELEASE_CHECKLIST.md).

## Compatibility and scope / Uyumluluk ve kapsam

- Supported release environment: macOS, a C++20 compiler, and CMake 3.20 or newer.
- Positions are one-based and omit the initial `0.`.
- Machine-readable outputs use schema version `1`.
- The investigated mathematical range is `1..1,000,000`; visual patterns are finite-range observations, not general proofs.

## License and citation / Lisans ve atıf

Released under the [MIT License](../LICENSE). Academic users can cite the repository through [CITATION.cff](../CITATION.cff).

## Included release assets / Dahil sürüm dosyaları

GitHub's source archive contains the C++ sources, tests, bilingual documentation, selected figures, compact plot tables, and checksums. Full one-million CSV/JSON exports and the complete high-resolution figure set are regenerated with the documented commands and are intentionally not duplicated in the source archive.
