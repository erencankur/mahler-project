# Mahler Project Roadmap / Mahler Projesi Yol Haritası

Date / Tarih: 2026-09-30  
Status / Durum: Phases 0–2 complete; Phase 3 next / Faz 0–2 tamamlandı; sıradaki çalışma Faz 3

## Phase overview / Faz özeti

**TR:** Proje yedi fazda ilerleyecek. Aşağıdaki Faz 0–6, ayrıntılardaki M0–M6 ile aynıdır. Literatür araştırması ve grafik araçlarının seçimi tamamlandı; Faz 0–2 uygulandı ve doğrulandı. Her fazın tamamlanması, aşağıdaki çıktılar ve Bölüm 6'daki kabul ölçütleriyle değerlendirilecek.

**EN:** The project proceeds through seven phases. Phases 0–6 below correspond to M0–M6 in the detailed plan. Literature research and graphics-tool selection are complete; Phases 0–2 are implemented and verified. Completion is determined by the deliverables below and the acceptance criteria in Section 6.

| Phase / Faz | Work / Çalışma | Deliverable / Çıktı |
| --- | --- | --- |
| 0 — Definitions / Tanımlar | Settle indexing, frequency, numeric limits, and data schema / İndeksleme, frekans, sayısal sınırlar ve veri şemasını kesinleştir | Bilingual mathematical and API specification / İki dilli matematik ve API sözleşmesi |
| 1 — Working foundation / Çalışan temel | Create C++20/CMake project, CLI, digit lookup, and natural-position calculation / C++20/CMake projesi, CLI, rakam sorgusu ve doğal konum hesabını kur | Buildable macOS application with verified basic queries / Temel sorguları doğrulanmış, macOS'ta derlenen uygulama |
| 2 — Early-bird algorithms / Erkenci algoritmaları | Implement window and reconstruction engines; compare with independent searches and legacy data / Pencere ve yeniden kurma motorlarını geliştir; bağımsız arama ve eski verilerle karşılaştır | Verified first positions, early frequencies, and comparison report / Doğrulanmış ilk konumlar, erken frekanslar ve karşılaştırma raporu |
| 3 — One-million dataset / Bir milyon veri seti | Scan all targets; export CSV/JSON; compare runtime and memory / Bütün hedefleri tara; CSV/JSON üret; süre ve belleği karşılaştır | Complete reproducible dataset and benchmarks / Eksiksiz, yeniden üretilebilir veri seti ve ölçümler |
| 4 — Mathematical investigation / Matematiksel inceleme | Study frequencies, primes, special subsets, rotations, and carries / Frekans, asal, özel kümeler, döndürme ve elde etkilerini incele | Analysis tables and clearly labeled findings/hypotheses / Analiz tabloları ve açıkça ayrılmış bulgular/hipotezler |
| 5 — Figures / Grafikler | Render statistical charts and Ulam layers with Gnuplot and C++/libpng / Gnuplot ve C++/libpng ile istatistik grafikleri ve Ulam katmanları üret | Turkish/English SVG, PDF, and PNG figures / Türkçe/İngilizce SVG, PDF ve PNG grafikler |
| 6 — Publication preparation / Yayın hazırlığı | Finish READMEs, references, license, clean-build checks, and release assets / README, kaynaklar, lisans, temiz derleme kontrolleri ve sürüm dosyalarını tamamla | Public-release-ready v0.1.0 / Public yayına hazır v0.1.0 |

**TR:** Sıra `0 → 1 → 2 → 3 → 4 → 5 → 6` olacaktır. İki dilli belgeler ve doğrulama her fazda güncellenir; Faz 6'da son düzenlemeleri yapılır. Faz 2'de yeniden kurma yönteminin tüm konumları eksiksiz verdiği henüz gösterilemiyorsa doğrulanmış pencere motoruyla ilerlenir; ikinci yöntem deneysel olarak işaretlenir. İlk sürümün tamamlanması bu araştırma yönteminin başarılı olmasına bağlı değildir.

**EN:** The order is `0 → 1 → 2 → 3 → 4 → 5 → 6`. Bilingual documentation and validation are maintained throughout and finalized in Phase 6. If complete occurrence reconstruction has not been established in Phase 2, proceed with the verified window engine and label reconstruction experimental. First-release completion does not depend on success of that research method.

### Implementation approval and current scope / Uygulama onayı ve mevcut kapsam

**TR:** Kullanıcı 2026-09-30 tarihinde “Başla” diyerek uygulamayı başlattı ve ardından “Devam et” diyerek Faz 2'ye geçilmesini istedi. Faz 0–2 tamamlandı; sonraki çalışma Faz 3'te yeniden üretilebilir toplu veri seti ve ölçümlerdir. Bütün proje değişiklikleri `mahler-project/` içinde yapılır; GitHub yayını ayrıca kullanıcıyla kesinleştirilecek.

**EN:** The user started implementation with “Başla” on 2026-09-30 and then requested Phase 2 with “Devam et.” Phases 0–2 are complete; Phase 3 adds a reproducible batch dataset and measurements. All project changes stay in `mahler-project/`; GitHub publication will be confirmed separately.

## 1. Purpose / Amaç

**TR:** Sayma sayılarının ondalık gösterimlerinin birleştirilmesiyle oluşan Mahler sayısı, diğer adıyla Champernowne sabiti, üzerine yeniden üretilebilir bir matematik ve yazılım araştırması geliştirmek. İlk sürüm iki problemi çözecek: dizinin `n`'inci rakamını bulmak ve `1 ≤ m ≤ 1.000.000` aralığındaki sayıların erkenciliğini, ilk görünümünü ve erken görünüm frekansını hesaplamak. Sonuçlar frekans, asal sayı ve Ulam spirali analizleriyle incelenecek.

**EN:** Develop a reproducible mathematical and software investigation of Mahler's number, also known as the Champernowne constant, formed by concatenating the decimal representations of positive integers. The first release will solve two problems: finding the digit at position `n`, and determining early-bird status, first occurrence, and early-occurrence frequency for every integer `1 ≤ m ≤ 1,000,000`. Frequency, prime-number, and Ulam spiral analyses will examine the results.

## 2. Agreed scope / Kesinleşen kapsam

| Decision / Karar | Scope / Kapsam |
| --- | --- |
| Implementation / Uygulama | C++20 |
| Initial platform / İlk platform | macOS |
| Interface / Arayüz | Command-line application / Komut satırı uygulaması |
| Research range / Araştırma aralığı | All integers from 1 through 1,000,000, inclusive / 1–1.000.000 arasındaki bütün sayılar, uçlar dahil |
| Numeral base / Sayı tabanı | Base 10 / Onluk taban |
| Data outputs / Veri çıktıları | CSV and JSON / CSV ve JSON |
| Documentation / Belgeler | Turkish and English / Türkçe ve İngilizce |
| Graphics / Grafikler | C++ aggregates + Gnuplot charts; C++ SVG/libpng Ulam renderer / C++ özetleri + Gnuplot grafikler; C++ SVG/libpng Ulam çizimi |
| Publication / Yayın | Public GitHub repository, after release preparation / Sürüm hazırlığından sonra public GitHub deposu |

**TR:** Bütün proje değişiklikleri `mahler-project/` içinde yapılacak. Kardeş dizin `../legacy-math-folders/` salt okunur kaynak arşividir; içindeki dosyalar değiştirilmeyecek, yeniden adlandırılmayacak veya taşınmayacak. İlk sürümün çalışması için Python gerekmeyecek. Diğer platformlar, başka tabanlar, başka sayı dizileri ve masaüstü arayüzü sonraki sürümlere bırakılacak.

**EN:** All project changes will stay inside `mahler-project/`. The sibling directory `../legacy-math-folders/` is a read-only source archive: its files will not be modified, renamed, or moved. Python will not be required to run the first release. Other platforms, numeral bases, source sequences, and a desktop interface are deferred to later releases.

## 3. Mathematical specification / Matematiksel tanımlar

### 3.1 Sequence and indexing / Dizi ve indeksleme

```text
S = 123456789101112131415...
C_10 = 0.123456789101112131415...
```

**TR:** `S` üzerinde çalışılır. Konumlar 1'den başlar: `d_1 = 1`, `d_9 = 9`, `d_10 = 1`, `d_11 = 0`. Sabitin başındaki `0.` veya `0,` dizinin parçası değildir. Pozitif tam sayılar standart ondalık gösterimleriyle, başta sıfır olmadan yazılır. `n = 0` ve `m = 0` temel sorgularda geçersizdir; sıfır rakamı dizide geçerli bir rakamdır.

**EN:** Computation uses `S`. Positions start at 1: `d_1 = 1`, `d_9 = 9`, `d_10 = 1`, and `d_11 = 0`. The initial `0.` or `0,` is excluded. Positive integers use their standard decimal representations without leading zeros. Core queries reject `n = 0` and `m = 0`; the digit zero remains a valid sequence digit.

### 3.2 Natural position / Doğal konum

Let / Tanım:

