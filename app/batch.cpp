#include "batch.hpp"
#include "cli_util.hpp"
#include "mahler/early.hpp"
#include "mahler/sequence.hpp"
#include "mahler/ulam.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <vector>

#ifdef __APPLE__
#include <sys/sysctl.h>
#endif

namespace {

namespace fs = std::filesystem;
using Clock = std::chrono::steady_clock;
constexpr std::uint64_t fnv_offset = 14'695'981'039'346'656'037ULL;
constexpr std::uint64_t fnv_prime = 1'099'511'628'211ULL;

struct FileInfo {
    fs::path path;
    std::uint64_t bytes;
    std::string checksum;
    std::uint64_t records;
};

double milliseconds(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

std::string json_quote(std::string_view input) {
    std::ostringstream out;
    out << '"';
    constexpr char hex[] = "0123456789abcdef";
    for (char byte : input) {
        const auto ch = static_cast<unsigned char>(byte);
        switch (ch) {
        case '"': out << "\\\""; break;
        case '\\': out << "\\\\"; break;
        case '\n': out << "\\n"; break;
        case '\r': out << "\\r"; break;
        case '\t': out << "\\t"; break;
        default:
            if (ch < 0x20) {
                out << "\\u00" << hex[ch >> 4] << hex[ch & 15];
            } else {
                out << static_cast<char>(ch);
            }
        }
    }
    out << '"';
    return out.str();
}

std::string hex_hash(std::uint64_t value) {
    std::ostringstream out;
    out << std::hex << std::setw(16) << std::setfill('0') << value;
    return out.str();
}

void mix_number(std::uint64_t& hash, std::uint64_t number) {
    for (unsigned shift = 0; shift < 64; shift += 8) {
        hash ^= (number >> shift) & 0xffU;
        hash *= fnv_prime;
    }
}

FileInfo inspect_file(const fs::path& path, std::uint64_t records) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("could not read output file: " + path.string());
    }
    std::array<char, 65'536> block{};
    std::uint64_t hash = fnv_offset;
    std::uint64_t bytes = 0;
    while (input) {
        input.read(block.data(), static_cast<std::streamsize>(block.size()));
        const auto read = input.gcount();
        for (std::streamsize i = 0; i < read; ++i) {
            hash ^= static_cast<unsigned char>(block[static_cast<std::size_t>(i)]);
            hash *= fnv_prime;
        }
        bytes += static_cast<std::uint64_t>(read);
    }
    if (!input.eof()) {
        throw std::runtime_error("could not finish reading output file: " + path.string());
    }
    return {path, bytes, hex_hash(hash), records};
}

template<class Writer>
void write_atomic(const fs::path& target, Writer writer) {
    if (!target.parent_path().empty()) {
        fs::create_directories(target.parent_path());
    }
    const fs::path temporary = target.string() + ".tmp." + std::to_string(getpid());
    try {
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output) {
            throw std::runtime_error("could not open output file: " + temporary.string());
        }
        writer(output);
        output.flush();
        if (!output) {
            throw std::runtime_error("could not write output file: " + temporary.string());
        }
        output.close();
        if (!output) {
            throw std::runtime_error("could not close output file: " + temporary.string());
        }
        fs::rename(temporary, target);
    } catch (...) {
        std::error_code ignored;
        fs::remove(temporary, ignored);
        throw;
    }
}

std::uint64_t peak_rss_bytes() {
    rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        throw std::runtime_error("could not read peak resident memory");
    }
#ifdef __APPLE__
    return static_cast<std::uint64_t>(usage.ru_maxrss);
#else
    return static_cast<std::uint64_t>(usage.ru_maxrss) * 1024;
#endif
}

std::string utc_timestamp() {
    const std::time_t now = std::time(nullptr);
    std::tm utc{};
    if (gmtime_r(&now, &utc) == nullptr) {
        throw std::runtime_error("could not format UTC timestamp");
    }
    std::ostringstream out;
    out << std::put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
    return out.str();
}

