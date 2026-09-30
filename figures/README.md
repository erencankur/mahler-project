# Figure catalog / Grafik kataloğu

**TR:** Aralık `1..1000000`, taban 10, konumlar 1 tabanlıdır. C++ hesaplamalarıyla üretilen küçük [kaynak tabloları](data/) ve seçilmiş görseller bu dizinde bulunur. Renkler, katmanlar ve yeniden üretim komutları aşağıda açıklanır. Tam seti `bash scripts/render_figures.sh` ile `results/figures/` altında üret.

**EN:** Scope is `1..1000000`, base 10, with 1-based positions. This directory contains small C++-generated [source tables](data/) and selected figures. Colors, layers, and reproduction commands are explained below. Regenerate the complete set under `results/figures/` with `bash scripts/render_figures.sh`.

## Statistical charts / İstatistik grafikleri

| Topic / Konu | English | Türkçe | Source / Kaynak |
|---|---|---|---|
| Early proportions / Erkenci oranı | [SVG](charts/early-by-digits-en.svg) | [SVG](charts/early-by-digits-tr.svg) | [groups](data/groups.csv) |
| Density / Yoğunluk | [SVG](charts/density-en.svg) | [SVG](charts/density-tr.svg) | [density](data/density.csv) |
| Frequency / Frekans | [SVG](charts/frequency-en.svg) | [SVG](charts/frequency-tr.svg) | [frequency](data/frequency.csv) |
| Relative ECDF / Göreli ECDF | [SVG](charts/relative-ecdf-en.svg) | [SVG](charts/relative-ecdf-tr.svg) | [ecdf](data/ecdf.csv) |
| Primes / Asallar | [SVG](charts/prime-comparison-en.svg) | [SVG](charts/prime-comparison-tr.svg) | [groups](data/groups.csv) |
| First positions / İlk konumlar | [SVG](charts/first-position-heatmap-en.svg) | [SVG](charts/first-position-heatmap-tr.svg) | [heatmap](data/heatmap.csv) |
| Mechanisms / Mekanizmalar | [SVG](charts/mechanisms-en.svg) | [SVG](charts/mechanisms-tr.svg) | [mechanisms](data/mechanisms.csv) |
| Block deviation / Blok sapması | [SVG](charts/block-deviation-en.svg) | [SVG](charts/block-deviation-tr.svg) | [blocks](data/blocks.csv) |
| Digit shares / Rakam payı | [SVG](charts/digit-shares-en.svg) | [SVG](charts/digit-shares-tr.svg) | [digit shares](data/digit-shares.csv) |
| Rings / Halkalar | [SVG](charts/ulam-rings-en.svg) | [SVG](charts/ulam-rings-tr.svg) | [rings](data/rings.csv) |
| Diagonals / Köşegenler | [SVG](charts/ulam-diagonals-en.svg) | [SVG](charts/ulam-diagonals-tr.svg) | [diagonals](data/diagonals.csv) |
| Performance / Performans | [SVG](charts/performance-en.svg) | [SVG](charts/performance-tr.svg) | [performance](data/performance.csv) |

**TR:** SVG 1200×800, Arial, beyaz zemin. Tam üretimde 2400×1600 PNG ve 12×8 inç PDF sürümleri de vardır. ECDF 0.001 aralıklı eşiklerde hesaplanır; ısı haritası 50×50 logaritmik hücredir. Yedi basamak grubu yalnızca 1.000.000 sayısını içerir. Rakam payları ve blok sapmaları gözlenen önek uçlarında işaretlenir; sıfır başlıklı bloklar da sayılır.

**EN:** SVG is 1200×800, Arial, white background. Full generation also exports 2400×1600 PNG and 12×8-inch PDF. ECDF is evaluated at thresholds spaced by 0.001; the heatmap uses 50×50 logarithmic bins. The seven-digit group contains only 1,000,000. Digit proportions and block deviations mark observed prefix endpoints, including leading-zero blocks.

## Ulam layers / Ulam katmanları

| Layer / Katman | PNG, 1001×1001 |
|---|---|
| Early / Erkenci | [PNG](charts/ulam-early-1000000.png) |
| Prime / Asal | [PNG](charts/ulam-prime-1000000.png) |
| Intersection / Kesişim | [PNG](charts/ulam-intersection-1000000.png) |
| Four classes / Dört sınıf | [PNG](charts/ulam-combined-1000000.png) |
| Punctual / Zamanında | [PNG](charts/ulam-punctual-1000000.png) |
| Frequency / Frekans | [PNG](charts/ulam-frequency-1000000.png) |
| Relative first / Göreli ilk görünüm | [PNG](charts/ulam-relative-1000000.png) |

**TR:** Her PNG'nin aynı isimli `.json` açıklaması aralığı, yönü, ölçeği, renkleri ve sınıf adetlerini kaydeder. Merkez 1, ilk sağa sonra yukarı; matematiksel y yukarı büyür. [25 sayılık SVG](charts/ulam-25.svg) etiketli bir örnektir. Bir hücre bir tam sayıdır. Dört sınıfta açık gri hiçbiri, mavi yalnız erkenci, turuncu yalnız asal, yeşil ikisi; koyu gri kapsam dışıdır. Frekans 0..9 ve göreli konum 0..1 sabit ölçekleri kullanır; zamanında hücreler açık gri kalır.

**EN:** Each PNG has a same-name `.json` description recording scope, direction, scale, colors, and class counts. Start with 1 at the center, step right then up; mathematical y grows upward. A [25-number SVG](charts/ulam-25.svg) provides labeled examples. Each cell represents an integer. The four-class legend is light gray for neither, blue for early only, orange for prime only, green for both; dark gray is outside scope. Frequency uses fixed 0..9 and relative position fixed 0..1 scales; punctual cells stay light gray.

**TR:** [Veri manifesti](data/manifest.json), [araç dökümü](tools.txt), [veri SHA-256](data-sha256.txt) ve [seçilmiş görsel SHA-256](figure-sha256.txt) üretim kaydını tamamlar. Benchmark JSON dosyaları ham tekrarları içerir; süreler yeniden üretimde değişebilir. Büyük kaynak CSV/JSON'un nasıl üretileceği Faz 3 raporunda açıklanır. Diğer spiral katmanlarının küçük SVG ve 4004×4004 ayrıntı PNG sürümleri yerel tam sette bulunur.

**EN:** The [data manifest](data/manifest.json), [tool inventory](tools.txt), [data SHA-256](data-sha256.txt), and [selected-image SHA-256](figure-sha256.txt) complete provenance. Benchmark JSON files retain raw repeats; times may change on regeneration. The Phase 3 report explains full summary CSV/JSON generation. Small SVG versions of the spiral layers and the 4004×4004 detailed PNG are available in the complete local set.