```text
k = number of decimal digits of m / m'nin ondalık basamak sayısı
B(0) = 0
B(k) = sum(j = 1..k, 9 × 10^(j-1) × j)
P(m) = 1 + B(k-1) + k × (m - 10^(k-1))
```

**TR:** `P(m)`, `m` sayısının kendi sırası geldiğinde yazıldığı başlangıç konumudur. Örneğin `P(1)=1`, `P(10)=10`, `P(100)=190`, `P(1000)=2890`.

**EN:** `P(m)` is the starting position where `m` is written in its natural enumeration order. Examples: `P(1)=1`, `P(10)=10`, `P(100)=190`, and `P(1000)=2890`.

### 3.3 Early birds and frequency / Erkencilik ve frekans

```text
F(m) = earliest starting position of the decimal representation of m in S
early(m) = F(m) < P(m)
E(m) = count of matching starting positions p with p < P(m)
A(m) = P(m) - F(m)
```

**TR:** `E(m)` erken görünüm frekansı, `A(m)` erkencilik mesafesidir. Erkenci olmayan sayılarda `F(m)=P(m)`, `E(m)=0`, `A(m)=0` olur. Farklı başlangıç konumlarındaki örtüşen eşleşmeler ayrı sayılır; aynı konum iki kez sayılmaz. Doğal görünüm ve ondan sonraki görünümler `E(m)` hesabına katılmaz. Eşleşme, bir kaynak sayının içinde veya birden fazla kaynak sayı sınırını aşarak oluşabilir. Temel karşılaştırma eşleşmenin başlangıç konumudur.

**EN:** `E(m)` is the early-occurrence frequency, and `A(m)` is the advance distance. Non-early birds have `F(m)=P(m)`, `E(m)=0`, and `A(m)=0`. Overlapping matches at distinct starting positions count separately; a position is never counted twice. The natural occurrence and subsequent occurrences are excluded from `E(m)`. Matches may lie inside a source integer or cross several source-integer boundaries. Classification compares match starting positions.

**TR:** Sonlu bir önekteki toplam görünüm sayısı ayrı bir ölçüttür; erken görünüm frekansıyla karıştırılmayacak. Pozitif hedef sayının basamak uzunluğundan daha uzun, sıfırla başlayan pencereler o sayının eşleşmesi sayılmayacak.

**EN:** Total occurrences within a finite prefix are a separate metric and must not be confused with early-occurrence frequency. Longer windows with leading zeros do not represent matches for a shorter positive target integer.

### 3.4 Direct digit lookup / Doğrudan basamak sorgusu

```text
Find k such that B(k-1) < n ≤ B(k).
r = n - B(k-1) - 1
source_integer = 10^(k-1) + floor(r / k)
digit_offset = r mod k                  # zero-based within source_integer
```

**TR:** İlgili rakam kaynak sayıdan çıkarılır; tüm önek üretilmez. Basamak gruplarıyla arama `O(log n)` işlem gerektirir. Hesaplar tam sayılarla yapılacak; kayan noktalı logaritma veya üs hesapları indeks belirlemeyecek. `uint64_t` girişleri için ara çarpım ve toplam taşmaları denetlenecek; API'nin desteklediği sınırlar belgelenip sınanacak.

**EN:** Extract the requested digit from the source integer without generating the full prefix. Searching digit groups takes `O(log n)` operations. Indexing will use integer arithmetic rather than floating-point logarithms or powers. Intermediate additions and multiplications will be checked for overflow with `uint64_t` inputs; supported API limits will be documented and tested.

## 4. Algorithms and resource plan / Algoritmalar ve kaynak planı

**TR:** `N=1.000.000` için `S_N = concat(1,...,N)` uzunluğu **5.888.896 rakamdır**. Bu, dizinin ilk bir milyon rakamıyla aynı kapsam değildir. `S_N`, her hedefin doğal gösterimini içerdiği için bu aralıktaki ilk görünüm ve erken görünüm hesapları için yeterlidir; daha sonraki sonsuz diziye ilişkin toplam frekans iddiası yapılmaz.

**EN:** For `N=1,000,000`, `S_N = concat(1,...,N)` contains **5,888,896 digits**. This differs from studying the first million sequence digits. Since `S_N` includes every target's natural representation, it suffices for first-occurrence and early-occurrence calculations in this range; no claim is made about total frequency in the subsequent infinite sequence.

| Component / Bileşen | Approach / Yaklaşım |
| --- | --- |
| Reference implementation / Referans uygulama | Direct overlapping substring search on small datasets / Küçük verilerde örtüşmeleri sayan doğrudan metin araması |
| Production scan / Ana tarama | Numeric rolling windows for lengths 1 through K / 1–K uzunluklarında sayısal kayan pencereler |
| Window filtering / Pencere süzme | Reject leading zeros, values outside 1..N, and starts at or after P(m) / Baştaki sıfırları, aralık dışı değerleri ve P(m) veya sonrasındaki başlangıçları eleme |
| Position lookup / Konum hesabı | Precomputed digit-group totals and P(m) formula / Önceden hesaplanan basamak grubu toplamları ve P(m) formülü |
| Prime classification / Asallık | Sieve of Eratosthenes / Eratosthenes eleği |
| Full occurrence export / Bütün konumların çıktısı | Optional separate output; avoid keeping all matches in memory / İsteğe bağlı ayrı çıktı; tüm eşleşmeleri bellekte tutmama |

**TR:** `L=|S_N|`, `K=digits(N)` olmak üzere ana tarama `O(LK)` ve özet tablolar `O(N)` alan hedefler; önek bellekte tutulursa toplam alan `O(L+N)` olur. Bir milyon için yaklaşık 41 milyon pencere değerlendirilir. Bu işlem sayısı tahminidir; süre ve tepe bellek kullanımı ölçülmeden performans vaadi verilmeyecek. Her sayıyı büyük metinde ayrı ayrı aramak yalnızca küçük verilerde doğrulama yöntemi olarak kullanılacak.

**EN:** With `L=|S_N|` and `K=digits(N)`, the production scan targets `O(LK)` work and `O(N)` summary storage; retaining the prefix gives `O(L+N)` total storage. One million targets require approximately 41 million window evaluations. This is an operation estimate, not a runtime promise: elapsed time and peak memory will be measured. Independent per-target text searches will serve only as a small-data validation method.

**TR:** Araştırma sonrası karar: bu tarama güvenilir temel yöntem olarak korunacak. OEIS'teki basamaklardan kaynak sayı yeniden kurma yöntemi ikinci motor olarak geliştirilecek; tamlık doğrulaması, konum/frekans eşitliği ve ölçümler tamamlandıktan sonra varsayılan motor seçilecek. Ayrıntılar Bölüm 12'de.

**EN:** Post-review decision: retain this scan as the dependable baseline. Develop digit-based source reconstruction from the OEIS implementation as a second engine; choose the default after completeness validation, occurrence/frequency agreement, and benchmarks. See Section 12.

## 5. Proposed repository structure / Önerilen depo yapısı

```text
mahler-project/
├── CMakeLists.txt
├── ROADMAP.md
├── README.md                    # English
├── README.tr.md                 # Türkçe
├── LICENSE                      # To be selected / Seçilecek
├── .gitignore
├── .github/workflows/           # macOS build and tests / Derleme ve testler
├── include/mahler/              # Public calculation API / Hesaplama API'si
├── src/                        # Mathematical core / Matematiksel çekirdek
├── app/                        # CLI
├── tests/                      # Independent and boundary checks / Doğrulama
├── benchmarks/                 # Reproducible measurement runs / Ölçümler
├── docs/en/                    # Definitions, algorithms, analyses
├── docs/tr/                    # Tanımlar, algoritmalar, analizler
├── data/reference/             # Small, documented fixtures / Küçük referans veriler
├── results/                    # Selected results and run manifests / Sonuçlar
└── figures/                    # Selected generated graphics / Grafikler
```

**TR:** Bu yapı planlanan yapıdır; tüm klasörler hemen oluşturulmayacak. Derleme ürünleri, büyük geçici çıktılar ve yerel araç dosyaları Git'e eklenmeyecek. Büyük veri çıktılarının sürüm dosyalarında mı yoksa GitHub Releases içinde mi yayımlanacağı ölçümden sonra kararlaştırılacak. Eski arşivin bir kopyası otomatik olarak depoya eklenmeyecek.

**EN:** This is the intended structure, not a requirement to create every directory immediately. Build products, large temporary outputs, and local tool files will be excluded from Git. Whether large datasets belong in the repository or GitHub Releases will be decided after measuring their size. The legacy archive will not be automatically copied into the repository.

## 6. Milestones and acceptance criteria / Aşamalar ve kabul ölçütleri

### M0 — Specification / Matematiksel sözleşme

