#pragma once

#include <sqlite3.h>

#include "dict.hpp"

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
    static Dictionary get_words_by_tag(std::string tag);

   private:
    static DBConnection db;
};
