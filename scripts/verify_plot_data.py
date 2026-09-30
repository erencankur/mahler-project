#!/usr/bin/env python3
"""Optional standard-library audit: aggregates versus the independent export file."""
import csv
import json
import math
import sys
from collections import Counter, defaultdict
from pathlib import Path

data, summary = map(Path, sys.argv[1:3])
def rows(name):
    with (data / name).open() as source:
        return list(csv.DictReader(line for line in source if line.strip()))

groups = defaultdict(Counter)
frequency = Counter()
heat = Counter()
early_ratios = []
total_frequency = 0
with summary.open() as source:
    for row in csv.DictReader(source):
        d = int(row['digit_count'])
        m, first, natural = (int(row[k]) for k in ('number','first_position','natural_position'))
        early = row['is_early'] == 'true'
        groups[d]['targets'] += 1
        groups[d]['early'] += early
        frequency[d, int(row['early_frequency'])] += 1
        total_frequency += int(row['early_frequency'])
        heat[min(49, int(math.log10(m)*50/6)), min(49,int(math.log10(first)*50/7))] += 1
        if early:
            early_ratios.append(first/natural)
manifest = json.loads((data/'manifest.json').read_text())
assert sum(g['targets'] for g in groups.values()) == int(manifest['maximum'])
assert sum(g['early'] for g in groups.values()) == manifest['early_targets']
assert total_frequency == manifest['early_occurrences']
for row in rows('groups.csv'):
    g = groups[int(row['digits'])]
    assert int(row['targets']) == g['targets'] and int(row['early']) == g['early']
    assert int(row['prime_targets']) + int(row['composite_targets']) + (int(row['digits'])==1) == g['targets']
    assert int(row['prime_early']) + int(row['composite_early']) == g['early']
for row in rows('frequency.csv'):
    assert int(row['targets']) == frequency[int(row['digits']),int(row['frequency'])]
for row in rows('heatmap.csv'):
    x,y = (int(round(float(row[k])*50/r-0.5)) for k,r in [('log10_number',6),('log10_first',7)])
    assert int(row['targets']) == heat[x,y]
for kind in ('source_width','boundaries','carry'):
    assert sum(int(r['occurrences']) for r in rows('mechanisms.csv') if r['kind']==kind) == total_frequency
for name in ('rings.csv','diagonals.csv'):
    assert sum(int(r['targets']) for r in rows(name)) == int(manifest['maximum'])
    assert sum(int(r['early']) for r in rows(name)) == manifest['early_targets']
import bisect
early_ratios.sort()
for row in rows('ecdf.csv'):
    assert int(row['early_at_or_below']) == bisect.bisect_right(early_ratios,float(row['ratio']))
digit_counts = defaultdict(int)
for row in rows('digit-shares.csv'):
    digit_counts[int(row['last_source'])] += int(row['count'])
    assert math.isclose(float(row['share']),int(row['count'])/int(row['prefix_digits']),abs_tol=1e-11)
for row in rows('blocks.csv'):
    assert digit_counts[int(row['last_source'])] == int(row['prefix_digits'])
    assert int(row['windows']) == int(row['prefix_digits'])-int(row['q'])+1
print('All plot aggregates, denominators, bins, and ECDF values match the full summary.')
