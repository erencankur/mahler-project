# Phase 2 verification / Faz 2 doğrulaması

## Scope / Kapsam

**TR:** Hedefler `1..1.000.000`; konumlar baştaki `0.` hariç, 1 tabanlıdır. `P(m)` doğal başlangıç, `F(m)` ilk başlangıç, `E(m)` ise `P(m)` öncesindeki farklı başlangıçların adedidir. Örtüşmeler sayılır. Eski PDF'ler `../legacy-math-folders/` içinde salt okunur incelendi. PDF karşılaştırması yalnız `1..9.999` aralığını kapsar.

**EN:** Targets are `1..1,000,000`; positions are one-based after excluding the initial `0.`. `P(m)` is the natural start, `F(m)` the first start, and `E(m)` counts distinct starts before `P(m)`, including overlaps. The archived PDFs in `../legacy-math-folders/` were read only. PDF comparisons cover `1..9,999`.

## Engines and completeness / Motorlar ve tamlık

**TR:** Kayan pencere motoru `S_N=concat(1,...,N)` metninin her başlangıcında 1–`digits(N)` uzunluğunda pencereleri tarar. Baştaki sıfırlar ve `N` üstündeki değerler atılır. Bir eşleşme yalnız `p<P(m)` olduğunda kaydedilir. Her pozitif hedefin doğal görünümü `S_N` içinde olduğundan ilk ve erken görünümler bu önekle eksiksiz belirlenir. Maliyet `O(|S_N| digits(N))` zaman ve `O(|S_N|+N)` bellektir.

**EN:** The rolling-window engine checks lengths 1 through `digits(N)` at every start in `S_N=concat(1,...,N)`. It discards leading-zero windows and values above `N`, and records a match only when `p<P(m)`. Every target's natural occurrence is in `S_N`, so the prefix suffices for complete first and early occurrences. Complexity is `O(|S_N| digits(N))` time and `O(|S_N|+N)` memory.

**TR:** Yeniden kurma motoru tek hedef için erken görünümün başladığı kaynak `a<m` sayısını arar. Erken görünüm bir kaynak sınırını aşmak zorundadır. Hedefin ilk `s` rakamı `a` sayısının son eki, sonraki rakamlar `a+1` sayısının başlangıcıdır; `1≤s<digits(m)` ve `s≤digits(a)≤digits(m)`. `digits(a+1)` için aynı genişlik ve bir basamak artışı ayrı denenir. Gözlenen `a+1` ön eki bir tamsayı aralığı, `a` son eki ise `a mod 10^s` koşulu verir. Kesişimdeki adayların gerçek ardışık sayı metni doğrulanır; konumlar sıralanıp tekilleştirilir. Bu aralık kesişimi her olası `a` değerini kapsar. Kod başka bir çalışmadan kopyalanmadı; R11'deki kaynak yeniden kurma fikri genişletildi.

**EN:** Per-target reconstruction searches for a starting source `a<m`. Every early match crosses a source boundary. The first `s` target digits are a suffix of `a`, and the following digits begin `a+1`, with `1≤s<digits(m)` and `s≤digits(a)≤digits(m)`. Both unchanged width and a one-digit carry for `a+1` are considered. The observed prefix of `a+1` gives an integer interval; the suffix of `a` gives `a mod 10^s`. Candidate sources in their intersection are checked against the actual consecutive-number text, then their positions are sorted and deduplicated. These cases cover every possible `a`. The implementation extends the reconstruction idea in R11 without copying its code.

**TR:** Üç ayrı denetim yapıldı: 1–9.999 için örtüşmeli doğrudan metin araması bütün erken konumları doğruladı; 1–1.000.000 için iki C++ motorunun `F(m)` ve `E(m)` değerleri her hedefte eşleşti; büyük/elde örneklerinde doğrudan metin araması yeniden kurulan bütün konumları doğruladı. Bu hesaplamalı doğrulama, kaynak aralığı ve `s` durumları için yukarıdaki tamlık gerekçesini destekler. Alternatif motorun performans seçimi ve daha geniş sayısal kapsam sonraki araştırma konusudur.

**EN:** Three checks were used: overlapping direct substring search confirmed every early position for 1–9,999; both C++ engines agreed on `F(m)` and `E(m)` for every target through one million; direct substring search also confirmed all reconstructed positions for selected carry and large examples. This computational evidence supports the interval and `s` case argument above. Choosing a default engine by measured performance and extending the numeric range remain research work.

## Counts / Sayımlar

| Digits / Basamak | Targets / Hedefler | Early / Erkenci |
| ---: | ---: | ---: |
| 1 | 9 | 0 |
| 2 | 90 | 45 |
| 3 | 900 | 630 |
| 4 | 9,000 | 6,896 |
| 5 | 90,000 | 73,059 |
| 6 | 900,000 | 757,755 |
| 7 (only 1,000,000) | 1 | 0 |
| **Total / Toplam** | **1,000,000** | **838,385** |

