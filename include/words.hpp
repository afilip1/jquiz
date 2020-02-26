#pragma once

#include <cstdlib>
#include <ctime>

#include "bsc/string.hpp"
#include "bsc/vec.hpp"

namespace jquiz::words {

struct Word {
    bsc::String kanji;
    bsc::Vec<bsc::String> readings;
    bsc::String meanings;
};

class WordList {
   public:
    WordList(bsc::Vec<Word> words) : words{words} {
        std::srand(std::time(nullptr));
    }

    Word get_random_word() { return words[std::rand() % words.size()]; }

   protected:
    bsc::Vec<Word> words;
};

}  // namespace jquiz::words
