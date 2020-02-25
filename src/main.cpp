#include <sqlite3.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

#include "bsc/map.hpp"
#include "bsc/string.hpp"
#include "bsc/vec.hpp"
#include "term.hpp"

struct Word {
    bsc::String kanji;
    bsc::Vec<bsc::String> readings;
    bsc::String meanings;
};

class Dictionary {
   public:
    Dictionary() {}
    Dictionary(bsc::Vec<Word> words) : dict{words} {}

    virtual Word random_word() {
        //
        return dict[std::rand() % dict.size()];
    }

   protected:
    bsc::Vec<Word> dict;
};

class KanjiQuizDictionary : public Dictionary {
   public:
    KanjiQuizDictionary(bsc::Vec<Word> words) {
        dict = words.iter()
                   .filter([](auto word) { return word.kanji != ""; })
                   .collect();
    }
};

Dictionary load_words(std::string tag) {
    sqlite3* db;
    auto exit = sqlite3_open("./data.db", &db);

    if (exit) {
        std::cerr << "Error opening DB: " << sqlite3_errmsg(db) << "\n";
    }

    auto query =
        "SELECT kanji, readings, meanings FROM words "
        "WHERE tags LIKE \"" +
        tag + "\";";

    bsc::Vec<Word> words;
    auto callback = [](void* data, int argc, char** argv, char** column) {
        auto words = static_cast<bsc::Vec<Word>*>(data);
        std::string kanji = argv[0] ? std::string(argv[0]) : "";
        bsc::Vec<bsc::String> readings = bsc::String(argv[1]).split_on(',');
        std::string meanings = argv[2];
        auto word = Word{kanji, readings, meanings};
        words->push_back(word);
        return 0;
    };

    char* errmsg = nullptr;
    sqlite3_exec(db, query.c_str(), callback, &words, &errmsg);

    if (errmsg) {
        std::cout << "err: " << errmsg << "\n";
    }

    sqlite3_close(db);

    return KanjiQuizDictionary(words);
}

int main() {
    std::srand(std::time(nullptr));

    auto prompt = ">|";
    auto dict = load_words("n5");

    while (true) {
        auto question = dict.random_word();
        std::cout << question.kanji << "\n" << prompt;

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
