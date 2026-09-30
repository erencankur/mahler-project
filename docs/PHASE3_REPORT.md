# Phase 3 dataset and measurements / Faz 3 veri seti ve ölçümler

## Result / Sonuç

**TR:** `1..1.000.000` aralığında **1.000.000** hedef özeti, **838.385** erkenci hedef ve **2.688.255** erken görünüm konumu üretildi. Konumlar baştaki `0.` hariç 1 tabanlıdır. `results/` yerel üretilmiş dosyaları içerir ve Git dışında tutulur; public yayın varlıkları Faz 6'da belirlenecektir.

**EN:** Over `1..1,000,000`, the run produced **1,000,000** target summaries, **838,385** early-bird targets, and **2,688,255** early occurrence positions. Positions are one-based after excluding the initial `0.`. Generated files live in the ignored `results/` directory; public release assets will be chosen in Phase 6.

| File / Dosya | Bytes / Bayt | Records / Kayıt | SHA-256 |
| --- | ---: | ---: | --- |
| `results/summary-1000000.csv` | 37,859,648 | 1,000,000 + header | `6f7bce4dd895c23dcd16ba06b55b9fb0ac7dc630cc1d25fa298ed696358c671d` |
| `results/summary-1000000.json` | 153,859,634 | 1,000,000 | `601d6951f12ca5494a68d9b15c9be5c820d1234934d148789f79b0c76c1ee139` |
| `results/occurrences-1000000.csv` | 80,344,430 | 2,688,255 + header | `ba34dfcecfbc8e559edbb81c1656624063651afe123d10c189e5b6ffad8a6891` |

**TR:** Özet CSV/JSON şeması `1` sürümüdür. Alanlar `number`, `digit_count`, `natural_position`, `first_position`, `is_early`, `early_frequency`, `advance_digits`. JSON'da büyük olabilecek tam sayılar ondalık metindir; `digit_count` ve `early_frequency` JSON sayısı, `is_early` mantıksaldır. CSV yerel ayraçsız ondalık ve `true`/`false` kullanır. Konum CSV'si `number`, `position`, `first_source`, `last_source`, `first_source_digit_offset` alanlarını taşır ve hedef/konuma göre sıralıdır.

**EN:** Summary CSV/JSON schema version is `1`. Fields are `number`, `digit_count`, `natural_position`, `first_position`, `is_early`, `early_frequency`, and `advance_digits`. Potentially large integers are decimal strings in JSON; `digit_count` and `early_frequency` are JSON numbers and `is_early` is boolean. CSV uses locale-free decimal values and `true`/`false`. The occurrence CSV has `number`, `position`, `first_source`, `last_source`, and `first_source_digit_offset`, ordered by target and position.