std::string hardware_model() {
#ifdef __APPLE__
    std::array<char, 256> buffer{};
    std::size_t length = buffer.size();
    if (sysctlbyname("hw.model", buffer.data(), &length, nullptr, 0) == 0 && length > 0) {
        const auto end = std::find(buffer.begin(), buffer.end(), '\0');
        return std::string(buffer.begin(), end);
    }
#endif
    return "unavailable";
}

void validate_maximum(std::uint64_t maximum) {
    if (maximum > mahler::max_prefix_integer) {
        throw std::invalid_argument("maximum exceeds the 1000000 limit");
    }
}

std::string normalized(const fs::path& path) {
    return fs::absolute(path).lexically_normal().string();
}

void write_summary(std::ostream& out, std::uint64_t maximum,
                   const std::vector<mahler::EarlyResult>& results, bool json) {
    if (json) {
        out << "{\"schema_version\":1,\"maximum\":\"" << maximum
            << "\",\"record_count\":" << maximum << ",\"records\":[\n";
    } else {
        out << "number,digit_count,natural_position,first_position,is_early,early_frequency,advance_digits\n";
    }
    for (std::uint64_t number = 1; number <= maximum; ++number) {
        const auto& record = results[static_cast<std::size_t>(number)];
        if (json) {
            if (number != 1) {
                out << ",\n";
            }
            out << "{\"number\":\"" << number << "\",\"digit_count\":"
                << mahler::decimal_digits(number)
                << ",\"natural_position\":\"" << record.natural_position
                << "\",\"first_position\":\"" << record.first_position
                << "\",\"is_early\":" << (record.is_early() ? "true" : "false")
                << ",\"early_frequency\":" << record.early_frequency
                << ",\"advance_digits\":\"" << record.advance_digits() << "\"}";
        } else {
            out << number << ',' << mahler::decimal_digits(number) << ','
                << record.natural_position << ',' << record.first_position << ','
                << (record.is_early() ? "true" : "false") << ','
                << record.early_frequency << ',' << record.advance_digits() << '\n';
        }
    }
    if (json) {
        out << "\n]}\n";
    }
}

std::uint64_t write_occurrences(std::ostream& out, std::uint64_t maximum,
                                const std::vector<mahler::EarlyResult>& results) {
    out << "number,position,first_source,last_source,first_source_digit_offset\n";
    std::uint64_t count = 0;
    for (std::uint64_t number = 1; number <= maximum; ++number) {
        const auto positions = mahler::reconstruct_early_positions(number);
        const auto& record = results[static_cast<std::size_t>(number)];
        if (positions.size() != record.early_frequency ||
            (!positions.empty() && positions.front() != record.first_position)) {
            throw std::runtime_error("occurrence engines disagree at target " + std::to_string(number));
        }
        for (const auto position : positions) {
            const auto first = mahler::locate_digit(position);
            const auto last = mahler::locate_digit(position + mahler::decimal_digits(number) - 1);
            out << number << ',' << position << ',' << first.source_number << ','
                << last.source_number << ',' << first.digit_offset << '\n';
            ++count;
        }
    }
    return count;
}

std::uint64_t write_legacy_rows(std::ostream& out, std::uint64_t maximum,
                                const std::vector<mahler::EarlyResult>& results,
                                const std::vector<bool>* primes) {
    // The historical PDFs are target lists annotated with frequency.  This
    // normalized form keeps that information and adds positions for auditing.
    out << "number,digit_count,first_position,natural_position,early_frequency,advance_digits\n";
    std::uint64_t count = 0;
    for (std::uint64_t number = 1; number <= maximum; ++number) {
        const auto& record = results[static_cast<std::size_t>(number)];
        if (!record.is_early() || (primes && !(*primes)[static_cast<std::size_t>(number)])) {
            continue;
        }
        out << number << ',' << mahler::decimal_digits(number) << ','
            << record.first_position << ',' << record.natural_position << ','
            << record.early_frequency << ',' << record.advance_digits() << '\n';
        ++count;
    }
    return count;
}

void write_file_entry(std::ostream& out, const FileInfo& file) {
    out << "{\"path\":" << json_quote(file.path.string())
        << ",\"bytes\":" << file.bytes
        << ",\"records\":" << file.records
        << ",\"fnv1a64\":\"" << file.checksum << "\"}";
}

