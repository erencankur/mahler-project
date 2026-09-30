# Phase 5: figures and Ulam geometry / Faz 5: grafikler ve Ulam geometrisi

Date / Tarih: 2026-09-30. Scope / Kapsam: every integer / her tam sayı `1..1000000`.

**TR:** Faz 5 tamamlandı. C++ hesaplamaları grafik tablolarını ve spiral görüntülerini üretir; Gnuplot yalnızca tabloları çizer. Python uygulamanın veya görsel üretiminin bağımlılığı değildir. İsteğe bağlı standart kütüphane doğrulayıcısı bir milyonluk CSV ile grafik tablolarını karşılaştırır. Arşiv dosyaları değiştirilmedi.

**EN:** Phase 5 is complete. C++ generates aggregate tables and spiral images; Gnuplot renders the tables. Neither the application nor figure generation depends on Python. An optional standard-library verifier compares plot tables with the million-target CSV. Archived files were not changed.

## Geometry and algorithm / Geometri ve algoritma

```text
17 16 15 14 13
18  5  4  3 12
19  6  1  2 11
20  7  8  9 10
21 22 23 24 25
```

**TR:** Merkez 1, ilk adım sağa, ikinci adım yukarıdır. Matematiksel koordinatlar `1=(0,0)`, `2=(1,0)`, `3=(1,1)`, `9=(1,-1)` olur. Görüntü satırı `side/2-y`, sütunu `side/2+x` hesaplanır. Önceki roadmap'teki 3×3 örnek, yön tanımıyla uyuşmadığından düzeltildi.

**EN:** Start at 1, step right, then up. Mathematical coordinates are `1=(0,0)`, `2=(1,0)`, `3=(1,1)`, and `9=(1,-1)`. Image row is `side/2-y`, column is `side/2+x`. The previous roadmap's 3×3 illustration contradicted its direction convention and has been corrected.

**TR:** `s`, `s²≥m` sağlayan en küçük tek tam sayı; halka yarıçapı `r=s/2`, halka sonu `s²` olur. `s²-m` farkının dört kenardan hangisinde kaldığı koordinatı belirler. Tam sayı karşılaştırmaları kare sınırlarında yuvarlama hatasını önler. Yan uzunluğu en fazla 1001 olduğundan mevcut uygulama bu küçük tek sayıları döngüyle bulur; daha büyük aralıkta tam karekök yaklaşımı değerlendirilebilir. Bağımsız test, `1,1,2,2,3,3,...` adım uzunluklarıyla spiral yürüyerek bir milyon koordinatı karşılaştırır.

**EN:** Let `s` be the smallest odd integer with `s²≥m`, `r=s/2`, and `s²` the ring endpoint. The edge containing offset `s²-m` determines the coordinates. Integer comparisons avoid rounding errors at squares. With side length capped at 1001, the implementation finds the small odd side in a loop; larger scopes could use an integer-square-root approach. An independent test walks segments of lengths `1,1,2,2,3,3,...` and compares all million coordinates.

## Layers and legend / Katmanlar ve renk açıklaması

| Layer / Katman | Meaning / Anlam | Scale / Ölçek |
|---|---|---|
| `early` | E(m)>0 | Blue / Mavi |
| `prime` | Prime / Asal | Orange / Turuncu |
| `intersection` | Prime and early / Asal ve erkenci | Green / Yeşil |
| `combined` | Four disjoint classes / Ayrık dört sınıf | Table below / Aşağıdaki tablo |
| `punctual` | E(m)=0, F(m)=P(m) | Blue / Mavi |
| `frequency` | Early occurrence count / Erken konum adedi | Fixed 0..9 / Sabit 0..9 |
| `relative` | F(m)/P(m), conditional on early / Yalnız erkencilerde F/P | Fixed 0..1 / Sabit 0..1 |

| Combined class / Birleşik sınıf | Count / Adet | Color / Renk |
|---|---:|---|
| Neither / Hiçbiri | 149,505 | `#E6E6E6` |
| Early only / Yalnız erkenci | 771,997 | `#0072B2` |
| Prime only / Yalnız asal | 12,110 | `#D55E00` |
| Both / İkisi birden | 66,388 | `#009E73` |
| Outside scope / Kapsam dışı | 2,001 | `#404040` |

