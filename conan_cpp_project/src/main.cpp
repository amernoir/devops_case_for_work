#include "json_processor.h"
#include <fmt/core.h>
#include <fmt/color.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
#include <cstdlib>


int main(int argc, char* argv[]) {
    try {
        // Настройка логирования
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto logger = std::make_shared<spdlog::logger>("main", console_sink);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::info);
        
        
        JsonProcessor processor;
        
        fmt::print(fg(fmt::color::yellow), "Example 1: Creating sample JSON\n");
        fmt::print("{}\n", fmt::format(fg(fmt::color::gray), "─").repeat(50));
        
        std::string sample_json = processor.create_sample_json();
        spdlog::info("Generated JSON:");
        fmt::print("{}\n\n", sample_json);
        
        fmt::print(fg(fmt::color::yellow), "Example 2: Parsing and analyzing JSON\n");
        fmt::print("{}\n", fmt::format(fg(fmt::color::gray), "─").repeat(50));
        
        processor.parse_and_display(sample_json);
        
        fmt::print(fg(fmt::color::yellow), "\nExample 3: Complex nested JSON\n");
        fmt::print("{}\n", fmt::format(fg(fmt::color::gray), "─").repeat(50));
        
        std::string complex_json = processor.create_complex_json();
        spdlog::info("Complex product catalog:");
        fmt::print("{}\n", complex_json);
        
        fmt::print(fg(fmt::color::yellow), "\nExample 4: Program statistics\n");
        fmt::print("{}\n", fmt::format(fg(fmt::color::gray), "─").repeat(50));
        
        spdlog::info("Program completed successfully");
        fmt::print("\n{} program with Conan dependencies!\n",
                  fmt::styled("Successfully built", 
                             fg(fmt::color::green) | fmt::emphasis::bold));
        
        return EXIT_SUCCESS;
        
    } catch (const std::exception& e) {
        spdlog::critical("Fatal error: {}", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        spdlog::critical("Unknown fatal error");
        return EXIT_FAILURE;
    }
}
