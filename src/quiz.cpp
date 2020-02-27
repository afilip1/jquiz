#include "quiz.hpp"

#include "term.hpp"

namespace jquiz::quiz {

void kanji_to_reading(words::WordList& wordlist) {
    auto question = wordlist.get_random_word();
    auto guess = term::prompt<bsc::String>(question.kanji);

    if (question.readings.contains(guess)) {
        std::cout << term::green("\n正解!\n");
    } else {
        std::cout << term::red("\n違うよ!\n");
    }

    std::cout << "読み方:\n";
    for (auto r : question.readings) {
        std::cout << "  " << r << "\n";
    }

    std::cout << "意味:\n  " << question.meanings << "\n\n";
}

void meaning_to_word(words::WordList& wordlist) {
    auto question = wordlist.get_random_word();
    auto guess = term::prompt<bsc::String>(question.meanings);

    bool correct;
    if (question.kanji != "") {
        correct = guess == question.kanji;
    } else {
        correct = question.readings.contains(guess);
    }

    if (correct) {
        std::cout << term::green("\n正解!\n");
    } else {
        std::cout << term::red("\n違うよ!\n");
    }

    std::cout << question.kanji << "\n";
    std::cout << "読み方:\n";
    for (auto r : question.readings) {
        std::cout << "  " << r << "\n";
    }

    std::cout << "意味:\n  " << question.meanings << "\n\n";
}

}  // namespace jquiz::quiz