**TR:** Bir milyonluk matris 1001×1001 hücredir. Kapsam dışı 2001 hücre dört sınıfın paydasına girmez. Ayrıntı görüntüsü hücre başına 4 piksel ile 4004×4004 olur; kenar yumuşatma ve boşluk yoktur. Küçük SVG en fazla 10.000 hedef içindir. `N≤400` ve hücre boyutu en az 8 olduğunda sayı etiketleri eklenir; her hücrede ayrıca SVG başlığı bulunur. Sürekli ölçekler Cividis'ten sabit beş renk çapasını doğrusal olarak birleştirir; tam 256 renkli özgün haritanın kopyası değildir. Zamanında görünenler sürekli katmanlarda açık gri tutulur.

**EN:** The million-target matrix uses 1001×1001 cells. Its 2001 out-of-range cells are excluded from class denominators. The detailed image uses four pixels per cell, producing 4004×4004 without smoothing or gaps. Small SVG supports up to 10,000 targets. Number labels appear when `N≤400` and cell size is at least 8; every SVG cell also has a title. Continuous scales interpolate five fixed Cividis color anchors rather than reproducing the full original 256-color map. Punctual targets remain light gray in continuous layers.

## Figure inventory and inputs / Grafikler ve girdiler

**TR:** 12 istatistik grafiği, iki dil ve üç biçimde **72 çıktı** üretildi. Buna yedi katmanın küçük SVG ve büyük PNG sürümleri, bir ayrıntı PNG ve etiketli küçük SVG eklenince **88 görsel** olur. Ham spiral dil bağımsızdır; iki dilde açıklamalar bu raporda bulunur. Küçük istatistik SVG'leri, büyük PNG önizlemeleri ve kaynak tabloları `figures/` içinde sürümlenir. Tam PNG/PDF/SVG seti `results/figures/` içinde Git dışında tutulur.

**EN:** Twelve statistical charts, two languages, and three formats produce **72 exports**. Seven layers in small SVG and large PNG, one detailed PNG, and one labeled small SVG bring the total to **88 figures**. Raw spirals are language independent; this report provides bilingual explanations. Compact statistical SVGs, large PNG previews, and source tables are versioned under `figures/`. The complete PNG/PDF/SVG set stays ignored under `results/figures/`.

| Figure / Grafik | Input / Girdi | Definition / Tanım |
|---|---|---|
| early-by-digits | groups.csv | Exact early/target counts; partial seven-digit group / Tam adetler; kısmi 7 basamak grubu |
| density | density.csv | Early and punctual proportions over 1..x / 1..x için iki oran |
| frequency | frequency.csv | E=0..9 proportions within widths 1..6 / 1..6 basamak gruplarında E payları |
| relative-ecdf | ecdf.csv | Exact ECDF evaluated on a 0.001 grid / 0.001 aralıklı eşiklerde tam ECDF |
| prime-comparison | groups.csv | Equal-width prime/composite denominators; 1 excluded / Eş basamaklı paydalar; 1 hariç |
| first-position-heatmap | heatmap.csv | 50×50 bins on log10(m)∈[0,6], log10(F)∈[0,7] |
| mechanisms | mechanisms.csv | Source width, boundaries, maximum trailing nines per occurrence / Konum başına üç ölçüt |
| block-deviation | blocks.csv | delta_q(T), q=1,2,3; observed endpoints / Gözlenen uçlar |
| digit-shares | digit-shares.csv | All ten digits including zero; observed endpoints / Sıfır dahil on rakam |
| ulam-rings | rings.csv | Proportions within square rings / Kare halkalarda oranlar |
| ulam-diagonals | diagonals.csv | Main diagonals versus other cells, matched by width / Basamağa göre ana köşegen ve diğer hücreler |
| performance | performance.csv | Median core time and process peak RSS / Medyan çekirdek süresi ve süreç tepe belleği |

**TR:** Isı haritasının sıfır sayımlı hücreleri beyazdır; renk ölçeği logaritmik ve sabit 1..1.000.000 aralığındadır. Logaritmik koordinatlar eksen etiketlerinde açıkça yazılır. ECDF'nin 1'deki zamanında kütlesi `161615/1000000=0.161615` olur; yalnız erkenci eğrisi farklı paydayı kullanır. Eğri 1001 eşikte değerlendirilir, aradaki bütün sıçramaları gösterme iddiası yoktur. Rakam/blok grafikleri yalnız ölçülen önekleri işaretler; aralarında matematiksel bir eğri varsayılmaz. Sonlu sayımlar için örneklem güven aralığı çizilmez.