- [x] **TR:** İndeksleme, erkencilik, frekans ve kaynak sayı sınırlarını örneklerle belgelemek. **EN:** Document indexing, early-bird status, frequency, and source boundaries with examples.
- [x] **TR:** CSV/JSON alanlarını, geçersiz girişleri ve sayısal sınırları belirlemek. **EN:** Define CSV/JSON fields, invalid inputs, and numerical limits.
- [x] **TR:** Eski bulgular ile yeni doğrulama sonuçlarını ayırmak. **EN:** Distinguish legacy findings from newly validated results.
- [x] **TR:** Kaynak sürümleri/indeksleme ve normallik/blok sapması ayrımını belgelemek. **EN:** Document source versions/indexing and the normality/block-deviation distinction.

**Acceptance / Kabul:** TR — Temel işlemlerin girdisi, çıktısı ve anlamı belirsiz olmamalı. EN — Core operations have unambiguous inputs, outputs, and semantics.

### M1 — C++ core and CLI / C++ çekirdeği ve komut satırı

- [x] **TR:** macOS üzerinde CMake ve Apple Clang ile derlenebilir yapı kurmak. **EN:** Establish a CMake build using Apple Clang on macOS.
- [x] **TR:** `digit_at`, `natural_position` ve önek üretimini uygulamak. **EN:** Implement `digit_at`, `natural_position`, and prefix generation.
- [x] **TR:** `digit` komutunu ve doğal konumu gösteren ilk `inspect` sürümünü geliştirmek; ilk görünüm ve frekans alanlarını Faz 2'de eklemek. **EN:** Implement `digit` and an initial `inspect` showing natural position; add first occurrence and frequency in Phase 2.
- [x] **TR:** Hatalı girişler, çıkış kodları ve hata mesajlarını düzenlemek. **EN:** Define invalid-input handling, exit codes, and error messages.

**Acceptance / Kabul:** TR — Rakam sorguları bağımsız üretilen önekle uyuşmalı; 9/10, 189/190 ve 2889/2890 konum sınırları, geçersiz girişler ve taşma sınırları doğrulanmalı. EN — Digit queries match an independently generated prefix; position boundaries 9/10, 189/190, and 2889/2890, invalid inputs, and overflow limits are verified.

### M2 — Early-bird engine and legacy comparison / Erkenci sayı motoru ve eski verilerin karşılaştırılması

- [x] **TR:** İlk görünüm, erken frekans ve kayan pencere taramasını uygulamak. **EN:** Implement first occurrence, early frequency, and the rolling-window scan.
- [x] **TR:** Küçük aralıklarda bağımsız metin aramasıyla sayı ve konum bazında karşılaştırmak. **EN:** Compare target values and positions against independent substring searches on small ranges.
- [x] **TR:** 1–9.999 aralığındaki eski listelerle karşılaştırma raporu oluşturmak. **EN:** Produce a comparison report for the legacy lists covering 1–9,999.
- [x] **TR:** Eksik kayıt, yinelenen konum ve yanlış frekans nedenlerini örneklerle açıklamak. **EN:** Explain missing entries, duplicated positions, and incorrect frequencies with examples.
- [x] **TR:** R11 tabanlı yeniden kurma motorunun tamlığını, elde durumlarını ve tüm konumların tekilleştirilmesini sınamak; doğrulanmayan sürümü deneysel tutmak. **EN:** Investigate completeness, carry handling, and position deduplication for the R11-based engine; keep unverified versions experimental.
- [x] **TR:** `991`, `919`, `9193`, `9199`, `11121`, `9090`, `900900` örneklerini bağımsız aramayla doğrulamak. **EN:** Validate these rotation/carry and negative-classification examples with independent searches.

**Acceptance / Kabul:** TR — 1, 2, 3 ve 4 basamaklı erkenci adetleri sırasıyla `0, 45, 630, 6896` olmalı; bağımsız referans ile frekanslar ve konumlar uyuşmalı. EN — Early-bird counts for 1, 2, 3, and 4 digits are `0, 45, 630, 6896`; frequencies and positions agree with the independent reference.

### M3 — One-million scan and exports / Bir milyon sayı taraması ve çıktılar

- [ ] **TR:** `scan --max 1000000` ile bütün hedefler için özet kayıt üretmek. **EN:** Generate summaries for every target using `scan --max 1000000`.
- [ ] **TR:** CSV ve JSON çıktıları, isteğe bağlı konum çıktısı ve çalışma manifesti oluşturmak. **EN:** Provide CSV and JSON exports, optional occurrence exports, and a run manifest.
- [ ] **TR:** 10.000, 100.000 ve 1.000.000 ölçeklerinde süre ve bellek ölçmek. **EN:** Measure runtime and memory at scales of 10,000, 100,000, and 1,000,000.
- [ ] **TR:** Aynı girdilerle tekrarlanan çalışmalarda matematiksel çıktıların aynı olduğunu doğrulamak. **EN:** Verify identical mathematical outputs across repeated runs with the same inputs.
- [ ] **TR:** Basamak adetlerini `0,45,630,6896,73059,757755`, toplam erkenci adedini `838385` dış hedefleriyle karşılaştırmak. **EN:** Compare digit-group counts and the total with the external targets.
- [ ] **TR:** Doğrulanmış motorları çıktı maliyetini çekirdek süresinden ayırarak karşılaştırmak; deneysel motor sonuçlarını ayrıca etiketlemek. **EN:** Benchmark validated engines separately from output cost; label experimental-engine results explicitly.

**Acceptance / Kabul:** TR — Eksiksiz bir milyon özet kayıt, doğrulanmış şema, ölçüm raporu ve yeniden üretim komutları bulunmalı. EN — Deliver one million complete summary records, a verified schema, a measurement report, and reproduction commands.

### M4 — Mathematical analyses / Matematiksel analizler

- [ ] **TR:** Basamak grubuna göre erkenci adedi, oranı ve frekans dağılımı. **EN:** Counts, proportions, and frequency distributions by digit length.
- [ ] **TR:** İlk görünüm konumları, erkencilik mesafeleri ve uç örnekler. **EN:** First-occurrence positions, advance distances, and extreme examples.
- [ ] **TR:** Aynı basamak gruplarında asal ve bileşik sayıların karşılaştırılması. **EN:** Compare prime and composite targets within matching digit-length groups.
- [ ] **TR:** Palindrom, lasa ve Fibonacci alt kümelerinin incelenmesi. **EN:** Examine palindrome, emirp, and Fibonacci subsets.
- [ ] **TR:** Rapordaki basamak kurallarını önerme veya hipotez olarak ele alıp doğrulamak. **EN:** Validate legacy digit rules as propositions or hypotheses.
- [ ] **TR:** İsteğe bağlı rakam ve kısa blok dağılımını ayrı ölçütlerle incelemek. **EN:** Optionally study digit and short-block distributions as separate metrics.
- [ ] **TR:** Kaynak uzunluğu, geçilen sınır sayısı, elde zinciri ve döndürme tanıkları için özetler üretmek. **EN:** Aggregate source widths, crossed-boundary counts, carry chains, and rotation certificates.

**Acceptance / Kabul:** TR — Her tabloda aralık, payda ve tanım açık olmalı; sonlu deney sonuçları genel matematiksel kanıt gibi sunulmamalı. EN — Each table states its range, denominator, and definitions; finite experiments are not presented as general mathematical proofs.

### M5 — Ulam spiral and graphics / Ulam spirali ve grafikler

- [ ] **TR:** Merkezde 1, sağa ilk adım, ardından saat yönünün tersine kare spiral sözleşmesini belgelemek. **EN:** Document a square spiral with 1 at the origin, the first step to the right, then counterclockwise growth.
- [ ] **TR:** Erkenci, asal ve hem asal hem erkenci sayı görünümleri üretmek. **EN:** Render early birds, primes, and their intersection.
- [ ] **TR:** SVG çıktıları, renk açıklamaları, aralıklar ve kaynak veri bağlantıları eklemek. **EN:** Provide SVG exports, legends, ranges, and source-data links.
- [ ] **TR:** Büyük spiral için libpng ile PNG, küçük spiral için SVG üretmek; Gnuplot ile istatistik grafiklerini çizmek. **EN:** Use libpng PNG for large spirals, SVG for small spirals, and Gnuplot for statistical charts.
- [ ] **TR:** Görsel çizgileri basamak grupları, halkalar ve gerekiyorsa köşegen ölçümleriyle değerlendirmek. **EN:** Investigate visual structures with digit groups, rings, and diagonal measurements where useful.

**Acceptance / Kabul:** TR — Spiral koordinatları küçük örneklerle doğrulanmalı; bütün görseller yeniden üretilebilmeli ve kullanılan ölçütleri belirtmeli. EN — Verify spiral coordinates on small examples; all figures are reproducible and identify their metrics.

### M6 — Documentation and public release / Belgeler ve public yayın

