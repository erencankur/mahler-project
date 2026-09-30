# v0.1.0 release checklist / v0.1.0 yayın kontrol listesi

Date / Tarih: 2026-09-30. `v0.1.0` is the initial public release / `v0.1.0` ilk public sürümdür.

| Item / Madde | Status / Durum | Evidence / Kanıt |
|---|---|---|
| Clean macOS core build, test, install, and CLI smoke tests / Temiz macOS çekirdek derleme, test, kurulum ve CLI duman testleri | Complete / Tamam | Commands below and CI workflow / Aşağıdaki komutlar ve CI |
| Optional libpng configuration / İsteğe bağlı libpng yapılandırması | Complete / Tamam | `MAHLER_ENABLE_GRAPHICS=ON` test suite / test grubu |
| Bilingual README and mathematical documentation / İki dilli README ve matematik belgeleri | Complete / Tamam | [README](../README.md), [Türkçe](../README.tr.md), [specifications](en/SPECIFICATION.md) |
| Reproducible selected figures and checksums / Yeniden üretilebilir seçilmiş grafikler ve sağlama toplamları | Complete / Tamam | [figure catalog](../figures/README.md) |
| Contribution guidance and release notes / Katkı rehberi ve sürüm notları | Complete / Tamam | [CONTRIBUTING](../CONTRIBUTING.md), [CHANGELOG](../CHANGELOG.md) |
| License text and copyright holder / Lisans metni ve telif sahibi | Complete / Tamam | [MIT License](../LICENSE), © 2026 Eren Can Kur |
| Annotated tag and GitHub Release / İmzalı etiket ve GitHub Release | Complete / Tamam | `v0.1.0` and linked release notes / etiket ve bağlı sürüm notları |

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

**EN:** MIT was selected as the short permissive license for broad reuse of this research CLI. It requires preservation of the copyright and license notice, while disclaiming warranty and liability. See the [MIT license overview](https://choosealicense.com/licenses/mit/) for a policy summary.

**TR:** Bu araştırma CLI’sinin geniş yeniden kullanımı için kısa ve izin verici MIT lisansı seçildi. Telif ve lisans bildiriminin korunmasını ister; garanti ve sorumluluğu reddeder. Politika özeti için [MIT lisans açıklamasına](https://choosealicense.com/licenses/mit/) bakın.

## Repository contents review / Depo içeriği incelemesi

**EN:** The tagged release contains source, tests, documentation, compact figures/data, and workflows. Builds, `results/`, local tools, the sibling legacy archive, and unreviewed bulk release assets remain outside Git. Full one-million summaries and the complete figure set are intentionally regenerated from documented commands.

**TR:** Etiketli sürüm kaynak kodu, testler, belgeler, küçük görseller/veriler ve iş akışlarını içerir. Derlemeler, `results/`, yerel araçlar, kardeş eski arşiv ve gözden geçirilmemiş büyük sürüm dosyaları Git dışında kalır. Tam bir milyonluk özetler ve tam görsel seti bilinçli olarak belgelenen komutlarla yeniden üretilir.