**EN:** Zero-count heatmap cells are white; its logarithmic color scale is fixed at 1..1,000,000. Axis labels explicitly show logarithmic coordinates. The punctual mass at ECDF ratio 1 is `161615/1000000=0.161615`; the early-only curve uses a different denominator. The curve evaluates 1001 thresholds and does not display every intervening jump. Digit/block charts mark observed prefixes without assuming a mathematical curve between them. Complete enumerations do not receive sampling confidence intervals.

## Examples and interpretation / Örnekler ve yorum

**TR:** `12` bir asal değildir ama dizinin başında görünür: `F(12)=1`, `P(12)=14`; birleşik katmanda mavi hücre olur. `13` asaldır ama erkenci değildir ve turuncudur. `23` hem asal hem erkencidir ve yeşildir. `1` iki özelliği de taşımaz, açık gridir. `9910` için `F/P=188/38530≈0.004879` olduğundan göreli konum katmanında ölçeğin düşük ucundadır; frekansı 4 olduğu için frekans katmanında farklı renk alır. Katmanların aynı hücrede farklı soruları yanıtladığı bu örneklerle anlaşılır.

**EN:** `12` is composite but appears at the start: `F(12)=1`, `P(12)=14`, so its combined cell is blue. `13` is prime and punctual, hence orange. `23` is both prime and early, hence green. `1` has neither property and is light gray. For `9910`, `F/P=188/38530≈0.004879` is near the lower end of the relative scale, while frequency 4 receives a different frequency-layer color. These examples illustrate the distinct questions answered by the layers.

| Six-digit targets / Altı basamaklı hedefler | Early / Erkenci | Targets / Hedefler | Proportion / Oran |
|---|---:|---:|---:|
| Main diagonals / Ana köşegenler | 1,121 | 1,367 | 0.820043891734 |
| Other cells / Diğer hücreler | 756,634 | 898,633 | 0.841983323559 |

**TR:** Ana köşegen `|x|=|y|` olarak tanımlanır; bütün paralel köşegenlerin araştırması değildir. Altı basamak grubunda yaklaşık 2,19 yüzde puanlık fark gözlendi. Halkalar farklı sayı aralıklarına, dolayısıyla farklı basamak ve ondalık örüntülere denk gelir; son halka kısmidir. Şekil görünümü ve bu betimleyici sayımlar genel bağımsızlık, nedensellik veya sonsuz yoğunluk kanıtlamaz. Permütasyon testi ve çoklu karşılaştırma bu fazın kapsamına eklenmedi.

**EN:** Main diagonals mean `|x|=|y|`, not every parallel diagonal. The six-digit group shows a difference of about 2.19 percentage points. Rings cover different numeric ranges and therefore different decimal widths and patterns; the outermost ring is partial. Visual appearance and descriptive counts do not establish general independence, causality, or limiting density. Permutation tests and multiple-comparison procedures were not added to this phase.

## Reproduction / Yeniden üretim

**TR:** Çekirdek ve SVG/CSV için ek grafik bağımlılığı gerekmez. PNG için libpng; istatistik çizimleri için SVG, pngcairo ve pdfcairo terminalli Gnuplot 6.x; ölçüm JSON'unu ayıklayan betik için jq gerekir. macOS'ta bu araçlar paket yöneticisiyle kurulabilir. Aşağıdaki komutlar depo kökünden çalışır.

**EN:** Core calculations and SVG/CSV need no additional graphics library. PNG requires libpng; statistical rendering requires Gnuplot 6.x with svg, pngcairo, and pdfcairo terminals; the script extracts benchmark JSON with jq. These tools are available through macOS package managers. Run the following from the repository root.

```sh
brew install cmake libpng gnuplot jq
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DMAHLER_ENABLE_GRAPHICS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
bash scripts/render_figures.sh

# Individual examples / Tekil örnekler
./build/mahler ulam --max 25 --layer combined --cell-size 16 --output results/ulam-25.svg
./build/mahler ulam --max 1000000 --layer early --output results/ulam-early.png
./build/mahler plot-data --max 1000000 --output-dir results/plot-data
gnuplot -c scripts/figures.gp results/figures/data results/figures/charts tr svg

# Optional independent export audit / İsteğe bağlı çıktı karşılaştırması
python3 scripts/verify_plot_data.py results/figures/data results/summary-1000000.csv
```

