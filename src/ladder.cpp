#include "ladder.h"

void error(string word1, string word2, string msg) {
    cout << "Cannot create word ladder from '" << word1 << "' to '" << word2 << "': " << msg << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int len1 = str1.length();
    int len2 = str2.length();
    
    if (abs(len1 - len2) > d) {
        return false;
    }
    
    if (len1 == 0) {
        return len2 <= d;
    }
    if (len2 == 0) {
        return len1 <= d;
    }
    
    // Create a dynamic programming matrix
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));
    
    // Initialize the first row and column
    for (int i = 0; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; j++) {
        dp[0][j] = j;
    }
    // Fill the matrix
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                // Take the minimum of three operations
                dp[i][j] = 1 + min(dp[i-1][j-1],  // Replace
                                  min(dp[i-1][j],   // Delete
                                      dp[i][j-1])); // Insert
            }
        }
    }
    return dp[len1][len2] <= d;
}

// Check if two words are adjacent (edit distance = 1)
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return {};
    }
    
    // Initialize queue for BFS
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    
    // Set to track visited words to avoid cycles
    set<string> visited;
    visited.insert(begin_word);
    
    // BFS to find shortest path
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        // Check all words in dictionary to find neighbors
        for (const string& word : word_list) {
            if (visited.find(word) != visited.end()) {
                continue;
            }
            
            // Check if the word is adjacent to the last word in the ladder
            if (is_adjacent(last_word, word)) {
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(word);
                
                if (word == end_word) {
                    return new_ladder;
                }
                visited.insert(word);  
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    error(begin_word, end_word, "No word ladder exists");
    return {};
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << file_name << endl;
        return;
    }
    
    string word;
    while (file >> word) {
        for (char& c : word) {
            c = tolower(c);
        }
        word_list.insert(word);
    }
    
    file.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " → ";
        }
    }
    cout << endl;
    
    cout << "Ladder length: " << ladder.size() << endl;
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}
