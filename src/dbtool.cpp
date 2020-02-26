#include <sqlite3.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "bsc/map.hpp"
#include "bsc/string.hpp"
#include "bsc/vec.hpp"

struct Word {
    bsc::String kanji;
    bsc::Vec<bsc::String> readings;
    bsc::String meanings;
};

bsc::Vec<Word> load_words(const std::string file) {
    std::ifstream words_file(file);
    bsc::Map<std::string, Word> words_map{};
    bsc::Vec<Word> no_kanji{};

    while (!words_file.eof()) {
        bsc::String line;
        std::getline(words_file, line);

        if (words_file.peek() == '\n') {
            words_file.get();  // consume leftover newline
        }

        auto components = line.split_on('\t');

        auto& kanji = components[0];
        auto& readings = components[1];
        auto& meanings = components[2];

        auto readings_split = readings.split_on('/');

        if (kanji == "") {
            no_kanji.push_back(Word{kanji, readings_split, meanings});
        } else if (words_map.contains(kanji)) {
            words_map[kanji].readings.append(readings_split);
        } else {
            words_map[kanji] = Word{kanji, readings_split, meanings};
        }
    }

    auto words = words_map.iter()
                     .map<Word>([](auto pair) { return pair.second; })
                     .collect();
    words.append(no_kanji);

    return words;
}

int main(int argc, char** argv) {
    sqlite3* db;
    auto exit = sqlite3_open("data.db", &db);

    if (exit) {
        std::cerr << "Error opening DB: " << sqlite3_errmsg(db) << "\n";
        return (-1);
    }

    std::cout << "DB OK\n";

    auto create =
        "CREATE TABLE IF NOT EXISTS words("
        "kanji      TEXT,"
        "readings   TEXT    NOT NULL,"
        "meanings   TEXT    NOT NULL,"
        "tags       TEXT    NOT NULL,"
        "PRIMARY KEY (kanji, readings, meanings));";

    char* errmsg = nullptr;
    sqlite3_exec(db, create, nullptr, nullptr, &errmsg);

    if (errmsg) {
        std::cout << errmsg << "\n";
        return -1;
    }
    std::cout << "create OK\n";

    sqlite3_exec(db, "BEGIN;", nullptr, nullptr, &errmsg);

    if (errmsg) {
        std::cout << errmsg << "\n";
        return -1;
    }
    std::cout << "begin OK\n";

    auto tag = argv[2];
    std::cout << "tag: " << tag << "\n";
    auto words = load_words(argv[1]);
    for (auto w : words) {
        auto sql =
            "INSERT INTO words(kanji, readings, meanings, tags) "
            "VALUES (\"" +
            w.kanji + "\", \"" + w.readings.iter().join<bsc::String>(',') +
            "\", \"" + w.meanings + "\", \"" + tag + "\");";

        sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
        if (errmsg) {
            std::cout << errmsg << "\n"
                      << w.kanji << " "
                      << w.readings.iter().join<bsc::String>(',') << " "
                      << w.meanings << "\n";
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, &errmsg);
            return -1;
        }
    }

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errmsg);

    if (errmsg) {
        std::cout << errmsg << "\n";
        return -1;
    }
    std::cout << "commit OK\n";

    std::cout << "insert OK\n";
}
