# Matematiksel tanımlar ve API sözleşmesi

[English](../en/SPECIFICATION.md)

Tarih: 2026-09-30. Faz 0 sözleşmesi ve Faz 1 uygulama kapsamı.

## Dizi ve konum kuralları

`S = concat(1,2,3,...)`, pozitif sayıların başta sıfır ve ayraç içermeyen standart ondalık gösterimlerini birleştirir. Konum 1'de rakam 1 vardır. Champernowne sabitinin başındaki `0.` gösterimi `S`'ye dahil değildir. `d_n`, n'inci kaynak sayıyı değil, rakam konumunu belirtir.

Pozitif `m` sayısının basamak sayısı `k(m)` olmak üzere:

```text
B(0) = 0
B(k) = sum(j=1..k, 9 * 10^(j-1) * j)
P(m) = 1 + B(k(m)-1) + k(m) * (m - 10^(k(m)-1))
```

`P(m)`, önceki görünümlerden bağımsız olarak `m`'nin kendi sırası geldiğindeki başlangıcıdır. Eşdeğer `P(m)=k(m)*m+1-(10^k(m)-1)/9` özdeşliği [OEIS A117804](https://oeis.org/A117804) içinde yer alır. Sonuç temsil edilebilirken büyük bir ara çarpım taşmasın diye uygulama, denetlenen basamak grubu toplamları kullanır.

Doğrudan sorguda `B(k-1)<n≤B(k)` koşulunu sağlayan `k` bulunur:

```text
r = n - B(k-1) - 1
source_number = 10^(k-1) + floor(r/k)
digit_offset = r mod k
```

Ofset soldan ve sıfır tabanlıdır. `digit_at(n)` ilgili kaynak rakamını çıkarır. İşlem maliyeti `O(log n)` basamak grubu adımı, sabit genişlikli sayılar için bellek sabittir. Taşacak bir grup uzunluğu, istenen konumdan büyük kabul edilir; işaretsiz taşmayla hesaplanmaz.

## Faz 2 için erken görünüm tanımları

Pozitif `m` için `F(m)` ilk eşleşmenin başlangıcıdır:

```text
is_early(m) = F(m) < P(m)
E(m) = P(m)'den küçük farklı eşleşme başlangıçlarının sayısı
A(m) = P(m) - F(m)
```

Örtüşmeler dahil farklı başlangıçlar sayılır. Doğal ve sonraki görünümler `E(m)` hesabına dahil edilmez. Eşleşme birden fazla kaynak sayı sınırını aşabilir. Başta sıfır bulunan pencere, pozitif hedefin standart gösterimi değildir.

Karşılaştırma görünümün başlangıcına göre yapılır. Kaynak uzunluğu, sınır sayısı ve elde zinciri görünüm düzeyindeki ölçütlerdir; hedef sayı adedi değildir. Erkenci olmayan hedeflerde `F=P`, `E=0`, `A=0` olur.

Araştırma aralığı `1≤m≤1.000.000` olarak sabittir. Önek, 1.000.000 kaynak sayısının tamamını içerir ve 5.888.896 rakam uzunluğundadır. Her hedefin doğal görünümü önekte vardır; bu aralık için ilk ve erken görünümler eksiksiz hesaplanabilir. Sonsuz dizide toplam frekans hesaplanmaz.

## Uygulanan kütüphane API'si

Başlık dosyaları: `include/mahler/sequence.hpp` ve `include/mahler/early.hpp`. Ad alanı: `mahler`.

| API | Sonuç ve sınırlar |
| --- | --- |
| `decimal_digits(number)` | Her `uint64_t` için basamak sayısı; yardımcı işlev sıfır için 1 döndürür |
| `locate_digit(position)` | `1..UINT64_MAX` için kaynak sayı, sıfır tabanlı ofset ve uzunluk |
| `digit_at(position)` | `1..UINT64_MAX` için 0–9 arasında rakam |
| `natural_position(number)` | `uint64_t` içinde temsil edilebilen doğal başlangıç |
| `make_prefix(last_integer)` | `1..1.000.000` sınırlarında `1..last_integer` birleştirmesi |
| `scan_early(maximum)` | `1..maximum` hedefleri için sayı ile indekslenen özet kayıtlar; `1≤maximum≤1.000.000` |
| `reconstruct_early_positions(number)` | Tek hedefin sıralı ve farklı erken konumları; `1..1.000.000` için ikinci araştırma motoru |

