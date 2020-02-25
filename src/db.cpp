#include "db.hpp"

#include <iostream>

#include "bsc/string.hpp"
#include "bsc/vec.hpp"

DBConnection::DBConnection() {
    auto exit_code = sqlite3_open("./data.db", &db);
    if (exit_code) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        std::exit(1);
    }
}

DBConnection::~DBConnection() { sqlite3_close(db); }

sqlite3* DBConnection::get() { return db; }

Dictionary DB::get_words_by_tag(std::string tag) {
    auto query =
        "SELECT kanji, readings, meanings FROM words "
        "WHERE tags LIKE \"" +
        tag + "\" AND kanji != '';";

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
    sqlite3_exec(db.get(), query.c_str(), callback, &words, &errmsg);

    if (errmsg) {
        std::cout << "err: " << errmsg << "\n";
    }

    return Dictionary(words);
}

DBConnection DB::db{};
