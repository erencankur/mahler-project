#include "batch.hpp"
#include "cli_util.hpp"
#include "mahler/early.hpp"
#include "mahler/sequence.hpp"
#include "mahler/ulam.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#ifdef MAHLER_HAVE_PNG
#include <png.h>
#endif

namespace {
namespace fs = std::filesystem;
using Count = std::uint64_t;
struct Counts { Count total=0, early=0, prime=0, both=0; };
void count(Counts& c, bool early, bool prime) {
    ++c.total; c.early += early; c.prime += prime; c.both += early && prime;
}
void save(const fs::path& path, const std::string& contents) {
    if (!path.parent_path().empty()) fs::create_directories(path.parent_path());
    auto temporary = path; temporary += ".tmp." + std::to_string(getpid());
    std::ofstream out(temporary, std::ios::binary);
    out << contents; out.close();
    if (!out) { fs::remove(temporary); throw std::runtime_error("could not write " + path.string()); }
    fs::rename(temporary, path);
}
std::ostringstream table(const std::string& header) {
    std::ostringstream s; s << std::setprecision(12) << header << '\n'; return s;
}
double ratio(Count numerator, Count denominator) {
    return denominator ? static_cast<double>(numerator)/static_cast<double>(denominator) : 0;
}
using Color = std::array<unsigned char, 3>;
constexpr Color outside{64,64,64}, neutral{230,230,230}, blue{0,114,178},
                orange{213,94,0}, green{0,158,115};
// Fixed anchors sampled from cividis; linear interpolation, no range rescaling.
constexpr std::array<Color, 5> cividis{{{0,32,77},{67,78,108},{124,123,120},
                                      {188,173,108},{254,232,56}}};
Color continuous(double value) {
    const double t = std::clamp(value, 0.0, 1.0) * 4;
    const auto i = std::min(static_cast<std::size_t>(t), std::size_t{3});
    Color result{};
    for (std::size_t c=0;c<3;++c)
        result[c] = static_cast<unsigned char>(std::lround(cividis[i][c] +
            (t-static_cast<double>(i))*(static_cast<int>(cividis[i+1][c])-cividis[i][c])));
    return result;
}
Color color(const std::string& layer, const mahler::EarlyResult& r, bool prime) {
    const bool early=r.is_early();
    if (layer=="combined") return early ? (prime?green:blue) : (prime?orange:neutral);
    if (layer=="early") return early?blue:neutral;
    if (layer=="prime") return prime?orange:neutral;
    if (layer=="intersection") return early&&prime?green:neutral;
    if (layer=="punctual") return !early?blue:neutral;
    if (layer=="frequency") return early ? continuous(static_cast<double>(r.early_frequency)/9.0) : neutral;
    return early ? continuous(ratio(r.first_position,r.natural_position)) : neutral;
}
std::string hex(Color c) {
    std::ostringstream s; s << '#' << std::hex << std::setfill('0');
    for (auto v:c) s << std::setw(2) << static_cast<unsigned>(v);
    return s.str();
}
void csv_counts(std::ostringstream& s, const Counts& c) {
    s << ',' << c.total << ',' << c.early << ',' << c.prime << ',' << c.both;
}
} // namespace

