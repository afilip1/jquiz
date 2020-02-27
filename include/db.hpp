#pragma once

#include <sqlite3.h>

#include "words.hpp"

namespace jquiz::db {

class DBConnection {
   public:
    DBConnection();
    ~DBConnection();

    sqlite3* get();

   private:
    sqlite3* db;
};

class DB {
   public:
    static bsc::Vec<bsc::String> get_tags();
    static words::WordList get_words_with_kanji_by_tag(std::string tag);
    static words::WordList get_words_by_tag(std::string tag);

   private:
    static DBConnection db;
};

}  // namespace jquiz::db

