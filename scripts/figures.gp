# gnuplot -c scripts/figures.gp DATA_DIR OUTPUT_DIR en|tr svg|png|pdf
data=ARG1; out=ARG2; lang=ARG3; fmt=ARG4; tr=(lang eq 'tr')
set encoding utf8
set datafile separator ','
set border 3
set tics nomirror
set grid ytics lc rgb '#dddddd'
set key outside right
set style line 1 lc rgb '#0072B2' lw 2 pt 7
set style line 2 lc rgb '#D55E00' lw 2 pt 5
set style line 3 lc rgb '#009E73' lw 2 pt 9
set linetype 1 lc rgb '#0072B2'
set linetype 2 lc rgb '#D55E00'
set linetype 3 lc rgb '#009E73'
set linetype 4 lc rgb '#CC79A7'
set linetype 5 lc rgb '#666666'
set linetype 6 lc rgb '#332288'
set linetype 7 lc rgb '#882255'
set linetype 8 lc rgb '#117733'
set linetype 9 lc rgb '#AA4499'
set linetype 10 lc rgb '#44AA99'
if (fmt eq 'svg') set terminal svg size 1200,800 font 'Arial,16' background rgb 'white'
if (fmt eq 'png') set terminal pngcairo size 2400,1600 font 'Arial,24' background rgb 'white'
if (fmt eq 'pdf') set terminal pdfcairo size 12in,8in font 'Arial,16'
file(name)=sprintf('%s/%s-%s.%s',out,name,lang,fmt)
input(name)=sprintf('%s/%s.csv',data,name)
early=tr?'Erkenci':'Early'
punctual=tr?'Zamanında':'Punctual'
digits=tr?'Basamak sayısı':'Digit length'
rate=tr?'Oran':'Proportion'

set output file('early-by-digits')
set title (tr?'Basamağa göre erkenci oranı (7 basamak: yalnız 1000000)':'Early proportions by width (7 digits: only 1000000)')
set xlabel digits; set ylabel rate; set yrange [0:1]; set xrange [0.5:7.5]; set xtics 1
set boxwidth 0.7; set style fill solid 0.85
plot input('groups') using 1:5 with boxes ls 1 title early, '' using 1:5:(sprintf('%d/%d',column(3),column(2))) with labels offset 0,1 notitle

set output file('density')
set title (tr?'1..x için birikimli yoğunluk':'Cumulative density over 1..x')
set xlabel 'x'; set ylabel rate; set logscale x; set xrange [1:1000000]; set xtics autofreq
plot input('density') using 1:4 with lines ls 1 title early, '' using 1:5 with lines ls 2 title punctual
unset logscale x

set output file('frequency')
set title (tr?'Erken görünüm frekansı: grup içindeki pay (E=0 dahil)':'Early frequency: within-width proportions (including E=0)')
set xlabel 'E(m)'; set ylabel rate; set xrange [-0.5:9.5]; set yrange [0:1]; set xtics 1
set key title digits
plot for [d=1:6] input('frequency') using ($1==d?$2:1/0):4 with linespoints lw 2 title sprintf('%d',d)
set key title ''

set output file('relative-ecdf')
set title (tr?'F/P dağılımı: 0.001 aralıklı ECDF; zamanında kütle 1’de':'F/P distribution: ECDF on a 0.001 grid; punctual mass at 1')
set xlabel 'F(m)/P(m)'; set ylabel 'ECDF'; set xrange [0:1]; set xtics autofreq
plot input('ecdf') using 1:3 with lines ls 1 title (tr?'Yalnız erkenci':'Conditional on early'), '' using 1:4 with lines ls 2 title (tr?'Tüm hedefler':'All targets')

set output file('prime-comparison')
set title (tr?'Aynı basamak grubunda asal/bileşik karşılaştırması (1 dışarıda)':'Prime/composite comparison within widths (1 excluded)')
set xlabel digits; set ylabel rate; set xrange [0.5:6.5]; set xtics 1
plot input('groups') using ($1<=6 && $6>0?$1:1/0):8 with linespoints ls 1 title (tr?'Asal':'Prime'), '' using ($1<=6 && $9>0?$1:1/0):11 with linespoints ls 2 title (tr?'Bileşik':'Composite')

set output file('first-position-heatmap')
set title (tr?'İlk konum: 50x50 logaritmik hücre, hedef adedi':'First positions: 50x50 logarithmic bins, target counts')
set xlabel 'log10(m)'; set ylabel 'log10(F(m))'; set xrange [0:6]; set yrange [0:7]; set xtics autofreq
set palette defined (0 '#00204d', 0.25 '#434e6c', 0.5 '#7c7b78', 0.75 '#bcad6c', 1 '#fee838')
set logscale cb; set cbrange [1:1000000]; set cblabel (tr?'Hedef adedi (log renk)':'Targets (log color)')
unset key; unset grid
plot input('heatmap') using 1:2:($3>0?$3:1/0) with image
unset logscale cb; unset colorbox; set key outside right; set grid ytics lc rgb '#dddddd'