void run_ulam_command(int argc, char** argv) {
    Count maximum=0, cell=1; std::string layer; fs::path output;
    std::set<std::string> seen;
    if (argc<8 || argc%2) throw std::invalid_argument("expected ulam --max N --layer NAME --output FILE [--cell-size N]");
    for (int i=2;i<argc;i+=2) {
        const std::string key=argv[i];
        if (!seen.insert(key).second) throw std::invalid_argument("duplicate option");
        if (key=="--max") maximum=parse_positive(argv[i+1]);
        else if (key=="--cell-size") cell=parse_positive(argv[i+1]);
        else if (key=="--layer") layer=argv[i+1];
        else if (key=="--output") output=argv[i+1];
        else throw std::invalid_argument("unknown Ulam option");
    }
    const std::set<std::string> layers{"early","prime","intersection","combined","punctual","frequency","relative"};
    if (output.empty() || !layers.contains(layer) || cell>16)
        throw std::invalid_argument("invalid layer, output, or cell size (1..16)");
    const auto side=mahler::ulam_side(maximum);
    const bool svg=output.extension()==".svg";
    if ((!svg && output.extension()!=".png") || (svg && maximum>10000))
        throw std::invalid_argument("use SVG for at most 10000 targets, or PNG");
#ifndef MAHLER_HAVE_PNG
    if (!svg) throw std::invalid_argument("PNG requires MAHLER_ENABLE_GRAPHICS=ON and libpng");
#endif
    const auto results=mahler::scan_early(maximum);
    const auto primes=mahler::prime_flags(maximum);
    const auto pixels=static_cast<unsigned>(side*cell);
    std::vector<unsigned char> rgb(static_cast<std::size_t>(pixels)*pixels*3);
    for (std::size_t i=0;i<rgb.size();++i) rgb[i]=outside[i%3];
    std::ostringstream vector;
    vector << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << pixels
           << "\" height=\"" << pixels << "\" viewBox=\"0 0 " << pixels << ' ' << pixels
           << "\" shape-rendering=\"crispEdges\"><title>Ulam " << layer << ", 1.." << maximum
           << "; right then up; legend in companion JSON</title><rect width=\"100%\" height=\"100%\" fill=\"#404040\"/>\n";
    Counts counts;
    for (Count m=1;m<=maximum;++m) {
        const auto point=mahler::ulam_point(m);
        const auto x=static_cast<unsigned>(static_cast<int>(side/2)+point.x)*static_cast<unsigned>(cell);
        const auto y=static_cast<unsigned>(static_cast<int>(side/2)-point.y)*static_cast<unsigned>(cell);
        const auto c=color(layer,results[static_cast<std::size_t>(m)],primes[static_cast<std::size_t>(m)]);
        count(counts,results[static_cast<std::size_t>(m)].is_early(),primes[static_cast<std::size_t>(m)]);
        if (svg) {
            vector << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << cell
                   << "\" height=\"" << cell << "\" fill=\"" << hex(c) << "\"><title>" << m << "</title></rect>\n";
            if (maximum<=400 && cell>=8)
                vector << "<text x=\"" << x+static_cast<unsigned>(cell)/2.0 << "\" y=\"" << y+static_cast<unsigned>(cell)/2.0
                       << "\" text-anchor=\"middle\" dominant-baseline=\"central\" font-family=\"Arial\" font-size=\""
                       << static_cast<double>(cell)*0.35 << "\" fill=\"#000000\">" << m << "</text>\n";
        }
        else for (unsigned dy=0;dy<cell;++dy) for (unsigned dx=0;dx<cell;++dx)
            for (std::size_t channel=0;channel<3;++channel)
                rgb[(static_cast<std::size_t>(y+dy)*pixels+x+dx)*3+channel]=c[channel];
    }
    if (svg) { vector << "</svg>\n"; save(output,vector.str()); }
#ifdef MAHLER_HAVE_PNG
    else {
        if (!output.parent_path().empty()) fs::create_directories(output.parent_path());
        auto temporary=output; temporary += ".tmp."+std::to_string(getpid());
        png_image image{}; image.version=PNG_IMAGE_VERSION; image.width=pixels; image.height=pixels; image.format=PNG_FORMAT_RGB;
        if (!png_image_write_to_file(&image,temporary.c_str(),0,rgb.data(),0,nullptr)) {
            const std::string message=image.message; png_image_free(&image); fs::remove(temporary);
            throw std::runtime_error("PNG write failed: "+message);
        }
        png_image_free(&image); fs::rename(temporary,output);
    }
#endif
    std::ostringstream manifest;
    manifest << "{\"schema_version\":1,\"maximum\":\"" << maximum << "\",\"layer\":\"" << layer
             << "\",\"side_cells\":" << side << ",\"cell_size\":" << cell << ",\"pixels\":" << pixels
             << ",\"outside_cells\":" << static_cast<Count>(side)*side-maximum
             << ",\"orientation\":\"1 at origin; first right, then up; y up; image row y down\""
             << ",\"engine\":\"numeric-window\",\"targets\":" << counts.total
             << ",\"early\":" << counts.early << ",\"primes\":" << counts.prime << ",\"intersection\":" << counts.both
             << ",\"legend\":{\"neither_or_unselected\":\"#e6e6e6\",\"early\":\"#0072b2\",\"prime\":\"#d55e00\",\"both\":\"#009e73\",\"outside\":\"#404040\"}"
             << ",\"continuous_palette\":[\"#00204d\",\"#434e6c\",\"#7c7b78\",\"#bcad6c\",\"#fee838\"]"
             << ",\"frequency_scale\":[0,9],\"relative_scale\":[0,1],\"punctual_continuous_color\":\"#e6e6e6\"}\n";
    save(output.string()+".json",manifest.str());
    std::cout << "ulam: " << output.string() << " (" << pixels << 'x' << pixels << ")\n";
}

