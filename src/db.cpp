#include "db.hpp"

#include <iostream>

#include "bsc/string.hpp"
#include "bsc/vec.hpp"

namespace jquiz::db {

DBConnection::DBConnection() {
    auto exit_code = sqlite3_open("./data.db", &db);
    if (exit_code) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        std::exit(1);
    }
}

DBConnection::~DBConnection() { sqlite3_close(db); }

sqlite3* DBConnection::get() { return db; }

bsc::Vec<bsc::String> DB::get_tags() {
    auto query = "SELECT DISTINCT tags FROM words;";

    auto callback = [](void* data, int argc, char** argv,
                       char** _column_names) {
        auto& tags = *static_cast<bsc::Vec<bsc::String>*>(data);
        tags.push_back(bsc::String(argv[0]));
        return 0;
    };

    bsc::Vec<bsc::String> tags;
    char* errmsg = nullptr;

    sqlite3_exec(db.get(), query, callback, &tags, &errmsg);
    return tags;
}

words::WordList DB::get_words_with_kanji_by_tag(std::string tag) {
    auto query =
        "SELECT kanji, readings, meanings FROM words "
        "WHERE kanji != '' AND tags LIKE \"" +
        tag + "\";";

    auto word_inserter_callback = [](void* data, int argc, char** argv,
                                     char** _column_names) {
        auto& words = *static_cast<bsc::Vec<words::Word>*>(data);

        bsc::String kanji(argv[0] ? argv[0] : "");
        auto readings = bsc::String(argv[1]).split_on(',');
        bsc::String meanings(argv[2]);

        auto word = words::Word{kanji, readings, meanings};
        words.push_back(word);

        return 0;
    };

    bsc::Vec<words::Word> words;
    char* errmsg = nullptr;
    sqlite3_exec(db.get(), query.c_str(), word_inserter_callback, &words,
                 &errmsg);

    if (errmsg) {
        std::cout << "err: " << errmsg << "\n";
    }

    return words::WordList(words);
}

words::WordList DB::get_words_by_tag(std::string tag) {
    auto query =
        "SELECT kanji, readings, meanings FROM words "
        "WHERE tags LIKE \"" +
        tag + "\";";

    auto word_inserter_callback = [](void* data, int argc, char** argv,
                                     char** _column_names) {
        auto& words = *static_cast<bsc::Vec<words::Word>*>(data);

        bsc::String kanji(argv[0] ? argv[0] : "");
        auto readings = bsc::String(argv[1]).split_on(',');
        bsc::String meanings(argv[2]);

        auto word = words::Word{kanji, readings, meanings};
        words.push_back(word);

        return 0;
    };

    bsc::Vec<words::Word> words;
    char* errmsg = nullptr;
    sqlite3_exec(db.get(), query.c_str(), word_inserter_callback, &words,
                 &errmsg);

    if (errmsg) {
        std::cout << "err: " << errmsg << "\n";
    }

    return words::WordList(words);
}

DBConnection DB::db{};

}  // namespace jquiz::db
