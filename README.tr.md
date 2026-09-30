# Mahler Projesi

[English](README.md)

Ondalık Champernowne sabiti, diğer adıyla Mahler sayısı, üzerine C++20 ile geliştirilen matematik araştırma projesi:

```text
0.123456789101112131415...
```

C++ uygulaması doğrudan rakam sorgusu, erkencilik araması ve 1–1.000.000 aralığındaki hedefler için yeniden üretilebilir CSV/JSON çıktıları sunar. Asal sayı analizleri ve Ulam spirali grafikleri sonraki fazlardadır.

## Mevcut özellikler

- Önceki rakamları üretmeden pozitif `uint64_t` konumundaki rakamı bulma.
- JSON çıktısında kaynak sayıyı ve sayı içindeki sıfır tabanlı rakam ofsetini gösterme.
- Taşma denetimiyle pozitif sayının doğal başlangıç konumunu hesaplama.
- Sonraki doğrulama ve taramalar için C++ kütüphanesinden sınırlı önek üretme.
- Metin veya JSON sorgu çıktısı.
- `early` komutuyla ilk görünüm, erkencilik, erken frekans ve erkencilik mesafesi hesabı.
- Sayısal pencere taramasını bağımsız hedef başına yeniden kurma motoruyla karşılaştırma.
- Hedef özetlerini, isteğe bağlı erken konum kayıtlarını ve sağlama toplamlı çalışma manifestini dışa aktarma.
- Dosya yazımını çekirdek ölçümden ayırarak iki motorun süresini ölçme.
- Frekansları, özel sayı kümelerini, kaynak sınırı mekanizmalarını, döndürme tanıklarını ve sonlu rakam bloklarını analiz etme.

Toplu matematiksel analiz ve grafikler **henüz uygulanmadı**. `inspect` yalnızca doğal konumu gösterir; erkencilik bilgileri için `early` kullanılır.

## macOS üzerinde derleme

Gereksinimler: Xcode Command Line Tools veya Xcode, CMake 3.20 ve üzeri, C++20 derleyicisi. İlk desteklenen ortam macOS üzerinde Apple Clang'dir. Bu fazda Python, Gnuplot veya PNG kütüphanesi gerekmez.

Geliştirici araçları yoksa `xcode-select --install` ile kurulabilir. CMake'i [resmî indirme sayfasından](https://cmake.org/download/) veya paket yöneticinden edinebilirsin.

Depo kökünde çalıştır:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Kullanım

```sh
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

./build/mahler scan --max 1000000 --format csv --output results/summary-1000000.csv --occurrences results/occurrences-1000000.csv
./build/mahler scan --max 1000000 --format json --output results/summary-1000000.json
./build/mahler benchmark --max 1000000 --engine window --repeat 3
./build/mahler analyze --max 1000000 --output results/analysis-1000000.json

./build/mahler --help
./build/mahler --version
```

Konumlar 1'den başlar; baştaki `0.` sayılmaz. JSON'da konumlar ve hedef/kaynak sayılar, JavaScript gibi tüketicilerde hassasiyet kaybını önlemek için ondalık metin olarak yazılır. Rakam, ofset, basamak sayısı ve şema sürümü JSON sayısıdır.

## Doğrulama

Çekirdek kontrolleri 10.000'e kadar bağımsız oluşturulmuş öneği, büyük konumları ve taşmaları kapsar. Erkencilik kontrolleri 9.999'a kadar bütün erken konumları doğrudan metin aramasıyla, bir milyona kadar her hedefin ilk konumunu ve frekansını iki C++ motoruyla karşılaştırır; seçili elde örnekleri ayrıca bağımsız aranır. Bir milyonluk taramada **838.385** erkenci sayı ve **2.688.255** erken konum bulundu. Toplu çıktı testleri şemaları, tekrarları ve hatalı seçenekleri sınar. İsteğe bağlı [çıktı doğrulayıcısı](scripts/verify_exports.py) tam CSV/JSON dosyalarını ve her konumu dizi metnine karşı denetler. Sonuçlar ve ölçümler [Faz 3 raporunda](docs/PHASE3_REPORT.md).

[Faz 4 analizi](docs/PHASE4_REPORT.md), bir milyonluk aralıktaki frekansları, asal ve özel sayı kümelerinin paydalarını, sınır mekanizmalarını ve sonlu blok istatistiklerini açıklar.

Apple Clang ile ek bellek ve aritmetik kontrolleri:

```sh
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DMAHLER_ENABLE_SANITIZERS=ON
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure
```

## Matematik ve geliştirme planı

- [Matematiksel tanımlar ve API sözleşmesi](docs/tr/SPECIFICATION.md)
- [İki dilli yol haritası, araştırma kaynakları ve grafik kararları](ROADMAP.md)
- [Faz 2 doğrulaması ve eski listelerin karşılaştırması](docs/PHASE2_REPORT.md)
- [Faz 3 veri seti, sağlama toplamları ve ölçümleri](docs/PHASE3_REPORT.md)
- [Faz 4 matematiksel analiz ve örnekler](docs/PHASE4_REPORT.md)
- [OEIS A033307: Champernowne rakamları](https://oeis.org/A033307)
- [OEIS A117804: doğal konumlar](https://oeis.org/A117804)
- [OEIS A116700: erkenci sayılar](https://oeis.org/A116700)

Eski matematik çalışması kardeş `../legacy-math-folders/` dizininde korunur. Uygulama çalışırken bu arşive ihtiyaç duymaz ve arşivi değiştirmez.

## Sürüm ve lisans durumu

Bu, `0.1.0` için geliştirme temelidir; araştırma sürümü henüz tamamlanmadı. Public yayından önce lisans seçilecek; şu anda açık kaynak kullanım lisansı verilmiş değildir.
