#!/bin/bash
# Run from the repository root. Override GNUPLOT and MAHLER for local tools.
set -euo pipefail
data=${1:-results/figures/data}
output=${2:-results/figures/charts}
gnuplot_bin=${GNUPLOT:-gnuplot}
mahler_bin=${MAHLER:-./build/mahler}
command -v "$gnuplot_bin" >/dev/null || { echo 'Gnuplot 6.x with svg/pngcairo/pdfcairo is required.' >&2; exit 1; }
command -v jq >/dev/null || { echo 'jq is required to extract benchmark JSON.' >&2; exit 1; }
mkdir -p "$data" "$output"
"$mahler_bin" plot-data --max 1000000 --output-dir "$data"
printf 'engine,maximum,median_ms,peak_rss_mib\n' > "$data/performance.csv"
for engine in window reconstruct; do
    for maximum in 10000 100000 1000000; do
        "$mahler_bin" benchmark --max "$maximum" --engine "$engine" --repeat 3 > "$data/benchmark-$engine-$maximum.json"
        jq -r '[.engine,.maximum,.median_core_ms,(.peak_rss_bytes/1048576)]|@csv' "$data/benchmark-$engine-$maximum.json" >> "$data/performance.csv"
    done
done
for lang in en tr; do
    for format in svg png pdf; do
        "$gnuplot_bin" -c scripts/figures.gp "$data" "$output" "$lang" "$format"
    done
done
for layer in early prime intersection combined punctual frequency relative; do
    "$mahler_bin" ulam --max 10000 --layer "$layer" --output "$output/ulam-$layer-10000.svg" --cell-size 4
    "$mahler_bin" ulam --max 1000000 --layer "$layer" --output "$output/ulam-$layer-1000000.png"
done
"$mahler_bin" ulam --max 1000000 --layer combined --output "$output/ulam-combined-detail.png" --cell-size 4
"$mahler_bin" ulam --max 25 --layer combined --output "$output/ulam-25.svg" --cell-size 16
{
    "$gnuplot_bin" --version
    "$mahler_bin" --version
    printf 'Font: Arial; SVG 1200x800; PNG 2400x1600; PDF 12x8 inches\n'
    uname -sm
} > "$output/tools.txt"
(cd "$data" && shasum -a 256 ./*.csv ./*.json) > "$output/data-sha256.txt"
(cd "$output" && shasum -a 256 ./*.svg ./*.png ./*.pdf) > "$output/figure-sha256.txt"