std::vector<mahler::EarlyResult> reconstruct_all(std::uint64_t maximum) {
    std::vector<mahler::EarlyResult> results(static_cast<std::size_t>(maximum + 1));
    for (std::uint64_t number = 1; number <= maximum; ++number) {
        const auto natural = mahler::natural_position(number);
        const auto positions = mahler::reconstruct_early_positions(number);
        results[static_cast<std::size_t>(number)] = {
            natural, positions.empty() ? natural : positions.front(),
            static_cast<std::uint64_t>(positions.size())};
    }
    return results;
}

std::string result_checksum(const std::vector<mahler::EarlyResult>& results) {
    std::uint64_t hash = fnv_offset;
    for (std::size_t i = 1; i < results.size(); ++i) {
        mix_number(hash, static_cast<std::uint64_t>(i));
        mix_number(hash, results[i].natural_position);
        mix_number(hash, results[i].first_position);
        mix_number(hash, results[i].early_frequency);
    }
    return hex_hash(hash);
}

} // namespace

void run_scan_command(int argc, char** argv) {
    std::optional<std::uint64_t> maximum;
    std::optional<fs::path> output;
    std::optional<fs::path> occurrences;
    std::optional<fs::path> manifest;
    std::string format = "csv";
    bool format_seen = false;
    if (argc < 6 || argc % 2 != 0) {
        throw std::invalid_argument("scan requires --max and --output with option values");
    }
    for (int i = 2; i < argc; i += 2) {
        const std::string_view option = argv[i];
        const std::string value = argv[i + 1];
        if (option == "--max" && !maximum) {
            maximum = parse_positive(value);
            validate_maximum(*maximum);
        } else if (option == "--output" && !output && !value.empty()) {
            output = value;
        } else if (option == "--occurrences" && !occurrences && !value.empty()) {
            occurrences = value;
        } else if (option == "--manifest" && !manifest && !value.empty()) {
            manifest = value;
        } else if (option == "--format" && !format_seen && (value == "csv" || value == "json")) {
            format = value;
            format_seen = true;
        } else {
            throw std::invalid_argument("unknown, duplicate, or invalid scan option");
        }
    }
    if (!maximum || !output) {
        throw std::invalid_argument("scan requires --max and --output");
    }
    if (!manifest) {
        manifest = output->string() + ".manifest.json";
    }
    std::vector<std::string> destinations = {normalized(*output), normalized(*manifest)};
    if (occurrences) {
        destinations.push_back(normalized(*occurrences));
    }
    std::sort(destinations.begin(), destinations.end());
    if (std::adjacent_find(destinations.begin(), destinations.end()) != destinations.end()) {
        throw std::invalid_argument("output, occurrences, and manifest paths must differ");
    }

    const auto begin = Clock::now();
    const auto results = mahler::scan_early(*maximum);
    const auto scanned = Clock::now();
    std::uint64_t early_count = 0;
    for (std::uint64_t number = 1; number <= *maximum; ++number) {
        early_count += results[static_cast<std::size_t>(number)].is_early() ? 1ULL : 0ULL;
    }
    write_atomic(*output, [&](std::ostream& stream) {
        write_summary(stream, *maximum, results, format == "json");
    });
    const auto summary_written = Clock::now();
    std::optional<std::uint64_t> occurrence_count;
    if (occurrences) {
        write_atomic(*occurrences, [&](std::ostream& stream) {
            occurrence_count = write_occurrences(stream, *maximum, results);
        });
    }
    const auto occurrences_written = Clock::now();
    const auto summary_info = inspect_file(*output, *maximum);
    std::optional<FileInfo> occurrence_info;
    if (occurrences) {
        occurrence_info = inspect_file(*occurrences, *occurrence_count);
    }
    const auto checked = Clock::now();
    utsname system{};
    if (uname(&system) != 0) {
        throw std::runtime_error("could not read operating-system information");
    }
    const auto memory = peak_rss_bytes();
    write_atomic(*manifest, [&](std::ostream& stream) {
        stream << "{\n  \"manifest_schema_version\":1,\n"
               << "  \"dataset_schema_version\":1,\n"
               << "  \"generated_utc\":" << json_quote(utc_timestamp()) << ",\n"
               << "  \"application_version\":" << json_quote(MAHLER_VERSION) << ",\n"
               << "  \"compiler\":" << json_quote(std::string(MAHLER_COMPILER_ID) + " " + MAHLER_COMPILER_VERSION) << ",\n"
               << "  \"build_type\":" << json_quote(MAHLER_BUILD_TYPE) << ",\n"
               << "  \"sanitizers_enabled\":" << (MAHLER_SANITIZERS_ENABLED ? "true" : "false") << ",\n"
               << "  \"system\":{\"name\":" << json_quote(system.sysname)
               << ",\"release\":" << json_quote(system.release)
               << ",\"machine\":" << json_quote(system.machine)
               << ",\"model\":" << json_quote(hardware_model()) << "},\n"
               << "  \"source\":\"concatenated decimal integers 1..maximum\",\n"
               << "  \"base\":10,\n"
               << "  \"positions\":\"one-based, initial 0. excluded\",\n"
               << "  \"maximum\":\"" << *maximum << "\",\n"
               << "  \"summary_format\":" << json_quote(format) << ",\n"
               << "  \"summary_engine\":\"window\",\n"
               << "  \"occurrence_engine\":"
               << (occurrences ? "\"reconstruct\"" : "null") << ",\n"
               << "  \"occurrence_engine_status\":"
               << (occurrences ? "\"experimental\"" : "null") << ",\n"
               << "  \"target_records\":" << *maximum << ",\n"
               << "  \"early_targets\":" << early_count << ",\n"
               << "  \"checksum_algorithm\":\"fnv1a64\",\n"
               << "  \"summary\":";
        write_file_entry(stream, summary_info);
        if (occurrence_info) {
            stream << ",\n  \"occurrences\":";
            write_file_entry(stream, *occurrence_info);
        }
        stream << ",\n  \"timing_ms\":{\"scan\":" << std::fixed << std::setprecision(3)
               << milliseconds(begin, scanned)
               << ",\"summary_write\":" << milliseconds(scanned, summary_written)
               << ",\"occurrence_write\":" << milliseconds(summary_written, occurrences_written)
               << ",\"checksum_read\":" << milliseconds(occurrences_written, checked)
               << ",\"through_checks\":" << milliseconds(begin, checked) << "},\n"
               << "  \"peak_rss_bytes\":" << memory << "\n}\n";
    });
    std::cout << "summary: " << output->string() << '\n'
              << "manifest: " << manifest->string() << '\n'
              << "targets: " << *maximum << '\n'
              << "early_targets: " << early_count << '\n';
    if (occurrences) {
        std::cout << "occurrences: " << occurrences->string() << '\n';
    }
}

