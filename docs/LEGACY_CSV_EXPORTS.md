# Legacy-compatible CSV exports / Eski listeyle uyumlu CSV çıktıları

The historical archive is a read-only collection of PDF/DOCX documents. Its two
list documents distinguish all early-bird numbers from the prime subset.
`legacy-csv` regenerates those two conceptual lists from the verified C++
engine, with a stable CSV schema and audit fields that the archived tables did
not consistently contain.

Eski arşiv salt okunur PDF/DOCX belgelerinden oluşur. İki liste belgesi tüm
erkenci sayıları ve asal alt kümeyi ayırır. `legacy-csv`, bu iki kavramsal
listeyi doğrulanmış C++ motorundan yeniden üretir; eski tablolarda tutarlı
bulunmayan denetim alanlarını da sabit CSV şemasına ekler.

```sh
./build/mahler legacy-csv --max 1000000 --output-dir results/legacy-compatible
```

The command creates the following UTF-8 files. They use LF line endings,
English headers, decimal integers without thousands separators, and no quoted
fields.

| File | Records at 1,000,000 | Meaning |
| --- | ---: | --- |
| `early-birds.csv` | 838,385 | Every early-bird target in increasing numeric order |
| `prime-early-birds.csv` | 66,388 | Prime early-bird targets in increasing numeric order |
| `manifest.json` | — | Range, schema, record counts, and FNV-1a-64 checksums |

Both CSV files have the same columns:

| Column | Meaning |
| --- | --- |
| `number` | Positive target integer `m` |
| `digit_count` | Decimal digit width of `m` |
| `first_position` | `F(m)`, its first appearance in the sequence |
| `natural_position` | `P(m)`, where `m` appears as its own concatenated integer |
| `early_frequency` | Number of distinct appearances strictly before `P(m)` |
| `advance_digits` | `P(m) - F(m)` |

Positions are one-based and exclude the initial `0.` of the decimal constant.
Every row is early, so an `is_early` column would be redundant. The all-target
summary from `scan` is complementary: it includes punctual numbers too and
therefore retains the `is_early` boolean.

Kayıtlardaki konumlar bir tabanlıdır ve sayının başındaki `0.` hesaba katılmaz.
Her satır zaten erkenci olduğundan `is_early` sütunu gereksizdir. `scan` ile
üretilen tüm-hedef özeti tamamlayıcıdır: erkenci olmayan sayıları da içerdiği
için `is_early` alanını korur.

For a small, inspectable run:

```sh
./build/mahler legacy-csv --max 100 --output-dir results/legacy-small
head -n 4 results/legacy-small/early-birds.csv
```

The first data row is `12,2,1,14,1,13`: `12` first occurs at the opening
digits `1|2`, before its natural start at position 14.

These outputs do not copy, rewrite, or depend on the sibling legacy archive.
The archival comparison and its known historical omissions are documented in
[Phase 2](PHASE2_REPORT.md).
