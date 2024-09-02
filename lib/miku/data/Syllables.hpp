#ifndef MIKU_DATA_SYLLABLES_HPP
#define MIKU_DATA_SYLLABLES_HPP

#include <vector>

namespace miku::data::syllables {
    const std::vector<char> Vowels = {
        'A',
        'I',
        'U',
        'E',
        'O'
    };

    const std::vector<char> ConsonantPrefixes = {
        'K',
        'S',
        'T',
        'N',
        'H',
        'M',
        'Y',
        'R',
        'W',
    };
}

#endif