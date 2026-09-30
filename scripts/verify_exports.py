"""Optional standard-library verifier for completed batch exports.

The calculation and export application is C++; Python is only used here to
parse two common interchange formats without adding a C++ JSON dependency.
"""

import argparse
import bisect
import csv
import json
from pathlib import Path


def verify(csv_path: Path, json_path: Path, occurrence_path: Path, manifest_path: Path) -> None:
    with json_path.open() as stream:
        dataset = json.load(stream)
    records = dataset["records"]
    maximum = int(dataset["maximum"])
    assert dataset["schema_version"] == 1
    assert dataset["record_count"] == len(records) == maximum

    starts = []
    early_count = 0
    with csv_path.open(newline="") as stream:
        rows = csv.DictReader(stream)
        for number, (csv_row, json_row) in enumerate(zip(rows, records), 1):
            assert csv_row["number"] == json_row["number"] == str(number)
            assert int(csv_row["digit_count"]) == json_row["digit_count"] == len(str(number))
            for field in ("natural_position", "first_position", "advance_digits"):
                assert csv_row[field] == json_row[field]
            assert csv_row["is_early"] == str(json_row["is_early"]).lower()
            assert int(csv_row["early_frequency"]) == json_row["early_frequency"]
            natural = int(csv_row["natural_position"])
            first = int(csv_row["first_position"])
            assert first <= natural and natural - first == int(csv_row["advance_digits"])
            starts.append(natural)
            early_count += json_row["is_early"]
        assert next(rows, None) is None
    assert len(starts) == maximum

    prefix = "".join(str(number) for number in range(1, maximum + 1))
    assert starts[-1] + len(str(maximum)) - 1 == len(prefix)
    count = [0] * (maximum + 1)
    first_seen = [0] * (maximum + 1)
    last_position = [0] * (maximum + 1)
    with occurrence_path.open(newline="") as stream:
        rows = csv.DictReader(stream)
        for row in rows:
            number = int(row["number"])
            position = int(row["position"])
            width = len(str(number))
            assert 1 <= number <= maximum
            assert 1 <= position < starts[number - 1]
            assert position > last_position[number]
            assert prefix[position - 1:position - 1 + width] == str(number)
            source_index = bisect.bisect_right(starts, position) - 1
            last_index = bisect.bisect_right(starts, position + width - 1) - 1
            assert int(row["first_source"]) == source_index + 1
            assert int(row["last_source"]) == last_index + 1
            assert int(row["first_source_digit_offset"]) == position - starts[source_index]
            if count[number] == 0:
                first_seen[number] = position
            count[number] += 1
            last_position[number] = position

    for number, record in enumerate(records, 1):
        assert count[number] == record["early_frequency"]
        assert int(record["first_position"]) == (first_seen[number] or starts[number - 1])

    with manifest_path.open() as stream:
        manifest = json.load(stream)
    assert manifest["dataset_schema_version"] == 1
    assert int(manifest["maximum"]) == maximum
    assert manifest["target_records"] == maximum
    assert manifest["early_targets"] == early_count
    assert manifest["summary"]["records"] == maximum
    assert manifest["occurrences"]["records"] == sum(count)
    assert manifest["summary"]["bytes"] == csv_path.stat().st_size
    assert manifest["occurrences"]["bytes"] == occurrence_path.stat().st_size
    print(f"Verified {maximum:,} summaries and {sum(count):,} early occurrences")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--csv", type=Path, required=True)
    parser.add_argument("--json", type=Path, required=True)
    parser.add_argument("--occurrences", type=Path, required=True)
    parser.add_argument("--manifest", type=Path, required=True)
    args = parser.parse_args()
    verify(args.csv, args.json, args.occurrences, args.manifest)
