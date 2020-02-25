#include <cstdlib>
#include <ctime>
#include <iostream>

#include "db.hpp"
#include "term.hpp"

int main() {
    std::srand(std::time(nullptr));

    auto dict = DB::get_words_by_tag("n5");

    while (true) {
        auto question = dict.random_word();
        std::cout << question.kanji << "\n";

        std::cout << ">|";
        std::string guess;
        std::cin >> guess;

        if (question.readings.contains(guess)) {
            std::cout << term::green("\n正解!\n");
        } else {
            std::cout << term::red("\n違うよ!\n");
        }

        std::cout << "読み方:\n";
        for (auto r : question.readings) {
            std::cout << "　" << r << "\n";
        }

        std::cout << "意味:\n　" << question.meanings << "\n\n";
    }
}
