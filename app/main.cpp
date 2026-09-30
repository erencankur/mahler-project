#include "batch.hpp"
#include "cli_util.hpp"
#include "mahler/early.hpp"
#include "mahler/sequence.hpp"

#include <cstdint>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace {

constexpr std::string_view usage =
    "Mahler Project — C++ mathematical CLI\n"
    "Usage:\n"
    "  mahler digit <position> [--format text|json]\n"
    "  mahler inspect <number> [--format text|json]\n"
    "  mahler early <number> [--format text|json]  (1..1000000)\n"
    "  mahler scan --max <number> --output <file> [--format csv|json]\n"
    "              [--occurrences <csv-file>] [--manifest <json-file>]\n"
    "  mahler benchmark --max <number> [--engine window|reconstruct] [--repeat <count>]\n"
    "  mahler analyze --max <number> --output <json-file>\n"
    "  mahler --help\n"
    "  mahler --version\n\n"
    "Positions start at 1; the initial 0. is excluded.\n"
    "inspect reports natural position; early reports first occurrence and early frequency.\n";

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc == 2 && std::string_view(argv[1]) == "--help") {
            std::cout << usage;
        } else if (argc == 2 && std::string_view(argv[1]) == "--version") {
            std::cout << "mahler " << MAHLER_VERSION << '\n';
        } else if (argc >= 2 && std::string_view(argv[1]) == "scan") {
            run_scan_command(argc, argv);
        } else if (argc >= 2 && std::string_view(argv[1]) == "benchmark") {
            run_benchmark_command(argc, argv);
        } else if (argc >= 2 && std::string_view(argv[1]) == "analyze") {
            run_analyze_command(argc, argv);
        } else {
            if (argc != 3 && argc != 5) {
                throw std::invalid_argument("invalid arguments; use mahler --help");
            }
            const std::string_view command = argv[1];
            if (command != "digit" && command != "inspect" && command != "early") {
                throw std::invalid_argument("unknown command; use mahler --help");
            }
            bool json = false;
            if (argc == 5) {
                const std::string_view format = argv[4];
                if (std::string_view(argv[3]) != "--format" ||
                    (format != "text" && format != "json")) {
                    throw std::invalid_argument("expected --format text or --format json");
                }
                json = format == "json";
            }
            const auto value = parse_positive(argv[2]);
            if (command == "digit") {
                const auto location = mahler::locate_digit(value);
                const auto digit = static_cast<unsigned>(mahler::digit_at(value));
                if (json) {
                    std::cout << "{\"schema_version\":1,\"position\":\"" << value
                              << "\",\"digit\":" << digit
                              << ",\"source_number\":\"" << location.source_number
                              << "\",\"digit_offset\":" << location.digit_offset
                              << ",\"digit_count\":" << location.digit_count << "}\n";
                } else {
                    std::cout << digit << '\n';
                }
            } else if (command == "inspect") {
                const auto position = mahler::natural_position(value);
                const auto width = mahler::decimal_digits(value);
                if (json) {
                    std::cout << "{\"schema_version\":1,\"number\":\"" << value
                              << "\",\"digit_count\":" << width
                              << ",\"natural_position\":\"" << position << "\"}\n";
                } else {
                    std::cout << "number: " << value << "\ndigit_count: " << width
                              << "\nnatural_position: " << position << '\n';
                }
            } else {
                if (value > mahler::max_prefix_integer) {
                    throw std::invalid_argument("early target exceeds the 1000000 limit");
                }
                const auto results = mahler::scan_early(value);
                const auto& result = results[static_cast<std::size_t>(value)];
                if (json) {
                    std::cout << "{\"schema_version\":1,\"number\":\"" << value
                              << "\",\"digit_count\":" << mahler::decimal_digits(value)
                              << ",\"natural_position\":\"" << result.natural_position
                              << "\",\"first_position\":\"" << result.first_position
                              << "\",\"is_early\":" << (result.is_early() ? "true" : "false")
                              << ",\"early_frequency\":" << result.early_frequency
                              << ",\"advance_digits\":\"" << result.advance_digits() << "\"}\n";
                } else {
                    std::cout << "number: " << value
                              << "\nnatural_position: " << result.natural_position
                              << "\nfirst_position: " << result.first_position
                              << "\nis_early: " << (result.is_early() ? "true" : "false")
                              << "\nearly_frequency: " << result.early_frequency
                              << "\nadvance_digits: " << result.advance_digits() << '\n';
                }
            }
        }
        if (!std::cout) {
            throw std::runtime_error("could not write output");
        }
        return 0;
    } catch (const std::invalid_argument& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 2;
    } catch (const std::overflow_error& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 3;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
