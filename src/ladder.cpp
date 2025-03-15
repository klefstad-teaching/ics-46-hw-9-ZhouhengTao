#include "ladder.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

void error(string word1, string word2, string msg) {
    cout << "Cannot create word ladder from '" << word1 << "' to '" << word2 << "': " << msg << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int len1 = str1.length();
    int len2 = str2.length();
    
    if (abs(len1 - len2) > d) {
        return false;
    }
    
    if (len1 == 0) return len2 <= d;
    if (len2 == 0) return len1 <= d;
    
    // Edit distance = 1
    if (d == 1) {
        // If same length, only one character can differ
        if (len1 == len2) {
            int diff_count = 0;
            for (int i = 0; i < len1; i++) {
                if (tolower(str1[i]) != tolower(str2[i])) {
                    diff_count++;
                    if (diff_count > 1) return false;
                }
            }
            return diff_count <= 1;
        }
        // If length differs by 1, check for insertion/deletion
        else if (abs(len1 - len2) == 1) {
            const string& shorter = (len1 < len2) ? str1 : str2;
            const string& longer = (len1 < len2) ? str2 : str1;
            int shorter_idx = 0, longer_idx = 0;
            bool skip_used = false;
            
            while (shorter_idx < shorter.length() && longer_idx < longer.length()) {
                if (tolower(shorter[shorter_idx]) != tolower(longer[longer_idx])) {
                    if (skip_used) return false;
                    skip_used = true;
                    longer_idx++;
                } else {
                    shorter_idx++;
                    longer_idx++;
                }
            }
            return true;
        }
        return false;
    }
    

    // Create a dynamic programming matrix
    vector<int> prev(len2 + 1), curr(len2 + 1);
    for (int j = 0; j <= len2; j++) prev[j] = j;

    for (int i = 1; i <= len1; i++) {
        curr[0] = i;
        for (int j = 1; j <= len2; j++) {
            if (tolower(str1[i-1]) == tolower(str2[j-1])) {
                curr[j] = prev[j-1];
            } else {
                curr[j] = 1 + min(prev[j-1], min(prev[j], curr[j-1]));
            }
        }
        prev = curr;
    }
    return prev[len2] <= d;
}

// Check if two words are adjacent (edit distance = 1)
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

unordered_map<int, vector<string>> organize_by_length(const set<string>& word_list) {
    unordered_map<int, vector<string>> length_map;
    for (const string& word : word_list) {
        length_map[word.length()].push_back(word);
    }
    return length_map;
}

vector<string> find_potential_neighbors(const string& word, 
                                       const unordered_map<int, vector<string>>& words_by_length) {
    vector<string> candidates;
    int len = word.length();
    
    // For each possible length (same, one bit shorter, one bit longer)
    for (int l : {len, len - 1, len + 1}) {
        if (l < 1 || words_by_length.find(l) == words_by_length.end()) continue;
        
        if (l == len) {
            for (const string& candidate : words_by_length.at(l)) {
                if (candidate == word) continue;
                
                int diff = 0;
                for (int i = 0; i < len && diff <= 1; i++) {
                    if (tolower(word[i]) != tolower(candidate[i])) diff++;
                }
                
                if (diff == 1) {
                    candidates.push_back(candidate);
                }
            }
        }
        // For the case where the length difference is 1, add all candidate words and perform precise checks later
        else {
            for (const string& candidate : words_by_length.at(l)) {
                candidates.push_back(candidate);
            }
        }
    }
    
    return candidates;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // Convert input words to lowercase
    string begin_lower = begin_word;
    string end_lower = end_word;
    for (char& c : begin_lower) c = tolower(c);
    for (char& c : end_lower) c = tolower(c);
    
    if (begin_lower == end_lower) {
        error(begin_word, end_word, "Start and end words are the same");
        return {};
    }
    
    // Check if end word is in the dictionary
    if (word_list.find(end_lower) == word_list.end()) {
        error(begin_word, end_word, "End word not in dictionary");
        return {};
    }
    
    // Precompute words grouped by length
    unordered_map<int, vector<string>> words_by_length = organize_by_length(word_list);
    
    // Initialize queue for BFS
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_lower});
    
    unordered_set<string> visited;
    visited.insert(begin_lower);
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        // Use pre-filtering to reduce the number of words you need to check
        vector<string> potential_neighbors = find_potential_neighbors(last_word, words_by_length);
        
        // Check words of same length first, then shorter, then longer
        int last_len = last_word.length();
        
        for (int len : {last_len, last_len - 1, last_len + 1}) {
            if (len < 1) continue;
            
            for (const string& word : potential_neighbors) {
                if (word.length() != len) continue;
                if (visited.find(word) != visited.end()) continue;
                
                if (is_adjacent(last_word, word)) {
                    visited.insert(word);
                    
                    vector<string> new_ladder = current_ladder;
                    new_ladder.push_back(word);
                    
                    if (word == end_lower) {
                        return new_ladder;
                    }
                    
                    ladder_queue.push(new_ladder);
                }
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
    
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " ";
        }
    }
    cout << " " << endl;
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