set output file('mechanisms')
unset title
set multiplot layout 1,3 title (tr?'Erken görünüm mekanizmaları; payda: 2688255 konum':'Early occurrence mechanisms; denominator: 2688255 positions')
set ylabel (tr?'Konum adedi':'Occurrence count'); set logscale y; set yrange [1:*]; set xrange [-0.5:6.5]; set xtics 1
set boxwidth 0.7
set xlabel (tr?'İlk kaynak basamağı':'First source width')
plot input('mechanisms') using (strcol(1) eq 'source_width'?$2:1/0):3 with boxes ls 1 notitle
set xlabel (tr?'Geçilen sınır':'Crossed boundaries')
plot input('mechanisms') using (strcol(1) eq 'boundaries'?$2:1/0):3 with boxes ls 1 notitle
set xlabel (tr?'En uzun 9 zinciri':'Maximum trailing-9 chain')
plot input('mechanisms') using (strcol(1) eq 'carry'?$2:1/0):3 with boxes ls 1 notitle
unset multiplot; unset logscale y

set output file('block-deviation')
set title (tr?'Sonlu blok sapması; önekler yalnız 10 kuvvetleri değil':'Finite-block deviation; endpoints include non-powers of ten')
set xlabel (tr?'Önek rakam uzunluğu T':'Prefix digit length T'); set ylabel 'delta_q(T)'; set logscale xy; set xrange [1:*]; set yrange [1e-6:1]; set xtics autofreq
set format y '10^{%L}'
plot for [q=1:3] input('blocks') using ($3==q?$2:1/0):5 with points pt (q+4) ps 1.2 title sprintf('q=%d',q)
unset logscale xy
set format y '%g'

set output file('digit-shares')
set title (tr?'Rakam payları: farklı önek uzunlukları':'Digit proportions across prefix endpoints')
set xlabel (tr?'Önek rakam uzunluğu T':'Prefix digit length T'); set ylabel rate; set logscale x; set xrange [1:*]; set yrange [0:1]
plot for [d=0:9] input('digit-shares') using ($3==d?$2:1/0):5 with points pt (d+4) ps 1.2 title sprintf('%d',d), 0.1 with lines dt 2 lc rgb '#000000' title '0.1'
unset logscale x

set output file('ulam-rings')
set title (tr?'Ulam halkaları: dış halka kısmi; keşifsel ölçüm':'Ulam rings: partial outer ring; descriptive measurement')
set xlabel (tr?'Halka r=max(|x|,|y|)':'Ring r=max(|x|,|y|)'); set ylabel rate; set xrange [0:*]; set yrange [0:1]
plot input('rings') using 1:6 with lines ls 1 title early, '' using 1:($4/$2) with lines ls 2 title (tr?'Asal':'Prime')

set output file('ulam-diagonals')
set title (tr?'Basamaklara göre ana köşegenler |x|=|y|; keşifsel':'Main diagonals |x|=|y| by width; descriptive')
set xlabel digits; set ylabel rate; set xrange [0.5:6.5]; set xtics 1
plot input('diagonals') using ($2==1?$1:1/0):7 with linespoints ls 1 title (tr?'Köşegen':'Diagonal'), '' using ($2==0?$1:1/0):7 with linespoints ls 2 title (tr?'Diğer hücreler':'Other cells')

set output file('performance')
unset title
set multiplot layout 1,2 title (tr?'Motor karşılaştırması: çıktı hariç, 3 tekrar, aynı kapsam':'Engine comparison: output excluded, 3 repeats, equal scopes')
set logscale xy; set xrange [10000:1000000]; set yrange [0.1:*]; set xtics autofreq
set xlabel 'N'; set ylabel (tr?'Medyan süre (ms)':'Median time (ms)')
plot input('performance') using (strcol(1) eq 'window'?$2:1/0):3 with linespoints ls 1 title 'window', '' using (strcol(1) eq 'reconstruct'?$2:1/0):3 with linespoints ls 2 title 'reconstruct'
unset logscale y; set yrange [0:*]; set ylabel (tr?'Süreç tepe belleği (MiB)':'Process peak RSS (MiB)')
plot input('performance') using (strcol(1) eq 'window'?$2:1/0):4 with linespoints ls 1 title 'window', '' using (strcol(1) eq 'reconstruct'?$2:1/0):4 with linespoints ls 2 title 'reconstruct'
unset multiplot
unset output
