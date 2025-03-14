#include "ladder.h"
#include <iostream>
#include <string>
#include <algorithm>

int main() {
    set<string> word_list;
    try {
        load_words(word_list, "src/words.txt");
    } catch (const exception& e) {
        cerr << "Error loading dictionary: " << e.what() << endl;
        return 1;
    }
    
    string begin_word, end_word;
    
    cout << "Enter the start word: ";
    cin >> begin_word;
    
    cout << "Enter the end word: ";
    cin >> end_word;
    
    transform(begin_word.begin(), begin_word.end(), begin_word.begin(), ::tolower);
    transform(end_word.begin(), end_word.end(), end_word.begin(), ::tolower);
    
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return 1;
    }
    
    if (word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "End word not found in dictionary");
        return 1;
    }
    
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    
    if (ladder.empty()) {
        cout << "No word ladder found between '" << begin_word << "' and '" << end_word << "'." << endl;
    } else {
        cout << "Word ladder from '" << begin_word << "' to '" << end_word << "':" << endl;
        print_word_ladder(ladder);
    }
    
    return 0;
}

// g++ -std=c++17 -o ladder src/ladder_main.cpp src/ladder.cpp
// ./ladder