Basamak sayısı yardımcısı dışındaki API'ler sıfır girdide `std::invalid_argument` üretir. Bir milyonun üzerindeki önek/erkencilik isteği `std::length_error`, temsil edilemeyen doğal konum `std::overflow_error` üretir. Bellek tahsis hataları çağırana iletilir. `EarlyResult`, `natural_position`, `first_position`, `early_frequency` ile türetilmiş `is_early()` ve `advance_digits()` sonuçlarını içerir.

`UINT64_MAX = 18446744073709551615`. Doğal başlangıcı temsil edilebilen en büyük hedef `1029360799201087511`, başlangıcı `18446744073709551599`'dur. `UINT64_MAX` konumundaki rakam, bu hedefin 16 ofsetindeki 5 rakamıdır. Doğal konum sınırını aşan hedefler, sayısal değerleri `uint64_t` içinde olsa bile reddedilir.

## Komut satırı ve JSON şema sürümü 1

```text
mahler digit <position> [--format text|json]
mahler inspect <number> [--format text|json]
mahler early <number> [--format text|json]  # 1..1000000
mahler scan --max <number> --output <file> [--format csv|json]
            [--occurrences <csv-file>] [--manifest <json-file>]
mahler benchmark --max <number> [--engine window|reconstruct] [--repeat <count>]
mahler analyze --max <number> --output <json-file>
mahler ulam --max <number> --layer <name> --output <svg|png-file> [--cell-size <1..16>]
mahler plot-data --max <number> --output-dir <directory>
mahler legacy-csv --max <number> --output-dir <directory>
mahler --help
mahler --version
```

Varsayılan çıktı metindir. Girdiler işaret, boşluk, ayraç, kesir ve son ek içermeyen pozitif ondalık tam sayılardır. Baştaki sıfırlar kabul edilir ve normalleştirilir. Seçenekler sayısal girdiden sonra gelir. Bilinmeyen veya yinelenen seçenekler hatadır.

Çıkış kodları: 0 başarı; 1 çalışma/çıktı hatası; 2 kullanım veya geçersiz girdi; 3 girdi/doğal konum taşması. Başarılı sonuç stdout'a, hata stderr'e yazılır; hatalı sorgu stdout'ta kısmi sonuç üretmez. Komut satırı etiketleri ve hataları İngilizce, belgeler iki dildedir.

| Komut | JSON alanları |
| --- | --- |
| `digit` | `schema_version` (tam sayı), `position` (ondalık metin), `digit` (tam sayı), `source_number` (ondalık metin), `digit_offset` (tam sayı), `digit_count` (tam sayı) |
| `inspect` | `schema_version` (tam sayı), `number` (ondalık metin), `digit_count` (tam sayı), `natural_position` (ondalık metin) |
| `early` | `schema_version` (tam sayı), `number` (ondalık metin), `digit_count` (tam sayı), `natural_position` (ondalık metin), `first_position` (ondalık metin), `is_early` (mantıksal), `early_frequency` (tam sayı), `advance_digits` (ondalık metin) |

Büyük olabilecek konumlar, hedefler ve mesafeler kayıpsız okunabilmeleri için ondalık metindir. `early_frequency` sınırlı hedef aralığında tam sayıdır. `inspect` Faz 1 şemasını korur; erkencilik alanları için `early` kullanılır.

## Uygulanan veri seti sözleşmesi

`scan`, `1≤maximum≤1.000.000` ve `--output` ister. Özet kayıtlarda `number`, `digit_count`, `natural_position`, `first_position`, `is_early`, `early_frequency`, `advance_digits` bulunur; erkenci olmayanlar dahil her hedefe bir kayıt düşer. Varsayılan biçim CSV'dir. JSON; `schema_version:1`, ondalık metin `maximum`, tam sayı `record_count` ve `records` dizisi içeren tek bir nesnedir. Özel sayı alanları sonraki analizlere bırakılmıştır.

CSV; UTF-8, İngilizce alan adları, yerel ayraç içermeyen ondalık tam sayılar ve `true`/`false` kullanır. JSON hedef sayıları, konumları ve mesafeleri ondalık metin; basamak ve frekansları tam sayı olarak korur. Veri seti şema sürümü ayrıca `1` olarak belirtilir. Hesaplanmayan özel sayı alanları yazılmaz. `--occurrences`; `number`, `position`, `first_source`, `last_source`, `first_source_digit_offset` alanlı ayrı CSV üretir. Kayıtlar hedef ve konuma göre sıralıdır, yalnız doğal konumdan önceki başlangıçları içerir. Bu çıktı yeniden kurma yöntemini kullanır ve her hedefte frekansı/ilk konumu pencere motoruyla karşılaştırır.

