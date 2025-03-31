#pragma once
#include <iostream>
#include <format>

template <typename... Args>
void logD(std::format_string<Args...> s, Args&&... args) {
    std::printf("DEBUG: %s\n", std::format(s, std::forward<Args>(args)...).c_str());
}

template <typename... Args>
void logW(std::format_string<Args...> s, Args&&... args) {
    std::printf("WARNING: %s\n", std::format(s, std::forward<Args>(args)...).c_str());
}

template <typename... Args>
void logE(std::format_string<Args...> s, Args&&... args) {
    std::printf("ERROR: %s\n", std::format(s, std::forward<Args>(args)...).c_str());
}