void run_plot_data_command(int argc, char** argv) {
    Count maximum=0; fs::path dir; std::set<std::string> seen;
    if (argc!=6) throw std::invalid_argument("expected plot-data --max N --output-dir DIRECTORY");
    for (int i=2;i<argc;i+=2) {
        const std::string key=argv[i];
        if (!seen.insert(key).second) throw std::invalid_argument("duplicate option");
        if (key=="--max") maximum=parse_positive(argv[i+1]);
        else if (key=="--output-dir") dir=argv[i+1];
        else throw std::invalid_argument("unknown plot-data option");
    }
    if (dir.empty()) throw std::invalid_argument("output directory required");
    (void)mahler::ulam_side(maximum);
    const auto results=mahler::scan_early(maximum); const auto primes=mahler::prime_flags(maximum);
    std::array<Counts,8> digits{}; std::map<unsigned,Counts> rings;
    std::array<Counts,8> diagonal{}, other{};
    std::map<std::pair<unsigned,Count>,Count> frequencies;
    std::vector<double> relative;
    std::array<std::array<Count,50>,50> heat{};
    std::set<Count> checkpoints{1,9,10,99,100,999,1000,9999,10000,99999,100000,999999,maximum};
    for (unsigned i=0;i<=120;++i) checkpoints.insert(std::min(maximum,static_cast<Count>(std::pow(10.0,6.0*i/120))));
    auto density=table("number,early,punctual,early_rate,punctual_rate");
    auto mechanisms=table("kind,value,occurrences");
    std::map<Count,Count> source_width,boundaries,carries;
    Count early_count=0,occurrence_count=0;
    for (Count m=1;m<=maximum;++m) {
        const auto& r=results[static_cast<std::size_t>(m)];
        const bool prime=primes[static_cast<std::size_t>(m)],early=r.is_early();
        early_count+=early; const auto d=mahler::decimal_digits(m);
        count(digits[d],early,prime); ++frequencies[{d,r.early_frequency}];
        const auto p=mahler::ulam_point(m); const auto ring=static_cast<unsigned>(std::max(std::abs(p.x),std::abs(p.y)));
        count(rings[ring],early,prime); count(std::abs(p.x)==std::abs(p.y)?diagonal[d]:other[d],early,prime);
        if (early) relative.push_back(ratio(r.first_position,r.natural_position));
        const auto xb=std::min(49u,static_cast<unsigned>(std::log10(static_cast<double>(m))*50/6));
        const auto yb=std::min(49u,static_cast<unsigned>(std::log10(static_cast<double>(r.first_position))*50/7));
        ++heat[xb][yb];
        if (checkpoints.contains(m)) density << m << ',' << early_count << ',' << m-early_count << ','
                                              << ratio(early_count,m) << ',' << ratio(m-early_count,m) << '\n';
        const auto positions=mahler::reconstruct_early_positions(m);
        if (positions.size()!=r.early_frequency || (!positions.empty() && positions.front()!=r.first_position))
            throw std::runtime_error("plot occurrence reconstruction disagrees with window scan");
        for (auto position:positions) {
            const auto a=mahler::locate_digit(position).source_number;
            const auto b=mahler::locate_digit(position+d-1).source_number;
            ++source_width[mahler::decimal_digits(a)]; ++boundaries[b-a]; unsigned carry=0;
            for (auto source=a;source<b;++source) {
                unsigned nines=0; for (auto n=source;n%10==9;n/=10) ++nines;
                carry=std::max(carry,nines);
            }
            ++carries[carry]; ++occurrence_count;
        }
    }
    auto group=table("digits,targets,early,punctual,early_rate,prime_targets,prime_early,prime_rate,composite_targets,composite_early,composite_rate,complete");
    auto hist=table("digits,frequency,targets,within_group_rate");
    for (unsigned d=1;d<8;++d) if (digits[d].total) {
        const auto& c=digits[d]; const Count composite=c.total-c.prime-(d==1?1:0);
        group << d << ',' << c.total << ',' << c.early << ',' << c.total-c.early << ',' << ratio(c.early,c.total)
              << ',' << c.prime << ',' << c.both << ',' << ratio(c.both,c.prime)
              << ',' << composite << ',' << c.early-c.both << ',' << ratio(c.early-c.both,composite)
              << ',' << (c.total==9*static_cast<Count>(std::pow(10,d-1))) << '\n';
        for (Count f=0;f<=9;++f) hist << d << ',' << f << ',' << frequencies[{d,f}] << ',' << ratio(frequencies[{d,f}],c.total) << '\n';
    }
    std::sort(relative.begin(),relative.end());
    auto ecdf=table("ratio,early_at_or_below,early_conditional_ecdf,all_target_ecdf,punctual_mass");
    for (unsigned i=0;i<=1000;++i) {
        const double x=static_cast<double>(i)/1000;
        const Count n=static_cast<Count>(std::upper_bound(relative.begin(),relative.end(),x)-relative.begin());
        ecdf << x << ',' << n << ',' << ratio(n,early_count) << ',' << ratio(n+(i==1000?maximum-early_count:0),maximum)
             << ',' << ratio(maximum-early_count,maximum) << '\n';
    }
    auto map=table("log10_number,log10_first,targets");
    for (unsigned y=0;y<50;++y) {
        for (unsigned x=0;x<50;++x) map << (x+0.5)*6/50 << ',' << (y+0.5)*7/50 << ',' << heat[x][y] << '\n';
        map << '\n';
    }
    for (const auto& [kind,values]:std::array<std::pair<std::string,std::map<Count,Count>>,3>{{{"source_width",source_width},{"boundaries",boundaries},{"carry",carries}}})
        for (const auto& [value,n]:values) mechanisms << kind << ',' << value << ',' << n << '\n';
    auto geometry=table("ring,targets,early,primes,intersection,early_rate");
    for (const auto& [ring,c]:rings) { geometry << ring; csv_counts(geometry,c); geometry << ',' << ratio(c.early,c.total) << '\n'; }
    auto diag=table("digits,diagonal,targets,early,primes,intersection,early_rate");
    for (unsigned d=1;d<8;++d) for (unsigned is_diag=0;is_diag<2;++is_diag) {
        const auto& c=is_diag?diagonal[d]:other[d]; if (!c.total) continue;
        diag << d << ',' << is_diag; csv_counts(diag,c); diag << ',' << ratio(c.early,c.total) << '\n';
    }
    auto blocks=table("last_source,prefix_digits,q,windows,delta");
    auto shares=table("last_source,prefix_digits,digit,count,share");
    std::set<Count> endpoints{1,9,99,999,9999,12345,99999,234567,543210,999999,maximum};
    for (auto last:endpoints) if (last<=maximum) {
        const auto prefix=mahler::make_prefix(last);
        for (unsigned q=1;q<=3;++q) if (prefix.size()>=q) {
            const auto size=static_cast<unsigned>(std::pow(10,q)); std::vector<Count> counts(size);
            unsigned value=0;
            for (std::size_t i=0;i<prefix.size();++i) {
                value=(value*10+static_cast<unsigned>(prefix[i]-'0'))%size;
                if (i+1>=q) ++counts[value];
            }
            const auto windows=prefix.size()-q+1; double delta=0;
            for (auto n:counts) delta=std::max(delta,std::abs(ratio(n,windows)-1.0/size));
            blocks << last << ',' << prefix.size() << ',' << q << ',' << windows << ',' << delta << '\n';
            if (q==1) for (unsigned digit=0;digit<10;++digit)
                shares << last << ',' << prefix.size() << ',' << digit << ',' << counts[digit] << ',' << ratio(counts[digit],windows) << '\n';
        }
    }
    const std::array<std::pair<std::string,std::string>,10> files{{
        {"groups.csv",group.str()},{"density.csv",density.str()},{"frequency.csv",hist.str()},
        {"ecdf.csv",ecdf.str()},{"heatmap.csv",map.str()},{"mechanisms.csv",mechanisms.str()},
        {"rings.csv",geometry.str()},{"diagonals.csv",diag.str()},{"blocks.csv",blocks.str()},{"digit-shares.csv",shares.str()}}};
    for (const auto& [name,contents]:files) save(dir/name,contents);
    std::ostringstream manifest;
    manifest << "{\"schema_version\":1,\"maximum\":\"" << maximum << "\",\"engine\":\"numeric-window; reconstruction for occurrence mechanisms\""
             << ",\"early_targets\":" << early_count << ",\"punctual_targets\":" << maximum-early_count
             << ",\"early_occurrences\":" << occurrence_count
             << ",\"ecdf_step\":0.001,\"heatmap_bins\":[50,50],\"heatmap_log10_ranges\":[[0,6],[0,7]],\"frequency_scale\":[0,9]}\n";
    save(dir/"manifest.json",manifest.str());
    std::cout << "plot data: " << dir.string() << " (" << maximum << " targets)\n";
}
