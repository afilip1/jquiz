#pragma once

#include "bsc/string.hpp"
#include "bsc/vec.hpp"

struct Word {
    bsc::String kanji;
    bsc::Vec<bsc::String> readings;
    bsc::String meanings;
};

class Dictionary {
   public:
    Dictionary(bsc::Vec<Word> words) : dict{words} {}

    virtual Word random_word() {
        //
        return dict[std::rand() % dict.size()];
    }

   protected:
    bsc::Vec<Word> dict;
};

