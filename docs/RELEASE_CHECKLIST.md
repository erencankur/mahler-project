# v0.1.0 release checklist / v0.1.0 yayın kontrol listesi

Date / Tarih: 2026-09-30. This checklist records a release candidate, not a release / Bu liste sürüm adayını kaydeder, yayınlanmış sürümü değil.

| Item / Madde | Status / Durum | Evidence / Kanıt |
|---|---|---|
| Clean macOS core build, test, install, and CLI smoke tests / Temiz macOS çekirdek derleme, test, kurulum ve CLI duman testleri | Complete / Tamam | Commands below and CI workflow / Aşağıdaki komutlar ve CI |
| Optional libpng configuration / İsteğe bağlı libpng yapılandırması | Complete / Tamam | `MAHLER_ENABLE_GRAPHICS=ON` test suite / test grubu |
| Bilingual README and mathematical documentation / İki dilli README ve matematik belgeleri | Complete / Tamam | [README](../README.md), [Türkçe](../README.tr.md), [specifications](en/SPECIFICATION.md) |
| Reproducible selected figures and checksums / Yeniden üretilebilir seçilmiş grafikler ve sağlama toplamları | Complete / Tamam | [figure catalog](../figures/README.md) |
| Contribution guidance and release notes / Katkı rehberi ve sürüm notları | Complete / Tamam | [CONTRIBUTING](../CONTRIBUTING.md), [CHANGELOG](../CHANGELOG.md) |
| License text and copyright holder / Lisans metni ve telif sahibi | **Owner decision required / Sahip kararı gerekli** | No license has been selected / Lisans seçilmedi |
| Annotated tag and GitHub Release / İmzalı etiket ve GitHub Release | Pending license / Lisans bekleniyor | Create after the license decision / Lisans sonrası oluştur |

## Clean macOS reproduction / Temiz macOS yeniden üretim

**EN:** Start in a fresh clone. The commands below write only to a disposable build and install directory. A successful sequence proves that documented core usage does not rely on the legacy archive, `results/`, or local `.tools/`.

**TR:** Yeni bir klonda başlayın. Aşağıdaki komutlar yalnız geçici derleme ve kurulum dizinine yazar. Başarılı akış, belgelenen çekirdek kullanımın eski arşive, `results/` klasörüne veya yerel `.tools/` dizinine dayanmadığını gösterir.

```sh
git clone https://github.com/erencankur/mahler-project.git
cd mahler-project
cmake -S . -B /tmp/mahler-build -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/mahler-build --parallel
ctest --test-dir /tmp/mahler-build --output-on-failure
cmake --install /tmp/mahler-build --prefix /tmp/mahler-install
/tmp/mahler-install/bin/mahler digit 2020
/tmp/mahler-install/bin/mahler early 9910 --format json
```

Expected first output / Beklenen ilk çıktı: `7`. The JSON response identifies `9910` as early with first position `188` and frequency `4` / JSON yanıtı `9910` için ilk konumu `188`, frekansı `4` ve erkenci durumunu gösterir.

## Optional graphics and figure reproduction / İsteğe bağlı grafik ve görsel üretimi

```sh
brew install libpng gnuplot jq
cmake -S . -B /tmp/mahler-graphics -DCMAKE_BUILD_TYPE=Release -DMAHLER_ENABLE_GRAPHICS=ON
cmake --build /tmp/mahler-graphics --parallel
ctest --test-dir /tmp/mahler-graphics --output-on-failure
MAHLER=/tmp/mahler-graphics/mahler bash scripts/render_figures.sh
```

**EN:** The final command creates ignored full assets in `results/figures/`; it takes longer because it computes the million-target dataset and the figures. Compare generated checksums with the documented inventories when an identical toolchain is used. Data and geometry, rather than byte-identical graphic metadata, are the reproducibility criterion.

**TR:** Son komut bir milyonluk veri ve grafikleri hesapladığı için daha uzun sürer; tam çıktıları Git dışındaki `results/figures/` altında oluşturur. Aynı araç zinciri kullanıldığında üretilen sağlama toplamlarını belgelenen dökümlerle karşılaştırın. Yeniden üretilebilirlik ölçütü, görsel metadata’sının bayt düzeyinde eşitliği değil veri ve geometridir.

## License decision / Lisans kararı

**EN:** For a research CLI intended to invite broad reuse, MIT is the short permissive option. Apache-2.0 is also permissive and includes an express patent grant. GPL-3.0-or-later requires distributed derivatives to remain under the same license. Choose one only after deciding the intended reuse policy and confirming the copyright holder name. These are policy summaries, not legal advice. See [Choose a License](https://choosealicense.com/) for the official comparison.

**TR:** Geniş yeniden kullanıma açık bir araştırma CLI’si için MIT kısa ve izin verici seçenektir. Apache-2.0 da izin vericidir ve açık patent lisansı içerir. GPL-3.0-or-later dağıtılan türevlerin aynı lisansla kalmasını ister. Hedeflenen yeniden kullanım politikasına ve telif sahibinin adına karar verdikten sonra seçin. Bunlar hukukî tavsiye değil, politika özetleridir. Karşılaştırma için [Choose a License](https://choosealicense.com/) kaynağına bakın.

## Repository contents review / Depo içeriği incelemesi

**EN:** Commit source, tests, documentation, compact figures/data, and workflows. Keep builds, `results/`, local tools, the sibling legacy archive, and unreviewed bulk release assets out of Git. The full one-million summaries and full figure set are intentionally regenerated, or attached to a GitHub Release after the license decision.

**TR:** Kaynak kodu, testler, belgeler, küçük görseller/veriler ve iş akışları commit edilir. Derlemeler, `results/`, yerel araçlar, kardeş eski arşiv ve gözden geçirilmemiş büyük sürüm dosyaları Git dışında tutulur. Tam bir milyonluk özetler ve tam görsel seti bilinçli olarak yeniden üretilir veya lisans kararından sonra GitHub Release’e eklenir.