void run_legacy_csv_command(int argc, char** argv) {
    std::optional<std::uint64_t> maximum;
    std::optional<fs::path> output_dir;
    if (argc != 6) {
        throw std::invalid_argument("legacy-csv requires --max and --output-dir");
    }
    for (int i = 2; i < argc; i += 2) {
        const std::string_view option = argv[i];
        const std::string value = argv[i + 1];
        if (option == "--max" && !maximum) {
            maximum = parse_positive(value);
            validate_maximum(*maximum);
        } else if (option == "--output-dir" && !output_dir && !value.empty()) {
            output_dir = value;
        } else {
            throw std::invalid_argument("unknown, duplicate, or invalid legacy-csv option");
        }
    }
    if (!maximum || !output_dir) {
        throw std::invalid_argument("legacy-csv requires --max and --output-dir");
    }

    const auto all_path = *output_dir / "early-birds.csv";
    const auto prime_path = *output_dir / "prime-early-birds.csv";
    const auto manifest_path = *output_dir / "manifest.json";
    const auto results = mahler::scan_early(*maximum);
    const auto primes = mahler::prime_flags(*maximum);
    std::uint64_t all_count = 0;
    std::uint64_t prime_count = 0;
    write_atomic(all_path, [&](std::ostream& stream) {
        all_count = write_legacy_rows(stream, *maximum, results, nullptr);
    });
    write_atomic(prime_path, [&](std::ostream& stream) {
        prime_count = write_legacy_rows(stream, *maximum, results, &primes);
    });
    const auto all_info = inspect_file(all_path, all_count);
    const auto prime_info = inspect_file(prime_path, prime_count);
    write_atomic(manifest_path, [&](std::ostream& stream) {
        stream << "{\n  \"schema_version\":1,\n"
               << "  \"kind\":\"legacy-compatible-early-bird-lists\",\n"
               << "  \"source\":\"concatenated decimal integers 1..maximum\",\n"
               << "  \"base\":10,\n"
               << "  \"positions\":\"one-based, initial 0. excluded\",\n"
               << "  \"maximum\":\"" << *maximum << "\",\n"
               << "  \"csv_schema\":[\"number\",\"digit_count\",\"first_position\",\"natural_position\",\"early_frequency\",\"advance_digits\"],\n"
               << "  \"checksum_algorithm\":\"fnv1a64\",\n"
               << "  \"early_birds\":";
        write_file_entry(stream, all_info);
        stream << ",\n  \"prime_early_birds\":";
        write_file_entry(stream, prime_info);
        stream << "\n}\n";
    });
    std::cout << "early_birds: " << all_path.string() << '\n'
              << "prime_early_birds: " << prime_path.string() << '\n'
              << "manifest: " << manifest_path.string() << '\n'
              << "early_targets: " << all_count << '\n'
              << "prime_early_targets: " << prime_count << '\n';
}

