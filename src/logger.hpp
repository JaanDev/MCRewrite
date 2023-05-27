#pragma once
#include <iostream>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/color.h>

template <typename... Args>
void logD(Args... args) {
    std::cout << fmt::format("[{:%H:%M:%S}] DEBUG: ", fmt::localtime(std::time(nullptr))) << fmt::format(args...) << std::endl;
}

template <typename... Args>
void logW(Args... args) {
    std::cout << fmt::format("[{:%H:%M:%S}] WARN: ", fmt::localtime(std::time(nullptr)))
              << fmt::format(fg(fmt::terminal_color::yellow), args...) << std::endl;
}

template <typename... Args>
void logE(Args... args) {
    std::cout << fmt::format("[{:%H:%M:%S}] ERROR: ", fmt::localtime(std::time(nullptr)))
              << fmt::format(fg(fmt::terminal_color::bright_red) | fmt::emphasis::bold, args...) << std::endl;
}