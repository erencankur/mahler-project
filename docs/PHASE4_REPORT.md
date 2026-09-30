# Phase 4 analysis / Faz 4 analizi

## Scope and status / Kapsam ve durum

**TR:** Bu rapordaki bütün sayımlar, onluk Champernowne dizisinde `1≤m≤1.000.000` hedefleri için tam sayımdır. Başlangıçtaki `0.` sayılmaz; konumlar 1 tabanlıdır. `E(m)` yalnız doğal konumdan önceki farklı başlangıçları sayar. Sonuçlar sonlu aralık gözlemleridir; sonsuz dizinin normalliği veya erken sayıların asimptotik yoğunluğu için yeni bir kanıt değildir.

**EN:** Every count in this report is a complete enumeration of targets `1≤m≤1,000,000` in the decimal Champernowne sequence. The initial `0.` is excluded and positions are one-based. `E(m)` counts distinct starts before the natural position only. Results are finite-range observations, not a new proof about normality of the infinite sequence or asymptotic early-bird density.

`mahler analyze --max 1000000 --output results/analysis-1000000.json` produces the machine-readable source for this report. It recomputes the early summaries, uses a sieve for primality, direct decimal reversal for palindromes/emirps, and the standard recurrence `1, 2, 3, 5, ...` for positive Fibonacci targets. It then uses the validated reconstruction engine to classify every early occurrence by its source boundary.

## Early-bird rate and frequency / Erkenci oranı ve frekansı

| Target digits / Hedef basamak | Targets / Hedef | Early / Erkenci | Rate / Oran | `E=0` | Most common nonzero frequencies / Yaygın sıfır dışı frekans |
| ---: | ---: | ---: | ---: | ---: | --- |
| 1 | 9 | 0 | 0.000% | 9 | — |
| 2 | 90 | 45 | 50.000% | 45 | `E=1`: 45 |
| 3 | 900 | 630 | 70.000% | 270 | `E=2`: 351 |
| 4 | 9,000 | 6,896 | 76.622% | 2,104 | `E=3`: 2,357 |
| 5 | 90,000 | 73,059 | 81.177% | 16,941 | `E=3`: 17,567 |
| 6 | 900,000 | 757,755 | 84.195% | 142,245 | `E=4`: 148,472 |
| 7 (`1,000,000` only) | 1 | 0 | 0.000% | 1 | — |
| **All targets / Tüm hedefler** | **1,000,000** | **838,385** | **83.839%** | **161,615** | — |

**TR:** Son satırdaki yedi basamak grubu tam bir grup değildir: yalnız `1.000.000` hedefini içerir. Bu yüzden basamak grupları arasındaki eğilimi yorumlarken 2–6 basamak grupları karşılaştırılmalıdır. Frekans dağılımı erken hedefleri tek tek değil, her `E(m)` değerinin kaç hedefte görüldüğünü sayar. Örneğin üç basamakta 351 hedefin iki erken başlangıcı vardır; bu, 351 değil 702 erken görünüm olduğu anlamına gelir.

**EN:** The seven-digit row is not a complete digit group: it contains only `1,000,000`. Compare two- through six-digit groups when discussing the apparent trend. The frequency distribution counts targets at each `E(m)`, not occurrences directly. For example, 351 three-digit targets have two early starts, contributing 702 early occurrences rather than 351.

### Examples and extremes / Örnekler ve uçlar

| Digits | Largest advance / En büyük mesafe | Largest `E(m)` / En büyük frekans | Smallest `F/P` / En küçük göreli ilk konum |
| ---: | --- | --- | --- |
| 2 | `91`, 163 digits | `12`, `E=1` | `89`, `F=8`, `P=168` |
| 3 | `990`, 2,691 digits | `121`, `E=2` | `789`, `F=7`, `P=2,257` |
| 4 | `9909`, 38,357 digits | `1121`, `E=4` | `7891`, `F=7`, `P=30,454` |
| 5 | `99900`, 485,802 digits | `11121`, `E=6` | `78910`, `F=7`, `P=383,440` |
| 6 | `999099`, 5,880,625 digits | `111211`, `E=9` | `789101`, `F=7`, `P=4,623,496` |

**TR:** Bu uçlar algoritmanın özel hedefi değildir; tüm aralık taranıp belirlenmiştir. `F/P` küçük olduğunda hedefin ilk görünümü doğal görünümünden çok daha erkendir. Mesafe ve oran farklı ölçütlerdir: büyük `P-F` her zaman en küçük `F/P` demek değildir.

**EN:** These extremes come from scanning the entire range, not from selected targets. Small `F/P` means the first appearance is far earlier relative to the natural appearance. Advance and relative position are different measures: the largest `P-F` need not have the smallest `F/P`.

## Prime and special subsets / Asal ve özel alt kümeler

| Digits | Prime early / Asal erkenci | Composite early / Bileşik erkenci | Palindrome early / Palindrom erkenci | Emirp early | Fibonacci early |
| ---: | ---: | ---: | ---: | ---: | ---: |
| 2 | 12 / 21 | 33 / 69 | 1 / 9 | 4 / 8 | 3 / 5 |
| 3 | 96 / 143 | 534 / 757 | 90 / 90 | 16 / 28 | 3 / 5 |
| 4 | 816 / 1,061 | 6,080 / 7,939 | 90 / 90 | 154 / 204 | 2 / 4 |
| 5 | 6,865 / 8,363 | 66,194 / 81,637 | 900 / 900 | 1,073 / 1,406 | 3 / 5 |
| 6 | 58,599 / 68,906 | 699,156 / 831,094 | 900 / 900 | 7,543 / 9,538 | 4 / 5 |

