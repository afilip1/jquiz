#pragma once

#include <iostream>
#include <string>

namespace jquiz::term {

enum class Color {
    Red,
    Green,
};

std::string color_to_string(Color color);
std::string with_color(Color color, std::string message);
std::string red(std::string message);
std::string green(std::string message);

template <class T>
T prompt(std::string message) {
    auto prompt = "|>";
    std::cout << message << "\n" << prompt;

    T input;
    std::cin >> input;

    return input;
}

}  // namespace jquiz::term