Varsayılan manifest yolu `<output>.manifest.json`; `--manifest` ile değiştirilebilir. Dosya yolları birbirinden farklı olmalıdır. Manifest; aralık, indeksleme, sürüm, derleyici/derleme türü, sistem/model, motorlar, adetler, aşama süreleri, sürecin tepe belleği ve FNV-1a 64 dosya sağlama toplamlarını kaydeder. FNV tekrar üretim kontrolüdür, kriptografik imza değildir. `benchmark`, motor çekirdeğini 1–20 tekrar ölçer ve karşılaştırma için sonuç sağlama toplamı üretir. Dosya yazımı ile sağlama toplamı okuması `core_ms` dışındadır.

`legacy-csv`, `--output-dir` altında `early-birds.csv`, `prime-early-birds.csv` ve sağlama toplamı manifesti yazar. İki liste de hedefe göre sıralıdır; sütunları `number`, `digit_count`, `first_position`, `natural_position`, `early_frequency`, `advance_digits` şeklindedir ve her satır tanım gereği erkencidir. Asal liste, analiz ve grafiklerdeki Eratosthenes eleğini kullanır. Adı eski iki liste kategorisiyle uyumluluğu anlatır: arşivdeki dosyaları okumaz, değiştirmez veya yeniden üretmez.

`analyze`, aynı sınırlı aralık için şema sürümü 1 JSON üretir. Hedef basamağına göre adetleri, her basamak grubunun tam erken frekans histogramını, asal/bileşik/palindrom/emirp/Fibonacci paydalarını ve erkenci adetlerini, ilk görünüm uçlarını, basit döndürme tanığı kapsamını, görünüm mekanizmalarını ve 1–3 uzunluklu sonlu rakam blok istatistiklerini içerir. Asallık Eratosthenes eleğiyle hesaplanır; emirp, ters ondalık gösterimi farklı ve asal olan asal sayıdır. `crossed_boundaries`, `last_source-first_source` değeridir; `maximum_trailing_nines`, geçilen kaynak artışlarındaki en büyük elde uzunluğudur. Blok sapması `delta_q=max_w |count(w)/(T-q+1)-10^-q|` olup başında sıfır bulunanlar dahil bütün örtüşmeli blokları sayar. Bunlar normallik kanıtı değil, sonlu önek tanımlarıdır.

## Kaynak ve doğrulama politikası

- Rakam dizisi: [OEIS A033307](https://oeis.org/A033307).
- Doğal konum: [OEIS A117804](https://oeis.org/A117804).
- Erkencilik: [OEIS A116700](https://oeis.org/A116700).
- Dış adet hedefleri: [OEIS A160234](https://oeis.org/A160234).
- Araştırma kaynakları ve grafik kararları: [ROADMAP.md](../../ROADMAP.md).

OEIS indeks kuralları her dizi için ayrı incelenecek. Baştaki sıfır ve sıfır tabanlı indeks birbirinin görünür ofsetini dengeleyebilir; konumlara körlemesine kaydırma uygulanmayacak. Yol haritasındaki literatür erişim tarihi 2026-09-30'dur; çevrimiçi veriler sonradan genişletilebilir.

Normallik asimptotik bir özelliktir. Sonlu blok sapması `delta_q(T)` ve erken frekans `E(m)` farklı ölçütlerdir. Sonlu tarama veya grafik genel normallik teoremini kanıtlamaz. Bilinen sonuçlar, yeni hesaplamalı bulgular ve hipotezler ayrı işaretlenecek.

Çekirdek doğrulaması; 10.000'e kadar bağımsız metin birleştirmesi, bilinen örnek/sınırlar ve büyük konumlar için daha geniş aritmetik kullanan kapalı formül referansıyla yapılır. Yalnız test referansı Apple Clang/GCC `__uint128_t` uzantısını kullanır; uygulama standart C++20 sabit genişlikli aritmetik kullanır. Erkencilik doğrulaması 9.999'a kadar bütün konumları doğrudan metin aramasıyla, bir milyona kadar ilk konum ve frekansı iki motorla, seçili elde örneklerini de doğrudan aramayla karşılaştırır. Toplu çıktı testleri ve isteğe bağlı standart kütüphane [çıktı doğrulayıcısı](../../scripts/verify_exports.py) dosya tutarlılığını denetler. Eski dosyalar salt okunur kalır.