## Reproduction / Yeniden üretim

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/mahler scan --max 1000000 --format csv --output results/summary-1000000.csv --occurrences results/occurrences-1000000.csv
./build/mahler scan --max 1000000 --format json --output results/summary-1000000.json
python3 scripts/verify_exports.py --csv results/summary-1000000.csv --json results/summary-1000000.json --occurrences results/occurrences-1000000.csv --manifest results/summary-1000000.csv.manifest.json
shasum -a 256 results/summary-1000000.csv results/summary-1000000.json results/occurrences-1000000.csv
```

## Worked example / Çalışan örnek

**TR:** `9910` için özet kaydı şöyledir:

```csv
9910,4,38530,188,true,4,38342
```

Bu satır; sayının doğal olarak 38.530. konumda yazıldığını, fakat ilk kez 188. konumda başladığını söyler. Aradaki fark 38.342 rakamdır. Dört erken başlangıcın ayrı konum dosyasındaki karşılığı şunlardır:

```csv
9910,188,99,100,0
9910,2619,909,910,2
9910,2888,999,1000,1
9910,35289,9099,9100,3
```

İlk kayıt `99|100` sınırından başlar. `first_source_digit_offset=0`, eşleşmenin `99` sayısının ilk rakamında başladığını gösterir. Sonraki kayıtlar daha uzun kaynak sayılar ve basamak artışı sınırlarını örnekler. Bu konumlar, aynı hedefe ait farklı başlangıçlar oldukları için frekansta ayrı ayrı sayılır.

**EN:** The summary row for `9910` is:

```csv
9910,4,38530,188,true,4,38342
```

It says that `9910` occurs naturally at position 38,530, but first starts at position 188, an advance of 38,342 digits. Its four early starts appear separately in the occurrence export:

```csv
9910,188,99,100,0
9910,2619,909,910,2
9910,2888,999,1000,1
9910,35289,9099,9100,3
```

The first row begins at boundary `99|100`. `first_source_digit_offset=0` means that the match begins at the first digit of `99`. The later rows show longer sources and digit-width boundaries. Since the starts are distinct, all four contribute to the frequency.

**TR:** Ana uygulama ve veri üretimi yalnız C++20 gerektirir. `scripts/verify_exports.py`, yalnız isteğe bağlı çapraz biçim denetimi için Python standart kütüphanesini kullanır. `scan` varsayılan olarak CSV yazar; `--manifest` verilmezse manifest yolu `<output>.manifest.json` olur. Manifest, kapsam, sürüm, derleyici, derleme türü, OS/cihaz, motorlar, kayıt sayıları, süreler, bellek ve FNV-1a 64 dosya sağlama toplamlarını içerir. FNV sağlama toplamları kazara farklı dosyaları saptamak içindir; paylaşım için yukarıdaki SHA-256 özetleri de kaydedildi.

**EN:** The calculation and export application requires C++20 only. `scripts/verify_exports.py` uses the Python standard library solely for optional cross-format verification. `scan` defaults to CSV; without `--manifest`, the manifest path is `<output>.manifest.json`. The manifest records scope, version, compiler, build type, OS/device, engines, counts, timings, memory, and FNV-1a 64 file checksums. FNV checksums detect accidental differences; SHA-256 hashes above are also recorded for sharing.

**TR:** Doğrulayıcı bir milyon CSV ve JSON özetini alan alan karşılaştırdı. Erken konum dosyasındaki her satır için sayı metnini `S_1000000` içinde tekrar okudu; ilk/son kaynak sayıyı, ofseti, sıralamayı ve hedefin frekansını sınadı. CSV tam taraması tekrar çalıştırıldı; iki dosya bayt düzeyinde aynıydı. Test paketi küçük bir taramayı da iki kez karşılaştırır.

**EN:** The verifier compared all one million CSV and JSON summaries field by field. For every occurrence row it reread the target text in `S_1000000`, checking first/last source, offset, ordering, and per-target frequency. The full CSV run was repeated and the two files were byte-identical. The automated suite also repeats a smaller export.

## Core engine measurements / Çekirdek motor ölçümleri

**TR:** Apple Clang 21, Release, macOS Darwin 25.6.0, arm64 `Mac15,3`, 16 GiB RAM. Her motor ve ölçekte ayrı süreçte üç tekrar yapıldı; aşağıda ortanca süre ve sürecin tepe yerleşik belleği var. Süre yalnız hesaplamayı kapsar; sonuç sağlama toplamı ve dosya yazımı hariçtir. İki motorun matematiksel sonuç özetleri her ölçekte eşleşti. Yeniden kurma, alternatif motor olarak deneysel etiketlidir; ilk konumları ve frekansları tüm hedef aralığında doğrulandı. Bunlar bu cihazdaki örnek ölçümlerdir; başka makineler veya çalışma koşulları için hız vaadi değildir.

**EN:** Apple Clang 21, Release, macOS Darwin 25.6.0, arm64 `Mac15,3`, 16 GiB RAM. Each engine and scale ran three times in a separate process; the table gives median calculation time and process peak resident memory. Timing excludes result checksum and file output. Both engines produced matching mathematical-result checksums at every scale. Reconstruction is labeled experimental as an alternative engine; its first positions and frequencies were validated throughout the target range. These are samples on this device, not performance promises for other machines or conditions.

| Targets / Hedefler | Window median / Pencere ortanca | Window peak / Pencere bellek | Reconstruction median / Yeniden kurma ortanca | Reconstruction peak / Yeniden kurma bellek |
| ---: | ---: | ---: | ---: | ---: |
| 10,000 | 0.415 ms | 1.70 MB | 2.202 ms | 1.69 MB |
| 100,000 | 3.956 ms | 4.34 MB | 22.595 ms | 3.87 MB |
| 1,000,000 | 75.694 ms | 31.34 MB | 303.188 ms | 25.49 MB |

**TR:** Bir milyonluk CSV+konum çalışmasının manifestinde çekirdek tarama **58,966 ms**, özet yazımı **261,226 ms**, konum üretimi/yazımı **877,672 ms**, dosya sağlama toplamı okuması **136,471 ms**, tepe bellek **31,51 MB** olarak ölçüldü. Konum aşaması yeniden kurma motorunu ve 2.688.255 satırın yazımını birlikte içerir; tek başına motor kıyası için üstteki tablo kullanılmalıdır. JSON özet yazımı kendi çalışmasında **414,924 ms** idi. Temel pencere motoru bu ölçekte daha hızlı çıktı; yeniden kurma motoru konum açıklamaları için ayrı tutulur.

**EN:** The million-target CSV plus occurrences manifest records **58.966 ms** core scan, **261.226 ms** summary write, **877.672 ms** occurrence generation/write, **136.471 ms** checksum reading, and **31.51 MB** peak memory. The occurrence stage combines reconstruction with writing 2,688,255 rows; use the core table above for engine comparison. JSON summary writing took **414.924 ms** in its run. The window baseline was faster at this scale; reconstruction remains available for occurrence explanations.