**TR:** Her hücre `erkenci / toplam` biçimindedir ve asal/bileşik paydaları aynı basamak grubu içindedir; `1` asal ya da bileşik sayılmaz. Emirp, ters çevrilmiş ondalık gösterimi farklı olan ve hem kendisi hem tersi asal olan sayıdır. Palindromlarda üç ila altı basamak gruplarında burada incelenenlerin tamamı erkencidir. Bu sonlu aralık için kesin bir sayımdır; daha büyük aralıklar için genelleme yapılmaz.

**EN:** Each cell is `early / total`, with prime/composite denominators inside the same digit group; `1` is neither prime nor composite. An emirp is a prime whose reversed decimal representation is a different prime. Every palindrome examined here in the three- through six-digit groups is early. That is an exact statement for this finite range, not a claim for larger ranges.

## Mechanisms of early occurrences / Erken görünümlerin mekanizmaları

An early occurrence always crosses at least one source-number boundary. For each of the 2,688,255 occurrences, `first_source` is the number containing its first digit and `last_source` contains its last digit. `crossed_boundaries = last_source - first_source`. `maximum_trailing_nines` is the largest number of terminal 9s among source values whose increment is crossed by that occurrence; it measures the strongest carry in the local span.

| Mechanism / Mekanizma | Count / Adet |
| --- | ---: |
| Starts in a 1-digit source / 1 basamaklı kaynakta başlar | 45 |
| Starts in a 2-digit source / 2 basamaklı kaynakta başlar | 720 |
| Starts in a 3-digit source / 3 basamaklı kaynakta başlar | 8,361 |
| Starts in a 4-digit source / 4 basamaklı kaynakta başlar | 78,696 |
| Starts in a 5-digit source / 5 basamaklı kaynakta başlar | 575,982 |
| Starts in a 6-digit source / 6 basamaklı kaynakta başlar | 2,024,451 |
| Crosses exactly 1 boundary / Tam 1 sınır geçer | 2,677,272 |
| Crosses 2 boundaries / 2 sınır geçer | 10,880 |
| Crosses 3–5 boundaries / 3–5 sınır geçer | 103 |
| Has no carry / Elde yok | 2,357,600 |
| Maximum carry length 1 / En büyük elde uzunluğu 1 | 291,945 |
| Maximum carry length 2 / En büyük elde uzunluğu 2 | 34,291 |
| Maximum carry length 3–5 / En büyük elde uzunluğu 3–5 | 4,419 |

**TR:** Örneğin `991`, `99|100` içinde erken görünür; bu görünüm iki sondaki 9 nedeniyle elde uzunluğu 2 olan bir sınırdan geçer. Buna karşılık erken görünümlerin çoğu elde içermez ve yalnız bir sayı sınırını geçer. Bu, elde mekanizmasının önemli ama gerekli olmayan bir kaynak olduğunu gösterir.

**EN:** For example, `991` occurs early in `99|100`, crossing a boundary with carry length 2 because of the two terminal 9s. Most early occurrences have no carry and cross exactly one boundary. Carry is therefore an important mechanism, but not a necessary one.

## Rotation certificates / Döndürme tanıkları

The implemented sufficient test examines every nontrivial cyclic rotation `r` of `m`. It accepts a certificate when `r<m`, `r` has no leading zero, and its final digit is not 9. Then the local concatenation `r|r+1` contains an early appearance of `m`. This is a one-way test: it proves a positive case but never classifies a target as punctual.

**TR:** `1..1.000.000` içinde 781.216 hedef sertifikadan geçti; bunların tamamı erkencidir. Bu, erkenci hedeflerin yaklaşık %93,18'ini kapsar. `991`, `99|100` içinde erken olmasına rağmen bu basit testten geçmeyen örnektir; bu nedenle kapsanmayan yaklaşık %6,82 erkenci hedef için sertifikanın başarısızlığı olumsuz sonuç değildir.

**EN:** In `1..1,000,000`, 781,216 targets passed the certificate and all are early. It covers about 93.18% of early targets. `991` is early in `99|100` but does not pass this simple test, so failure to receive a certificate is not a negative classification for the remaining roughly 6.82%.

## Digit blocks / Rakam blokları

The prefix here is `S_1000000`, with 5,888,896 digits. For each length `q`, all overlapping windows are counted, including blocks beginning with zero. The reported finite deviation is `delta_q=max_w |count(w)/(T-q+1)-10^-q|`; it is a descriptive finite-prefix measure, not the classical discrepancy used in some normality literature.

| `q` | Windows | Minimum block count | Maximum block count | Most frequent block | `delta_q` |
| ---: | ---: | ---: | ---: | --- | ---: |
| 1 | 5,888,896 | 488,895 | 600,001 | `1` | 0.016980194590 |
| 2 | 5,888,895 | 38,894 | 61,112 | `91` | 0.003395365344 |
| 3 | 5,888,894 | 2,893 | 6,224 | `899` | 0.000508736275 |

**TR:** Bu tablo, sonlu önekte blokların tam eşit dağılmadığını gösterir. Bu durum normal sayılarla çelişmez; normallik limit davranışına ilişkindir. `E(m)` frekansı ile blok sayımı da farklıdır: ilki hedef sayının doğal görünümünden önceki eşleşmeleri, ikincisi dizideki tüm kısa pencereleri sayar.

**EN:** The table shows that blocks are not exactly uniform in this finite prefix. This does not conflict with normality, which is a limiting property. Block counts and `E(m)` are also distinct: the former counts every short window in the sequence, while the latter counts occurrences before a target's natural position.
