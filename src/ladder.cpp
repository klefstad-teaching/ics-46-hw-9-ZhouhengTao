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
