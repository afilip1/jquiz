#include <iostream>

#include "db.hpp"
#include "quiz.hpp"
#include "term.hpp"
#include "words.hpp"

namespace jquiz {

int main(int argc, char** argv) {
    auto mode =
        static_cast<quiz::Mode>(term::prompt<int>("Select quiz mode:\n"
                                                  "  1) Kanji -> Reading\n"
                                                  "  2) Meaning -> Word"));

    auto tags = db::DB::get_tags().iter().join<bsc::String>(' ');
    auto tag = term::prompt<bsc::String>("Select word list:\n  " + tags);

    switch (mode) {
        case quiz::Mode::KanjiToReading: {
            auto wordlist = db::DB::get_words_with_kanji_by_tag(tag);
            while (true) {
                quiz::kanji_to_reading(wordlist);
            }
        } break;

        case quiz::Mode::MeaningToWord: {
            auto wordlist = db::DB::get_words_by_tag(tag);
            while (true) {
                quiz::meaning_to_word(wordlist);
            }
        } break;
    }
}

}  // namespace jquiz

int main(int argc, char** argv) { jquiz::main(argc, argv); }