void run_benchmark_command(int argc, char** argv) {
    std::optional<std::uint64_t> maximum;
    std::string engine = "window";
    bool engine_seen = false;
    std::uint64_t repeat = 3;
    bool repeat_seen = false;
    if (argc < 4 || argc % 2 != 0) {
        throw std::invalid_argument("benchmark requires --max with option values");
    }
    for (int i = 2; i < argc; i += 2) {
        const std::string_view option = argv[i];
        const std::string_view value = argv[i + 1];
        if (option == "--max" && !maximum) {
            maximum = parse_positive(value);
            validate_maximum(*maximum);
        } else if (option == "--engine" && !engine_seen &&
                   (value == "window" || value == "reconstruct")) {
            engine = value;
            engine_seen = true;
        } else if (option == "--repeat" && !repeat_seen) {
            repeat = parse_positive(value);
            repeat_seen = true;
            if (repeat > 20) {
                throw std::invalid_argument("benchmark repeat exceeds the 20 limit");
            }
        } else {
            throw std::invalid_argument("unknown, duplicate, or invalid benchmark option");
        }
    }
    if (!maximum) {
        throw std::invalid_argument("benchmark requires --max");
    }
    std::vector<double> samples;
    std::string checksum;
    for (std::uint64_t run = 0; run < repeat; ++run) {
        const auto start = Clock::now();
        const auto results = engine == "window" ? mahler::scan_early(*maximum)
                                                 : reconstruct_all(*maximum);
        const auto end = Clock::now();
        const auto current = result_checksum(results);
        if (!checksum.empty() && current != checksum) {
            throw std::runtime_error("benchmark runs produced different results");
        }
        checksum = current;
        samples.push_back(milliseconds(start, end));
    }
    auto sorted = samples;
    std::sort(sorted.begin(), sorted.end());
    const auto median = sorted.size() % 2 == 0
        ? (sorted[sorted.size() / 2 - 1] + sorted[sorted.size() / 2]) / 2
        : sorted[sorted.size() / 2];
    std::cout << "{\"schema_version\":1,\"maximum\":\"" << *maximum
              << "\",\"engine\":" << json_quote(engine)
              << ",\"engine_status\":"
              << json_quote(engine == "window" ? "baseline" : "experimental")
              << ",\"repeat\":" << repeat
              << ",\"core_ms\":[" << std::fixed << std::setprecision(3);
    for (std::size_t i = 0; i < samples.size(); ++i) {
        if (i) {
            std::cout << ',';
        }
        std::cout << samples[i];
    }
    std::cout << "],\"median_core_ms\":" << median
              << ",\"peak_rss_bytes\":" << peak_rss_bytes()
              << ",\"result_fnv1a64\":\"" << checksum << "\"}\n";
}
