#include <iostream>

#include "db.hpp"
#include "term.hpp"

namespace jquiz {

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <tag>\n";
        return 1;
    }

    auto dict = db::DB::get_words_by_tag(argv[1]);

    while (true) {
        auto question = dict.get_random_word();
        std::cout << question.kanji << "\n";

        std::cout << ">|";
        bsc::String guess;
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

}  // namespace jquiz

int main(int argc, char** argv) { jquiz::main(argc, argv); }
