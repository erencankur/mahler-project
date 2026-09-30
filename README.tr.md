# Mahler Projesi

[English](README.md)

Ondalık Champernowne sabiti, diğer adıyla Mahler sayısı, üzerine C++20 ile geliştirilen matematik araştırma projesi:

```text
0.123456789101112131415...
```

Çalışan yazılım temeli, doğrudan rakam sorgusu ve doğal konum hesabı sunar. Araştırmanın devamında 1–1.000.000 arasındaki bütün sayıların erkenciliği ve erken görünüm frekansları, asal sayı analizleri ve Ulam spirali grafikleri incelenecek.

## Mevcut özellikler

- Önceki rakamları üretmeden pozitif `uint64_t` konumundaki rakamı bulma.
- JSON çıktısında kaynak sayıyı ve sayı içindeki sıfır tabanlı rakam ofsetini gösterme.
- Taşma denetimiyle pozitif sayının doğal başlangıç konumunu hesaplama.
- Sonraki doğrulama ve taramalar için C++ kütüphanesinden sınırlı önek üretme.
- Metin veya JSON sorgu çıktısı.

Erkencilik, ilk görünüm araması, frekanslar, toplu CSV/JSON çıktıları ve grafikler **henüz uygulanmadı**. `inspect` şimdilik yalnızca doğal konumu gösterir. Erkencilik alanının bulunmaması, sayının erkenci olmadığı anlamına gelmez.

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

./build/mahler --help
./build/mahler --version
```

Konumlar 1'den başlar; baştaki `0.` sayılmaz. JSON'da konumlar ve hedef/kaynak sayılar, JavaScript gibi tüketicilerde hassasiyet kaybını önlemek için ondalık metin olarak yazılır. Rakam, ofset, basamak sayısı ve şema sürümü JSON sayısıdır.

## Doğrulama

Çekirdek kontrolleri rapordaki örnekleri, 10.000'e kadar bağımsız oluşturulmuş öneğin her konumunu, büyük konumları ve doğal konum taşmalarını, ayrıca 1.000.000'a kadar üretilen 5.888.896 rakamlık öneği kapsar. Komut satırı kontrolleri metin/JSON çıktısını, geçersiz girdileri ve çıkış kodlarını sınar. Bunlar temel sorguların doğrulamasıdır; **bir milyon sayı için erkencilik taraması henüz yapılmadı**.

Apple Clang ile ek bellek ve aritmetik kontrolleri:

```sh
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DMAHLER_ENABLE_SANITIZERS=ON
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure
```

## Matematik ve geliştirme planı

- [Matematiksel tanımlar ve API sözleşmesi](docs/tr/SPECIFICATION.md)
- [İki dilli yol haritası, araştırma kaynakları ve grafik kararları](ROADMAP.md)
- [OEIS A033307: Champernowne rakamları](https://oeis.org/A033307)
- [OEIS A117804: doğal konumlar](https://oeis.org/A117804)
- [OEIS A116700: erkenci sayılar](https://oeis.org/A116700)

Eski matematik çalışması kardeş `../legacy-math-project/` dizininde korunur. Uygulama çalışırken bu arşive ihtiyaç duymaz ve arşivi değiştirmez.

## Sürüm ve lisans durumu

Bu, `0.1.0` için geliştirme temelidir; araştırma sürümü henüz tamamlanmadı. Public yayından önce lisans seçilecek; şu anda açık kaynak kullanım lisansı verilmiş değildir.