- [ ] **TR:** İngilizce `README.md` ve Türkçe `README.tr.md` hazırlamak; aralarında bağlantı vermek. **EN:** Prepare English `README.md` and Turkish `README.tr.md` with reciprocal links.
- [ ] **TR:** Tanımlar, algoritmalar, doğrulama, bulgular ve sınırlamalar için iki dilde belgeler yazmak. **EN:** Write bilingual documentation for definitions, algorithms, validation, findings, and limitations.
- [ ] **TR:** Temiz macOS kurulumundan derleme, test ve örnek analiz akışını doğrulamak. **EN:** Verify build, tests, and an example analysis from a clean macOS setup.
- [ ] **TR:** macOS derleme ve test iş akışı eklemek. **EN:** Add a macOS build-and-test workflow.
- [ ] **TR:** Kaynaklar, lisans, katkı yönergeleri ve sürüm notlarını tamamlamak. **EN:** Complete references, licensing, contribution guidance, and release notes.
- [ ] **TR:** Public yayın öncesinde depoya girecek dosyaları kontrol etmek ve yayın adımını kullanıcıyla kesinleştirmek. **EN:** Review release contents and confirm the publication step with the user.

**Acceptance / Kabul:** TR — `v0.1.0` başka bir kullanıcı tarafından belgelenmiş komutlarla derlenebilmeli, çalıştırılabilmeli ve seçilmiş sonuçları yeniden üretebilmeli. EN — Another user can build and run `v0.1.0` and reproduce selected results using the documented commands.

## 7. Data and CLI contract / Veri ve komut sözleşmesi

### Proposed commands / Önerilen komutlar

```sh
mahler digit 2020
mahler inspect 9910
mahler early 9910 --format json
mahler scan --max 1000000 --format csv --output results/summary.csv
mahler scan --max 1000000 --format json --output results/summary.json
mahler summarize results/summary.csv
mahler ulam --max 10000 --layer early --output figures/ulam-early.svg
```

**TR:** `digit`, `inspect` ve `early` çalışıyor. `scan`, `summarize` ve `ulam` planlanan komutlardır. Konum dışa aktarımı ve grafik komutlarının kesin seçenekleri ilgili aşamada belirlenecek. Komut isimleri ve makineye yönelik alan adları İngilizce olacak; kullanım belgeleri iki dilde sunulacak.

**EN:** `digit`, `inspect`, and `early` are implemented. `scan`, `summarize`, and `ulam` remain proposed commands. Exact occurrence-export and graphics options will be settled in their milestones. Command names and machine-readable field names will be English; usage documentation will be bilingual.

### Summary fields / Özet alanları

```text
number
digit_count
natural_position
first_position
is_early
early_frequency
advance_digits
is_prime
is_palindrome
is_emirp
is_fibonacci
```

**TR:** Analiz özellikleri uygulandıkça şemaya eklenecek; hesaplanmayan bir özellik yanlışlıkla `false` olarak yazılmayacak. Şema sürümlenecek. Konum çıktısı ayrı kayıtlar içerecek: hedef sayı, başlangıç konumu, ilk ve son kaynak sayı, ilk kaynak sayı içindeki sıfır tabanlı rakam ofseti. JSON büyük tam sayıların kayıpsız tüketimi için belgelenmiş bir gösterim kullanacak.

**EN:** Analysis fields will be added as they are implemented; an uncomputed property must not be silently exported as `false`. The schema will be versioned. Occurrence exports will contain separate records for target integer, starting position, first and last source integers, and zero-based digit offset within the first source integer. JSON will use a documented representation for lossless consumption of large integers.

**TR:** Çalışma manifesti; kapsamı, tabanı, indeksleme sözleşmesini, uygulama sürümü veya commit'ini, derleyiciyi, derleme seçeneklerini, cihaz/işletim sistemi bilgisini, süreyi, ölçülmüş bellek kullanımını ve çıktı dosyalarının özetlerini içerecek. Süre gibi değişken metadata matematiksel çıktıların eşitlik kontrolünden ayrı tutulacak.

**EN:** A run manifest will record range, base, indexing convention, application version or commit, compiler, build options, device/OS information, elapsed time, measured memory usage, and output checksums. Variable metadata such as timing will be excluded from comparisons of mathematical outputs.

## 8. Validation priorities / Doğrulama öncelikleri

**TR:** Testler aynı algoritmanın yeniden yazımından ibaret olmayacak. Küçük verilerde basit bağımsız arama, sınır örnekleri ve dış referanslar birlikte kullanılacak. Büyük tarama için yapısal kontroller ve seçilmiş doğrudan sorgular uygulanacak.

**EN:** Tests will not merely repeat the implementation. Small-data validation will combine simple independent searches, boundary examples, and external references. Large scans will use structural checks and selected direct queries.

- [ ] `d_7=7`, `d_24=1`, `d_2020=7`, `d_3719=2`.
- [ ] **TR:** 1–9 erkenci değildir; 10'un kuvvetleri erkenci değildir. **EN:** 1–9 and powers of 10 are not early birds.
- [ ] **TR:** 666: ilk konum 122, doğal konum 1888, erken frekans 1. **EN:** 666: first position 122, natural position 1888, early frequency 1.
- [ ] **TR:** 7891: erken konumlar 7 ve 6047, frekans 2. **EN:** 7891: early positions 7 and 6047, frequency 2.
- [ ] **TR:** 9910: erken konumlar 188, 2619, 2888 ve 35289, frekans 4. **EN:** 9910: early positions 188, 2619, 2888, and 35289, frequency 4.
- [ ] **TR:** 891: `8|9|10` ve `189|190`, frekans 2. **EN:** 891: `8|9|10` and `189|190`, frequency 2.
- [ ] **TR:** 1234: aynı konumu çift saymadan frekans 1. **EN:** 1234: frequency 1 without duplicate-position counting.
- [ ] **TR:** 1323: `31|32|33`; 2939: `92|93|94`. **EN:** 1323: `31|32|33`; 2939: `92|93|94`.
- [ ] **TR:** Asallıkta 1, 2, kare sayılar ve bileşikler; özel kümelerde bilinen olumlu ve olumsuz örnekler. **EN:** Prime checks cover 1, 2, squares, and composites; special subsets include known positive and negative examples.
- [ ] **TR:** Sayısal taşma, sıfırla başlayan pencereler, aralık sonu ve CSV/JSON eşdeğerliği. **EN:** Check overflow, leading-zero windows, range endpoints, and CSV/JSON equivalence.

**TR:** İlk incelemede genel PDF listesinde 28 eksik dört basamaklı sayı ve mevcut kayıtlarda 62 frekans farkı; asal PDF listesinde 1 eksik sayı ve 6 frekans farkı gözlendi. Bu değerler başlangıç inceleme bulgularıdır; C++ motoru ve bağımsız referansla yeniden doğrulanıp karşılaştırma raporunda gerekçelendirilecek. Eski dosyalar düzeltilmeyecek.

**EN:** Initial inspection found 28 missing four-digit targets and 62 frequency discrepancies in existing entries of the general PDF list, plus one missing target and six frequency discrepancies in the prime PDF list. These are initial review findings to be revalidated by the C++ engine and independent reference and explained in the comparison report. Legacy files will not be corrected.

## 9. Research and publication principles / Araştırma ve yayın ilkeleri

**TR:** Gözlem, hesaplamayla doğrulanmış sonuç, hipotez ve matematiksel kanıt açıkça ayrılacak. İki dilde belgeler aynı tanım ve sonuçları kullanacak. Görsel bir örüntü tek başına kanıt sayılmayacak. Projenin katkısı; eski çalışmanın doğrulanması, açık C++ uygulaması, konum temelli frekans hesabı ve yeniden üretilebilir analizler üzerinden anlatılacak. Literatürde bulunan sabit veya diziler için yeni keşif iddiası yapılmayacak.

**EN:** Observations, computationally verified results, hypotheses, and mathematical proofs will be distinguished explicitly. Both documentation languages will use identical definitions and findings. A visual pattern alone is not a proof. The project's contribution will be framed around validating the legacy study, an open C++ implementation, position-based frequency calculations, and reproducible analyses. Existing constants and sequences will not be presented as new discoveries.

### Reference starting points / Başlangıç kaynakları