**TR:** Bu sayımlar C++ test çalışmasının çıktısıyla doğrulandı ve bağımsız [OEIS A160234](https://oeis.org/A160234) hedefleriyle uyuşuyor. `1.000.000` erkenci olmayan bir 10 kuvvetidir. Buradaki sonuç frekansların tam dağılımı ya da grafikler değildir; onlar sonraki fazlarda üretilecek.

**EN:** These counts were verified by the C++ test run and match the external targets in [OEIS A160234](https://oeis.org/A160234). `1,000,000` is a non-early power of ten. Full frequency distributions and figures are later-phase outputs.

## Archived list comparison / Eski listelerin karşılaştırması

| PDF | Listed / Listelenen | Expected / Beklenen | Missing / Eksik | Extra / Fazla | Frequency differences / Frekans farkı |
| --- | ---: | ---: | ---: | ---: | ---: |
| `Erkenci Sayılar.pdf` | 7,543 | 7,571 | 28 | 0 | 62 (56 low, 6 high) |
| `Asal Erkenci Sayılar.pdf` | 923 | 924 | 1 | 0 | 6 (5 low, 1 high) |

**TR:** Genel listede eksik hedefler: `1323, 1424, 1525, 1626, 1727, 1828, 1929, 2434, 2535, 2636, 2737, 2838, 2939, 3545, 3646, 3747, 3848, 3949, 4656, 4757, 4858, 4959, 5767, 5868, 5969, 6878, 6979, 7989`. Asal listede eksik hedef `2939`dur. PDF'nin `(xN)` frekans etiketleri karşılaştırıldı; PDF'de verilen sınır açıklamalarının tamamı doğrulanmış konum verisi sayılmadı.

**EN:** Missing general-list targets: `1323, 1424, 1525, 1626, 1727, 1828, 1929, 2434, 2535, 2636, 2737, 2838, 2939, 3545, 3646, 3747, 3848, 3949, 4656, 4757, 4858, 4959, 5767, 5868, 5969, 6878, 6979, 7989`. The prime list is missing `2939`. The PDF `(xN)` frequency labels were compared; its boundary annotations were not all treated as verified occurrence positions.

| Target / Hedef | Evidence / Kanıt | PDF / C++ frequency |
| ---: | --- | ---: |
| `1323` | Start 53 in `31|32|33`; absent from the general list / Genel listede yok | absent / 1 |
| `2939` | Start 175 in `92|93|94`; absent from both lists / İki listede yok | absent / 1 |
| `891` | Starts 8 (`8|9|10`) and 458 (`189|190`) / İki ayrı konum | 1 / 2 |
| `1121` | Starts 13, 226, 524, 3339 / Dört ayrı konum | 3 / 4 |
| `1234` | Start 1 only; the same boundary description appears twice / Aynı açıklama iki kez yazılmış | 2 / 1 |

**TR:** Eksik kayıt ve düşük frekans örnekleri, birden fazla kaynak sınırını aşan veya ardışık sayıların birleşiminden oluşan eşleşmelerin atlanmasıyla tutarlıdır. Yüksek frekans örneği `1234` ise aynı başlangıcın iki kez yazılmasıdır. Bütün 62 farkın tek bir ortak nedene dayandığı iddia edilmez. Eski PDF'ler değiştirilmedi.

**EN:** Missing entries and low frequencies are consistent with overlooked matches spanning multiple source boundaries or consecutive source numbers. The high count for `1234` repeats a single start. We do not claim one common cause for all 62 differences. The archived PDFs were left untouched.

## Carry and negative examples / Elde ve erkenci olmayan örnekler

| Target / Hedef | Natural `P` | Early positions / Erken konumlar |
| ---: | ---: | --- |
| `991` | 2,863 | 188, 2,619 |
| `919` | 2,647 | 172, 459 |
| `9193` | 35,662 | 6,609, 14,567 |
| `9199` | 35,686 | 2,647, 2,864, 6,849 |
| `11121` | 44,495 | 12, 225, 3,338, 3,736, 7,335, 44,451 |
| `9090` | 35,250 | none / yok |
| `900900` | 5,294,290 | none / yok |

**TR:** `991` için `99|100` elde sınırını verir. `9090` ve `900900` örneklerinde erken başlangıç bulunmadı; bunlar seçili olumsuz sınıflandırma testleridir, genel bir sayı ailesi teoremi değildir.

**EN:** `991` includes the carry boundary `99|100`. `9090` and `900900` have no early starts; these are selected negative-classification checks, not a theorem about an entire family.
