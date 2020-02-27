#pragma once

#include "words.hpp"

namespace jquiz::quiz {

enum class Mode {
    KanjiToReading = 1,
    MeaningToWord = 2,
};

void kanji_to_reading(words::WordList& wordlist);
void meaning_to_word(words::WordList& wordlist);

}  // namespace jquiz::quiz