- [OEIS A033307 — Decimal expansion of the Champernowne constant](https://oeis.org/A033307).
- [OEIS A132133 — Counts of punctual birds and corresponding early-bird counts](https://oeis.org/A132133).
- [OEIS A131881 — Punctual birds, complement of early birds](https://oeis.org/A131881).
- [Kurt Mahler — On the decimal expansion of certain irrational numbers, 1937 reprint](https://ems.press/books/dms/252/4981).
- **TR:** `../legacy-math-folders/` içindeki özgün rapor, PDF listeleri ve Ulam görselleri. **EN:** Original report, PDF lists, and Ulam figures in `../legacy-math-folders/`.

## 10. Decisions deferred to implementation / Uygulamada kesinleştirilecek kararlar

| Topic / Konu | Proposed direction / Önerilen yön |
| --- | --- |
| Test framework / Test çatısı | Select during M1; keep dependencies small / M1'de seç; bağımlılıkları az tut |
| CLI and JSON libraries / CLI ve JSON kütüphaneleri | Select during M1–M3 according to requirements / Gereksinimlere göre M1–M3'te seç |
| License / Lisans | User selection before public release / Public yayından önce kullanıcı seçimi |
| Large Ulam rendering / Büyük Ulam gösterimi | Decided: C++ + libpng; geometry and export rules in Section 13 / Karar: C++ + libpng; geometri ve çıktı kuralları Bölüm 13'te |
| Interactive report / Etkileşimli rapor | Optional HTML/JavaScript consumer of C++ outputs / C++ çıktıları kullanan isteğe bağlı HTML/JavaScript |
| Runtime target / Süre hedefi | Set after baseline measurements / İlk ölçümlerden sonra belirle |
| Schedule / Takvim | Estimate after M1–M2; milestones determine order / M1–M2 sonrası tahmin et; sıra aşamalara göre |

**Next step / Sonraki adım:** TR — Faz 3: `scan --max` toplu çıktıları, çalışma manifesti ve süre/bellek ölçümleri. EN — Phase 3: `scan --max` batch exports, run manifest, and runtime/memory measurements.

## 11. Literature review / Literatür araştırması

Research date / Araştırma tarihi: 2026-09-30.

**TR:** İnceleme; Mahler/Champernowne sabiti, erkenci sayıların tanımı ve sayımı, görünüm konumları, arama algoritmaları, sonlu öneklerde rakam dağılımı ve grafik araçlarını kapsar. Aşağıda kaynakta verilen sonuçlar ile proje için önerilen geliştirmeler ayrılmıştır. Birkaç sayfa web aracıyla açılamadığı için OEIS'in özgün kayıtları, C++ kaynak metni ve IEEE'nin özgün 2003 PDF'i doğrudan okunmuştur. Kullanıcı arşivinde değişiklik yapılmamıştır.

**EN:** The review covers Mahler/Champernowne constants, early-bird definitions and counts, occurrence positions, search algorithms, digit distributions in finite prefixes, and graphics tools. Published results are distinguished from proposed project developments below. Some pages could not be opened by the web tool; the original OEIS entries, C++ source text, and IEEE's original 2003 PDF were read directly. The user archive was not modified.

### 11.1 Source map / Kaynak haritası

| ID | Primary source / Birincil kaynak | Role in this project / Projedeki rolü |
| --- | --- | --- |
| R1 | [Mahler, 1937, original paper reprint](https://ems.press/books/dms/252/4981) | Base-specific concatenation and transcendence / Tabana göre birleştirme ve aşkınlık |
| R2 | [OEIS A033307](https://oeis.org/A033307) | Decimal digit sequence / Ondalık rakam dizisi |
| R3 | [Golomb, Early Bird Numbers, 2002, p. 10](https://www.itsoc.org/sites/default/files/2021-03/itNL1202.pdf) | Original puzzle statement / Özgün problem metni |
| R4 | [Golomb, Early Bird Numbers — Solutions, 2003, p. 30](https://www.itsoc.org/sites/default/files/2021-03/itNL0303.pdf) | Rotation certificates, carry cases, multiple occurrences, density / Döndürme tanıkları, elde durumları, çoklu görünümler, yoğunluk |
| R5 | [OEIS A116700](https://oeis.org/A116700) | Early-bird values and simple reference searches / Erkenci değerler ve basit referans aramalar |
| R6 | [OEIS A117804](https://oeis.org/A117804) | Natural-position formula / Doğal konum formülü |
| R7 | [OEIS A031297](https://oeis.org/A031297) | First occurrence of each positive integer / Her pozitif sayının ilk görünümü |
| R8 | [OEIS A220376](https://oeis.org/A220376) | First positions indexed by early-bird rank / Erkenci sıra numarasına göre ilk konumlar |
| R9 | [OEIS A160234](https://oeis.org/A160234) | Counts by target digit length / Hedef basamak uzunluğuna göre adetler |
| R10 | [OEIS A132133](https://oeis.org/A132133) | Complementary punctual-bird counts / Tamamlayıcı zamanında görünen sayı adetleri |
| R11 | [Arne Bouillon, C++ source, 2026-06-12](https://oeis.org/A132133/a132133.cpp.txt) | Source-integer reconstruction without generating the sequence / Diziyi üretmeden kaynak sayı yeniden kurma |
| R12 | [OEIS A390926](https://oeis.org/A390926) | Positions of successive occurrences / Ardışık görünümlerin konumları |
| R13 | [OEIS A393101](https://oeis.org/A393101) | KMP comparison counts for Champernowne searches / Champernowne aramalarında KMP karşılaştırma adetleri |
| R14 | [OEIS A390503](https://oeis.org/A390503) | Boyer–Moore comparison counts and precise variant / Boyer–Moore karşılaştırma adetleri ve kullanılan varyant |
| R15 | [Aho and Corasick, 1975, original paper](https://cr.yp.to/bib/1975/aho.pdf) | Searching multiple selected patterns / Seçilmiş çoklu örüntülerin aranması |
| R16 | [Becher and Graus, The discrepancy of the Champernowne constant](https://arxiv.org/abs/2407.13114) | Finite-prefix block counts and normality context / Sonlu önek blok sayımı ve normallik bağlamı |
| R17 | [Project Euler Problem 40](https://projecteuler.net/problem=40) | Additional digit-lookup example / Ek basamak sorgusu örneği |
| R18 | [Gnuplot terminal documentation](https://www.gnuplot.info/docs/Terminals.html) | SVG, pngcairo, pdfcairo output capabilities / Çıktı biçimleri |
| R19 | [libpng official reference library](https://www.libpng.org/pub/png/libpng.html) | Lossless raster encoding / Kayıpsız raster kodlama |

**TR:** R7'nin bazı programları başta `0` içeren diziyle sıfır tabanlı indeks kullanır; sonuçlar bizim başta `0` içermeyen, 1 tabanlı konumlarımızla örtüşebilir. Bu benzerlik körlemesine indeks dönüşümü yapma gerekçesi değildir: her kaynak için ilk örnekler kontrol edilecek. R8'in indeksi hedef sayı değil, erkenci sayının sırasıdır. R12 sonsuz dizide sonraki görünümleri de içerir; bu projenin `E(m)` alanı yalnızca erken görünümleri sayar.

**EN:** Some R7 programs use a leading-zero sequence with zero-based offsets, which can match our positive-only, one-based positions. This is not a reason to apply offsets blindly: initial examples must be checked for each source. R8 is indexed by early-bird rank, not target value. R12 includes subsequent occurrences in the infinite sequence, whereas this project's `E(m)` counts only early occurrences.

### 11.2 Findings relevant to the project / Projeyi etkileyen bulgular

**TR:** Golomb'un 2003 çözümü; küçük bir döndürülmüş sayıdan erken görünüm tanığı kurmayı, son basamak 9 olduğunda oluşan elde sorunlarını ve çoklu görünüm örneklerini tartışır. Yeni basamak kuralları geliştirirken döndürme ailesi ve elde durumları ayrı ele alınacak. Bu kaynak zaten var olan fikirlerin kaynağı olarak gösterilecek. [R4]

**EN:** Golomb's 2003 solution discusses certificates from smaller rotations, carry complications with a final 9, and multiple occurrences. Rotation families and carry cases will be separated when developing digit rules, with attribution to the existing ideas. [R4]

**TR:** Erkenci sayıların asimptotik yoğunluğunun 1 olması R5'te Golomb'a atfedilir. R4'teki tartışmayı burada bağımsız, eksiksiz kanıt olarak yeniden sunmayacağız. Sonlu tarama bu genel sonucu kanıtlamaz; bir milyon sınırındaki oran ayrı hesaplanır. [R4, R5]

**EN:** R5 attributes asymptotic density 1 to Golomb. We will not present the discussion in R4 as our own independent complete proof. Finite enumeration does not prove this general result; the proportion at one million is a separate calculation. [R4, R5]

**TR:** R11, Haziran 2026 tarihli bir C++ sınıflandırıcısıdır. Hedefin görünen ilk parçasını olası kaynak sayının son basamakları olarak yorumlar; kaynak uzunluğunu ve eldeyi kullanarak kaynağı yeniden kurar, ardından ardışık sayılarla devamı sınar. Erkenci/zamanında sınıflandırması yapar; ilk konum ve bütün erken konumları doğrudan üretmez. Kod burada çalıştırılmadı veya depoya kopyalanmadı. [R11]

**EN:** R11 is a June 2026 C++ classifier. It treats an initial target segment as a visible source suffix, reconstructs the source using its width and carry behavior, and checks continuation through consecutive integers. It returns early/punctual status rather than first or all early positions. It was neither executed nor copied into this repository. [R11]

**TR:** R16, normalliğin sonlu öneklerde kusursuz eşit rakam dağılımı anlamına gelmediğini açıklayan bağlam sağlar. Çalışmadaki klasik discrepancy, aşağıda önerilen sonlu blok sapmasıyla aynı ölçüt değildir. Öneğin nerede kesildiği kaydedilecek; yalnızca 10'un kuvvetlerinde yapılan ölçümlerle genel davranış çıkarılmayacak. [R16]

**EN:** R16 provides context for why normality does not imply perfectly uniform digit counts in finite prefixes. Its classical discrepancy is distinct from the finite-block deviation proposed below. Prefix endpoints will be recorded, and measurements solely at powers of ten will not be treated as representative of all endpoints. [R16]

### 11.3 External validation targets / Dış doğrulama hedefleri

| Target digit length / Hedef basamak sayısı | Early birds / Erkenci adet | All targets / Toplam |
| --- | ---: | ---: |
| 1 | 0 | 9 |
| 2 | 45 | 90 |
| 3 | 630 | 900 |
| 4 | 6,896 | 9,000 |
| 5 | 73,059 | 90,000 |
| 6 | 757,755 | 900,000 |

**TR:** R9/R10 değerlerinin toplamı `838385` eder. `1000000` erkenci olmayan bir 10 kuvvetidir; `1..1000000` için C++ taraması da **838.385** erkenci, yani **%83,8385** buldu. Dış hedef ve hesap sonucu ayrı kaynaklardır. İlk görünüm R7, erkenci sırası üzerinden R8 ile ayrıca denetlenebilir. [R7–R10]

**EN:** Summing R9/R10 yields `838385`. Since `1000000` is a non-early power of ten, the C++ scan also found **838,385** early birds, or **83.8385%**, over `1..1000000`. The external target and computed result have distinct provenance. First positions can also be checked against R7 and, by early-bird rank, R8. [R7–R10]

## 12. Algorithm development decisions / Algoritma geliştirme kararları

### 12.1 Three engines with different purposes / Farklı amaçlara sahip üç motor

| Engine / Motor | Role / Rol | Decision / Karar |
| --- | --- | --- |
| Independent substring search / Bağımsız metin araması | Small-data correctness reference / Küçük veri doğruluk referansı | Required / Zorunlu |
| Exact numeric windows / Tam sayısal pencereler | Complete batch scan with frequencies / Frekanslarla eksiksiz toplu tarama | Initial production baseline / İlk ana yöntem |
| Source reconstruction / Kaynak yeniden kurma | Per-target arithmetic search / Hedef başına aritmetik arama | Second engine and research priority / İkinci motor ve araştırma önceliği |

**TR:** N'inci rakam sorgusu ayrı, doğrudan aritmetik bir API olarak kalır. Doğal konum için R6'daki eşdeğer kısa formül de kontrol edilir: `P(m)=k*m+1-(10^k-1)/9`. `k`, tam sayı basamak hesabıyla belirlenir. Formül bilinen sonuç olarak kaynaklandırılır, yeni algoritma iddiasıyla sunulmaz.

**EN:** Digit-at-position lookup remains a separate direct arithmetic API. Check the equivalent concise natural-position formula in R6: `P(m)=k*m+1-(10^k-1)/9`, with integer digit-length computation. Attribute this known formula rather than claiming it as a new algorithm.

### 12.2 Boundary lemma / Sayı sınırı önermesi

**TR — Proje için çıkarım:** Erken görünüm bir kaynak sayı sınırından geçmek zorundadır. Kanıt: erken başlangıcın içinde bulunduğu kaynak sayı `a<m` olur; bu nedenle `digits(a)≤digits(m)=k`. Tek bir kaynak sayının içinde `k` rakamlık hedef bulunabilmesi için kaynak uzunluğu `k`, ofset sıfır ve kaynak `m` olmak zorundadır; bu ise `a<m` ile çelişir. Bu önerme, arama alanını sınırları geçen pencerelerle daraltır; ayrı bir normallik sonucu gerektirmez.

**EN — Project deduction:** Every early occurrence must cross a source-integer boundary. Proof: its initial source integer is `a<m`, hence `digits(a)≤digits(m)=k`. A `k`-digit target contained in one source would require a source of width `k`, offset zero, and source value `m`, contradicting `a<m`. This restricts the search to boundary-crossing windows without relying on normality.

**TR:** Bu, daha önce bilinmediği iddia edilen bir teorem değildir. Alternatif bir optimizasyon; her sınırın çevresinde yeterli ileri/geri rakam tutup yalnızca geçen pencereleri incelemektir. Çoklu sınır geçen eşleşmeler ilk geçtikleri sınıra atanmalı veya `(target, position)` ile tekilleştirilmelidir. Asimptotik olarak temel yöntemden daha iyi olduğu varsayılmayacak.

**EN:** No novelty claim is made for this lemma. An alternative optimization retains sufficient digits around each boundary and examines crossing windows only. Matches crossing multiple boundaries must be assigned to their first crossed boundary or deduplicated by `(target, position)`. An asymptotic improvement over the baseline is not assumed.

### 12.3 Extending source reconstruction / Kaynak yeniden kurmanın genişletilmesi

**TR:** R11'in fikrinden yararlanarak hedef uzunluğu `k` için görünen kaynak son eki uzunluğu `s=1..k-1` ve kaynak basamak sayısı `d=s..k` denenir. İlk hedef basamaklarından kaynak `a` adayları eldeyi dikkate alarak yeniden kurulur. Her adayın gerçek basamak sayısı, görünen son eki ve `a+1,a+2,...` ile devamı doğrulanır. Görünüm konumu `p=P(a)+d-s` olur; `p<P(m)` koşulu uygulanır.

**EN:** Following the idea in R11, enumerate visible suffix length `s=1..k-1` and source width `d=s..k` for a target of width `k`. Reconstruct candidate sources `a` from initial target digits with carry handling. Verify actual source width, suffix, and continuation through `a+1,a+2,...`. The occurrence position is `p=P(a)+d-s`; require `p<P(m)`.

**TR:** Orijinal sınıflandırıcı ilk tanıkta durur. Biz tüm doğrulanmış adayları gezip konumları tekilleştirerek `F(m)`, `E(m)` ve görünüm açıklamalarını hesaplamayı hedefliyoruz. Tamlık kanıtı, elde/basamak artışı durumları ve bağımsız testler tamamlanmadan bu genişletme üretim motoru sayılmayacak. Bir adayın doğrulanması için tüm dizi değil, yalnızca hedefi kapsayan yerel ardışık sayılar gerekir.

**EN:** The original classifier stops at its first witness. Our extension will examine all validated candidates and deduplicate positions to derive `F(m)`, `E(m)`, and explanations. It will not become a production engine before completeness arguments, carry/width-change cases, and independent tests are complete. Candidate validation requires only the local consecutive integers covering the target.

**TR:** `(s,d)` çifti sayısı `(k-1)(k+2)/2`'dir; 6 basamak için 20, 7 basamak için 27. Aday başına en fazla `O(k)` rakam doğrulamasıyla hedef başına `O(k^3)` basit üst sınır planlanır; büyük tam sayı aritmetiğinin maliyeti ayrıca hesaba katılır. Bir milyonluk taramadan kesin hızlı olduğu söylenemez. Aynı sonuçlar için süre, bellek ve doğrulanmış rakam sayısı ölçülür.

**EN:** There are `(k-1)(k+2)/2` suffix/width pairs: 20 for six digits and 27 for seven. With up to `O(k)` digit checks per candidate, use a simple `O(k^3)` per-target bound, accounting separately for arbitrary-precision arithmetic if added. It is not necessarily faster than the million-target window scan. Compare time, memory, and validated digit counts for identical results.

### 12.4 Rotation certificates and selected-pattern searches / Döndürme tanıkları ve seçili örüntü aramaları

**TR:** R4'ün basit yeter koşulu: `m`'nin başı sıfır olmayan, `r<m` sağlayan ve son rakamı 9 olmayan bir döndürmesi `r` bulunursa `r|r+1` erken görünüm tanığı verir. Bu yalnızca olumlu sınıflandırma kısa yoludur; koşulun sağlanmaması erkenci olmadığını göstermez, bulunan tanık da ilk görünüm veya bütün frekans değildir. `991`, `99|100` içinde erken görünmesine rağmen bu basit döndürme filtresinden geçmeyen yararlı bir örnektir.

**EN:** R4 provides a simple sufficient condition: a rotation `r<m` with no leading zero and final digit other than 9 gives a witness in `r|r+1`. This is a positive-classification shortcut only: failure does not imply punctuality, and a witness is neither necessarily the first occurrence nor the complete frequency. `991`, early in `99|100`, is a useful example not covered by this simple filter.

**TR:** KMP tek hedefin bağımsız arama/benchmark alternatifi; Aho–Corasick yalnızca asal/Fibonacci gibi seçilmiş hedef kümelerini taramak için araştırma alternatifi olacak. Bütün `1..N` hedeflerinde örüntü otomatonu ve çıktı maliyeti sayısal pencereye göre ölçülmeden eklenmeyecek. R13/R14 karşılaştırma adetleri kullanılan varyanta bağlıdır; duvar saati performansı veya frekans doğrulamasıyla eş tutulmaz. [R13–R15]

**EN:** KMP is an independent single-pattern search/benchmark option; Aho–Corasick is an experimental option for selected target sets such as primes or Fibonacci numbers. Do not add a full `1..N` pattern automaton without measuring its storage and output overhead against numeric windows. R13/R14 comparison counts depend on algorithm variants and are not runtime or frequency validations. [R13–R15]

### 12.5 New research questions / Yeni araştırma soruları

- **TR:** Erken görünüm hangi kaynak basamak uzunluklarında ve kaç sayı sınırında oluşuyor? **EN:** Which source widths and numbers of crossed boundaries produce early occurrences?
- **TR:** `9→10`, `99→100`, `999→1000` gibi artışlar ve sondaki 9 zincirleri frekansa nasıl katkı veriyor? **EN:** How do width changes and trailing-nine carry chains contribute to frequency?
- **TR:** Döndürme filtresi erkencilerin ne kadarını yakalıyor; kalanlar hangi ailelere ayrılıyor? **EN:** What fraction of early birds have simple rotation certificates, and how do uncovered families differ?
- **TR:** Göreli ilk görünüm `F(m)/P(m)` basamak grupları ve asal/bileşik sınıflarında nasıl dağılıyor? **EN:** How does `F(m)/P(m)` vary by digit length and prime/composite class?
- **TR:** Ulam çizgileri sayı özelliklerinden mi yoksa yerleşim ve basamak gruplarından mı kaynaklanıyor? **EN:** Are Ulam structures explained by numerical properties or layout and digit groups?

## 13. Graphics specification / Grafik üretim kararları

### 13.1 Selected toolchain / Seçilen araçlar

| Output / Çıktı | Producer / Üretici | Format / Biçim |
| --- | --- | --- |
| Statistical charts / İstatistik grafikler | C++ aggregates + versioned Gnuplot scripts / C++ özetleri + sürümlenen Gnuplot betikleri | SVG for README; PDF for print; PNG for previews / README için SVG; baskı için PDF; önizleme için PNG |
| Small Ulam spiral, N ≤ 10,000 / Küçük spiral | C++ vector renderer / C++ vektör çizimi | SVG; optional PNG / SVG; isteğe bağlı PNG |
| Large Ulam spiral, N > 10,000 / Büyük spiral | C++ raster renderer using libpng / libpng kullanan C++ raster çizimi | Lossless PNG / Kayıpsız PNG |
| Zoomable Ulam exploration / Yakınlaştırılabilir Ulam incelemesi | Later HTML Canvas viewer / Sonraki aşamada HTML Canvas görüntüleyici | Static HTML/JS + generated data; no Node runtime required / Statik HTML/JS + üretilen veriler; Node çalışma ortamı gerektirmez |

**TR:** İlk sürümde istatistikleri ve grafik verisini C++ hesaplayacak; Gnuplot yalnızca çizim yapacak. Gnuplot 6.x'in `svg`, `pngcairo`, `pdfcairo` terminalleri hedeflenir; mevcut derlemede bunların varlığı kontrol edilir. libpng büyük spiral için seçilen C/C++ uyumlu PNG kütüphanesidir. CMake grafik desteğini ayrı seçenek olarak sunacak: çekirdek sorgu ve CSV/JSON işlemleri grafik bağımlılıkları olmadan derlenebilecek. Grafik bağımlılıkları henüz kurulmadı. [R18, R19]

**EN:** C++ computes statistics and plot data; Gnuplot only renders. Target Gnuplot 6.x terminals `svg`, `pngcairo`, and `pdfcairo`, checking their availability in the installed build. libpng is the selected C/C++ compatible PNG library for large spirals. CMake will expose graphics as an optional feature so core queries and CSV/JSON operations build without graphics dependencies. Graphics dependencies have not been installed. [R18, R19]

### 13.2 Required figure set / Zorunlu grafik seti

| Figure / Grafik | Metric and presentation / Ölçüt ve gösterim |
| --- | --- |
| Early proportion / Erkenci oranı | Bars by digit length, exact counts and denominators; mark incomplete seven-digit group / Basamağa göre sütunlar, adet ve paydalar; tamamlanmamış 7 basamak grubunu işaretle |
| Cumulative density / Birikimli yoğunluk | `count(early m≤x)/x` at log-spaced endpoints plus decade boundaries; plot punctual proportion too / Logaritmik örnek uçları ve basamak sınırları; zamanında görünen oranını da göster |
| Frequency distribution / Frekans dağılımı | Discrete histogram by digit length, showing E=0; optional early-only view with its own denominator / Basamağa göre ayrık histogram, E=0 dahil; ayrı paydalı isteğe bağlı yalnız-erkenci görünüm |
| Relative first occurrence / Göreli ilk görünüm | ECDF of `F/P`; punctual targets at 1 reported separately / `F/P` ampirik birikimli dağılımı; 1'deki zamanında hedefleri ayrıca belirt |
| Prime comparison / Asal karşılaştırması | Prime vs composite early proportions within equal digit groups; exclude 1 / Aynı basamak grubunda asal/bileşik oranları; 1'i dışla |
| First-position map / İlk konum haritası | Binned heatmap of `(m,F(m))` with logarithmic axes and counts; no million SVG points / Log eksenli, hücre adetli ısı haritası; milyon SVG noktası üretme |
| Boundary mechanisms / Sınır mekanizmaları | Counts by source width, boundary count, and carry length, with explicit occurrence-level denominators / Kaynak uzunluğu, sınır adedi ve elde zincirine göre görünüm adetleri ve paydalar |
| Ulam comparison / Ulam karşılaştırması | Early, prime, intersection, punctual, frequency, and F/P layers / Erkenci, asal, kesişim, zamanında, frekans ve F/P katmanları |
| Digit/block deviation / Rakam ve blok sapması | Digit shares and a defined finite-block deviation across several prefix endpoints / Farklı önek uçlarında rakam payları ve tanımlı sonlu blok sapması |
| Performance / Performans | Time and peak memory per engine at equal scopes and output modes / Aynı kapsam ve çıktı modunda motorlara göre süre ve tepe bellek |

**TR:** Rakam/blok ölçümü için ayrı bir `T` rakam uzunluğu kullanılır; hedef sayı sınırı `N` ile karıştırılmaz. `q` uzunluklu bloklarda `T-q+1` örtüşen pencere sayılır; `00` gibi başta sıfır olan bloklar dahil edilir. Tanımlı ölçüt `delta_q(T)=max_w |count(w)/(T-q+1)-10^(-q)|`, başlangıç kapsamı `q=1,2,3` olacaktır. Bu, R16'daki klasik discrepancy'nin hesaplanması değildir. Eşleşen hedef sayıların `E(m)` frekansından da ayrıdır.

**EN:** Digit/block analysis uses a separate digit-prefix length `T`, distinct from target bound `N`. For length `q`, count `T-q+1` overlapping windows, including leading-zero blocks such as `00`. Define `delta_q(T)=max_w |count(w)/(T-q+1)-10^(-q)|`, initially for `q=1,2,3`. This does not compute the classical discrepancy in R16 and is separate from target early-frequency `E(m)`.

**TR:** Bunlar tam aralık sayımlarıdır; ana oran grafiklerine örneklem güven aralıkları eklenmez. Ulam yapısını sınamak için permütasyon deneyleri yapılırsa basamak grubu içindeki adetler korunur; tohum, tekrar sayısı, test ölçütü ve çoklu karşılaştırma seçimi kaydedilir. Böyle bir model matematiksel bağımsızlık varsayımını kanıtlamaz.

**EN:** These are full-range enumerations, so primary proportion charts do not carry sampling confidence intervals. If permutation experiments investigate Ulam structure, preserve counts within digit groups and record seed, repetitions, test statistic, and multiple-comparison choices. Such a model does not prove mathematical independence.

### 13.3 Ulam geometry and export / Ulam geometrisi ve çıktı

```text
7  8  9
6  1  2
5  4  3
```

**TR:** Merkez `(0,0)` konumunda 1; ilk adım sağa, ardından yukarı ve saat yönünün tersine. Matematiksel `y` yukarı büyür; görüntü satırlarına dönüşümde ters çevrilir. Hücre `m` sayısını temsil eder, `d_m` rakamını değil. Spiral boyutu `s`, `s²≥N` sağlayan en küçük tek tam sayıdır. `N=1000000` için **1001×1001** hücre kullanılır; 1.000.001–1.002.001 hücreleri veri dışı işaretlenir, erkenci olmayan hücre gibi sayılmaz.

**EN:** Place 1 at `(0,0)`, step right, then up and counterclockwise. Mathematical `y` grows upward and is inverted when mapping to image rows. Cell `m` represents integer `m`, not digit `d_m`. Use the smallest odd side length `s` with `s²≥N`. For `N=1000000`, use **1001×1001** cells; values 1,000,001–1,002,001 are outside the dataset and must not be classified as punctual.

**TR:** Bir milyonluk ham PNG'de hücre başına 1 piksel, ayrıntı sürümünde 4×4 piksel ile **4004×4004** görüntü üretilecek. Hücreler arasına boşluk veya yumuşatma konulmayacak. Sayı etiketleri yalnızca `N≤400` küçük görünümlerde açık olacak. Genel görünümde rakam yazıları milyon kez çizilmeyecek. Rapor başlığı ve renk açıklaması ham matrisin dışında tutulacak; görsele eşlik eden açıklama dosyasında sınır, yön ve renkler bulunacak.

**EN:** Produce a raw million-target PNG at one pixel per cell and a detailed **4004×4004** version at 4×4 pixels per cell. No cell gaps or smoothing. Integer labels default to small views with `N≤400`; the overview will not render a million text labels. Keep titles and legends outside the raw matrix, with a companion description recording range, orientation, and colors.

### 13.4 Visual conventions and reproducibility / Görsel kurallar ve yeniden üretim

**TR:** Dört sınıflı Ulam görünümünde varsayılan renkler: hiçbiri `#E6E6E6`, yalnız erkenci `#0072B2`, yalnız asal `#D55E00`, ikisi birden `#009E73`. Veri dışı alan koyu gri ve ayrı açıklamalıdır. Tek sınıflı görünümler renk yanında açıklama ve tablo sunar. Frekans/göreli konum için sabitlenmiş Cividis ölçeği kullanılır; aynı ölçütü karşılaştıran grafiklerde renk sınırları aynı tutulur. Frekans ayrıca zamanında/erkenci ayrımını korur; log renk dönüşümü varsa açıklanır.

**EN:** Default four-class Ulam colors: neither `#E6E6E6`, early only `#0072B2`, prime only `#D55E00`, both `#009E73`. Out-of-range cells are dark gray with a separate legend. Single-class views include textual descriptions and tables. Frequency/relative-position layers use a pinned Cividis scale with identical bounds for comparable plots. Frequency preserves the punctual/early distinction; logarithmic color transforms are disclosed.

**TR:** Beyaz zemin, 2B gösterim, okunabilir UTF-8 yazı ve sade eksenler kullanılır. Varsayılan grafik boyutu 1200×800 SVG ve 2400×1600 PNG'dir; çok panelli grafikler açık boyutlarla üretilir. Aynı veri ve ölçekle `--lang tr` / `--lang en` ayrı başlık/etiket çıktıları oluşturur. Kaynak dosyaları, özet CSV, grafik betiği, ölçekler, font, araç sürümleri ve veri checksum'ları saklanır. Platforma bağlı font/metadata farkları yüzünden bayt düzeyinde görüntü eşitliği vaat edilmez; veri ve geometrinin eşitliği doğrulanır.

**EN:** Use white backgrounds, 2D presentation, readable UTF-8 text, and simple axes. Default sizes are 1200×800 SVG and 2400×1600 PNG, with explicit dimensions for multipanel charts. Generate separate `--lang tr` and `--lang en` labels from identical data and scales. Retain inputs, aggregate CSV, plot scripts, scales, font, tool versions, and data checksums. Platform-dependent fonts/metadata prevent a promise of byte-identical images; validate data and geometry instead.

**TR:** GitHub README küçük SVG grafiklerini ve PNG spiral önizlemesini kullanır; ayrıntılı görüntüler ve tam veri GitHub Releases için hazırlanır. Yerel etkileşimli görüntüleyici ikinci aşamadır: ilk sürümün doğruluğu ve yayınlanabilir statik grafikler buna bağlı olmayacak. Gnuplot kurulumu yoksa hesaplamalar tamamlanır, çizim için açık bağımlılık bilgisi verilir.

**EN:** The GitHub README uses compact SVG charts and PNG spiral previews; detailed images and full data are prepared for GitHub Releases. A local interactive viewer is a later stage, not a prerequisite for first-release correctness or static publication figures. If Gnuplot is unavailable, computations still finish and plotting reports its dependency explicitly.

## 14. Current progress / Mevcut ilerleme

- [x] **TR:** Kullanıcı tercihleri: C++20, macOS, iki dil, komut satırı ve bir milyon sayı kapsamı. **EN:** User preferences established: C++20, macOS, bilingual documentation, CLI, and one-million range.
- [x] **TR:** Literatür araştırması ve algoritma seçenekleri kaydedildi. **EN:** Literature review and algorithm options documented.
- [x] **TR:** Grafik araçları, biçimleri ve Ulam geometri kuralları seçildi. **EN:** Graphics tools, formats, and Ulam geometry selected.
- [x] **TR:** Araştırma sonrası işler Bölüm 6'daki ilgili fazlara birleştirildi. **EN:** Post-research tasks integrated into their phases in Section 6.
- [x] **TR:** Kullanıcı uygulamaya başlamayı onayladı. **EN:** User approved starting implementation.
- [x] **TR:** Faz 0–1 tamamlandı: sözleşme ve çalışan yazılım temeli. **EN:** Phases 0–1 complete: specification and working software foundation.
- [x] **TR:** Faz 2 tamamlandı: doğrulanmış algoritmalar ve eski liste karşılaştırması. **EN:** Phase 2 complete: validated algorithms and archived-list comparison.
- [ ] **TR:** Faz 3–5 tamamlandı: veri, analiz ve grafikler. **EN:** Phases 3–5 complete: dataset, analyses, and figures.
- [ ] **TR:** Faz 6 tamamlandı: public yayına hazır sürüm. **EN:** Phase 6 complete: public-release-ready version.

**TR:** Temel sorgular ve erkencilik motorları tamamlandı; toplu veri dosyaları ve araştırma grafikleri henüz üretilmedi.

**EN:** Foundational queries and early-bird engines are complete; batch data files and research figures have not yet been generated.

## 15. Phase 0–1 delivery / Faz 0–1 teslimi

Date / Tarih: 2026-09-30.

- **TR:** C++20 `mahler_core` kütüphanesi ve `mahler` komut satırı oluşturuldu. **EN:** C++20 `mahler_core` library and `mahler` CLI created.
- **TR:** `digit`, doğal konum gösteren `inspect`, metin/JSON çıktıları, yardım ve sürüm bilgisi çalışıyor. **EN:** `digit`, natural-position `inspect`, text/JSON output, help, and version are implemented.
- **TR:** Sorgu sınırları ve veri sözleşmesi [Türkçe](docs/tr/SPECIFICATION.md) ve [İngilizce](docs/en/SPECIFICATION.md) belgelendi. **EN:** Query limits and data contracts are documented in both languages.
- **TR:** Apple Clang 21 ve proje içinde tutulan CMake 3.31.10 ile Release ve Debug/sanitizer derlemeleri doğrulandı. Yerel CMake `.tools/` içinde Git dışında tutulur; uygulama Python gerektirmez. **EN:** Release and Debug/sanitizer builds verified with Apple Clang 21 and local CMake 3.31.10. Local CMake remains untracked in `.tools/`; the application does not require Python.
- **TR:** Her iki derlemede `core` ve `cli` CTest grupları geçti. Bağımsız 10.000 sayılık önek, büyük konumlar, taşmalar, hatalı girdiler ve JSON kontrolleri yapıldı. **EN:** Both builds passed the `core` and `cli` CTest suites, covering an independent prefix through 10,000, large positions, overflows, invalid inputs, and JSON outputs.
- **TR:** Bir milyonluk önek üretiminin uzunluğu ve sonu doğrulandı; bu henüz erkencilik taraması değildir. **EN:** Million-integer prefix length and endpoint verified; this is not yet an early-bird scan.

## 16. Phase 2 delivery / Faz 2 teslimi

Date / Tarih: 2026-09-30. Detailed bilingual evidence / İki dilli ayrıntılı kanıt: [Phase 2 verification / Faz 2 doğrulaması](docs/PHASE2_REPORT.md).

- **TR:** `scan_early(maximum)` sayısal pencere motoru, `reconstruct_early_positions(m)` kaynak yeniden kurma motoru ve `early` metin/JSON komutu eklendi. **EN:** Added the numeric-window `scan_early(maximum)` engine, source reconstruction `reconstruct_early_positions(m)`, and text/JSON `early` command.
- **TR:** 9.999'a kadar her hedefin bütün erken konumları bağımsız örtüşmeli metin aramasıyla uyuştu. 1.000.000'a kadar her hedefin ilk konumu ve frekansı iki C++ motorunda eşleşti; seçili büyük/elde örnekleri metinle ayrıca doğrulandı. **EN:** Every early position through 9,999 agreed with independent overlapping substring search. Both C++ engines agreed on first positions and frequencies through 1,000,000; selected large/carry examples also matched direct search.
- **TR:** Bir milyon hedefte 838.385 erkenci bulundu; bir–altı basamak grupları dış adet hedefleriyle uyuştu. **EN:** The million-target scan found 838,385 early birds; digit groups one through six matched external counts.
- **TR:** Eski genel PDF'de 28 eksik kayıt ve 62 frekans farkı; asal PDF'de 1 eksik kayıt ve 6 frekans farkı raporlandı. Arşiv dosyaları değiştirilmedi. **EN:** The archived general PDF has 28 missing entries and 62 frequency differences; the prime PDF has one missing entry and six frequency differences. Archived files were not changed.
- **TR:** Toplu CSV/JSON, çalışma manifesti, performans ölçümleri ve grafikler sonraki fazlardadır. **EN:** Batch CSV/JSON, a run manifest, performance measurements, and figures belong to later phases.
