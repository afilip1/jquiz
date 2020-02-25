#include <string>

namespace term {

enum class Color {
    Red,
    Green,
};

std::string color_to_string(Color color) {
    switch (color) {
        case Color::Red:
            return "\e[0;31m";
        case Color::Green:
            return "\e[0;32m";
    }

    return "\e[m";
}

std::string with_color(Color color, std::string message) {
    return color_to_string(color) + message + "\e[m";
}

std::string red(std::string message) { return with_color(Color::Red, message); }

std::string green(std::string message) {
    return with_color(Color::Green, message);
}

}  // namespace term