**TR:** `render_figures.sh` bir milyon hedef için sabit üretim akışıdır; diğer aralıklar `ulam` ve `plot-data` ile hesaplanabilir. Grafik betiğindeki kapsam etiketleri ve karşılaştırma eksenleri bir milyonluk rapora aittir. Ölçümleri tekrar çalıştırmak süre/bellek değerlerini değiştirebilir. `GNUPLOT` ve `MAHLER` ortam değişkenleri araç yollarını değiştirebilir. Tam üretimin sonunda veri ve görsellerin SHA-256 dosyaları oluşturulur.

**EN:** `render_figures.sh` is a fixed million-target workflow; other ranges can be computed with `ulam` and `plot-data`. Plot-script scope labels and comparison axes refer to this million-target report. Rerunning benchmarks may change timing/memory values. `GNUPLOT` and `MAHLER` override executable paths. Full generation ends by writing SHA-256 inventories of data and images.

## Validation and environment / Doğrulama ve ortam

**TR:** Apple Clang 21, CMake 3.31.10, libpng 1.6.55 ve Gnuplot 6.0.3 ile üretildi. Gnuplot yerel ve Git dışındaki `.tools/` dizinine, mevcut Cairo/Pango ile, Qt gerektirmeyen biçimde derlendi. Font Arial; SVG 1200×800, PNG 2400×1600, PDF 12×8 inç. Araç ve veri dökümü [figures/README.md](../figures/README.md) içinde bağlantılıdır. Platformdaki font ve metadata farkları nedeniyle görsellerin bayt düzeyinde eşitliği beklenmez.

**EN:** Generated with Apple Clang 21, CMake 3.31.10, libpng 1.6.55, and Gnuplot 6.0.3. Gnuplot was built locally in ignored `.tools/`, using existing Cairo/Pango without Qt. Font is Arial; SVG 1200×800, PNG 2400×1600, PDF 12×8 inches. Tool/data inventories are linked from [figures/README.md](../figures/README.md). Font and metadata differences across platforms prevent a byte-identical image guarantee.

**TR:** Release, grafik bağımlılığı kapalı Release ve AddressSanitizer/UndefinedBehaviorSanitizer derlemelerinde altı test grubu geçti. Bir milyon koordinat bağımsız yürüyüşle karşılaştırıldı. Küçük PNG dosyası yeniden açılarak her hücrenin koordinatı, dört sınıf rengi ve kapsam dışı rengi bağımsız metin araması/asal bölme testiyle doğrulandı. Grafik tabloları bir milyonluk ayrı CSV çıktısıyla karşılaştırıldı; ECDF eşikleri ve bütün histogram/ısı hücreleri eşleşti. Görsellerde etiket ve yerleşim kontrolü yapıldı.

**EN:** Six test groups passed in Release, graphics-disabled Release, and AddressSanitizer/UndefinedBehaviorSanitizer builds. A million coordinates matched an independent walk. A small PNG was decoded and every cell's position, four-class color, and outside color checked against independent text search/trial division. Plot tables matched the separately exported million-target CSV, including ECDF thresholds and all histogram/heatmap bins. Figures were inspected for labels and layout.

**TR:** Bir milyon hedef için bu tekrarın çekirdek süre medyanı pencere motorunda 61,227 ms, yeniden kurmada 303,873 ms oldu; süreç tepe belleği sırasıyla 29,953125 ve 24,359375 MiB. Bunlar bu makineye ait üç tekrarlı ölçümlerdir; genel hız garantisi değildir. Tam grafik setinin boyutu yaklaşık 13 MiB'dir.

**EN:** This run's million-target median core times were 61.227 ms for windows and 303.873 ms for reconstruction; process peak RSS was 29.953125 and 24.359375 MiB, respectively. These are three-repeat measurements on this machine, not general performance guarantees. The complete figure set is approximately 13 MiB.

**Next / Sonraki:** Phase 6 — release documentation, clean-build verification, CI, license choice, and release assets / Faz 6 — yayın belgeleri, temiz derleme, CI, lisans seçimi ve sürüm dosyaları.
