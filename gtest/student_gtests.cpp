#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "/home/zhouhent/ICS46/HW9/src/dijkstras.h"
#include "/home/zhouhent/ICS46/HW9/src/ladder.h"

// Explicitly declare the ladder functions we're testing
extern bool edit_distance_within(const std::string& str1, const std::string& str2, int d);
extern bool is_adjacent(const std::string& word1, const std::string& word2);
extern std::vector<std::string> generate_word_ladder(const std::string& begin_word, const std::string& end_word, const std::set<std::string>& word_list);
extern void load_words(std::set<std::string>& word_list, const std::string& file_name);
extern void print_word_ladder(const std::vector<std::string>& ladder);
extern void verify_word_ladder();

// ==================== Ladder Tests ====================

TEST(Ladder, Edit_Distance_Within) {
    // Test exact same words
    EXPECT_TRUE(edit_distance_within("book", "book", 0));
    EXPECT_TRUE(edit_distance_within("book", "book", 1));
    
    // Test words with edit distance 1
    EXPECT_TRUE(edit_distance_within("fish", "wish", 1));    // Substitution
    EXPECT_TRUE(edit_distance_within("moon", "moons", 1));   // Insertion
    EXPECT_TRUE(edit_distance_within("stars", "star", 1));   // Deletion
    
    // Test words with edit distance 2
    EXPECT_TRUE(edit_distance_within("fish", "wish", 2));   // One substitution
    EXPECT_FALSE(edit_distance_within("ship", "fish", 1));  // Should fail with distance 1
    
    // Test case insensitivity
    EXPECT_TRUE(edit_distance_within("BOOK", "book", 0));
    EXPECT_TRUE(edit_distance_within("Book", "bOOk", 0));
    
    // Test different lengths
    EXPECT_TRUE(edit_distance_within("", "sun", 3));
    EXPECT_FALSE(edit_distance_within("", "moon", 3));
    EXPECT_TRUE(edit_distance_within("star", "", 4));
    
    // Test longer words
    EXPECT_TRUE(edit_distance_within("computer", "computers", 1));
    EXPECT_TRUE(edit_distance_within("computer", "commuter", 2));
    EXPECT_FALSE(edit_distance_within("computer", "commuter", 1));
}

TEST(Ladder, Is_Adjacent) {
    // Test exact same words (not adjacent)
    EXPECT_FALSE(is_adjacent("world", "world"));
    
    // Test words with edit distance 1 (should be adjacent)
    EXPECT_TRUE(is_adjacent("cold", "gold"));     // Substitution
    EXPECT_TRUE(is_adjacent("play", "plays"));    // Insertion
    EXPECT_TRUE(is_adjacent("reads", "read"));    // Deletion
    
    // Test words with edit distance > 1 (should not be adjacent)
    EXPECT_FALSE(is_adjacent("blue", "black"));   // Multiple changes
    EXPECT_FALSE(is_adjacent("fish", "bird"));    // Total change
    
    // Test case insensitivity
    EXPECT_FALSE(is_adjacent("COLD", "cold"));    // Same word different case
    EXPECT_TRUE(is_adjacent("COLD", "gold"));     // Should be adjacent despite case
    
    // Test different lengths
    EXPECT_TRUE(is_adjacent("plant", "plan"));    // Delete one letter
    EXPECT_TRUE(is_adjacent("read", "reads"));    // Add one letter
    EXPECT_FALSE(is_adjacent("run", "ruins"));    // Add multiple letters
}

TEST(Ladder, Generate_Word_Ladder) {
    std::set<std::string> word_list = {"cold", "gold", "golf", "wolf", "word", "wood", "wool"};
    
    // Test a simple ladder
    std::vector<std::string> expected = {"cold", "gold", "golf", "wolf"};
    std::vector<std::string> result = generate_word_ladder("cold", "wolf", word_list);
    EXPECT_EQ(result, expected);
    
    // Test with same start and end words
    result = generate_word_ladder("cold", "cold", word_list);
    EXPECT_TRUE(result.empty());
    
    // Test with non-existent end word
    result = generate_word_ladder("cold", "bold", word_list);
    EXPECT_TRUE(result.empty());
    
    // Test with words of different lengths
    word_list.insert("world");
    word_list.insert("worlds");
    expected = {"word", "wood", "wool", "world", "worlds"};
    result = generate_word_ladder("word", "worlds", word_list);
    EXPECT_EQ(result.size(), 5);
    
    // Test case sensitivity (start word should work regardless of case)
    result = generate_word_ladder("COLD", "wolf", word_list);
    EXPECT_EQ(result.size(), 4);
}

TEST(Ladder, Load_Words) {
    // Create a temporary test file
    std::ofstream testFile("test_words.txt");
    testFile << "blue\nred\ngreen\nyellow\nblack\n";
    testFile.close();
    
    std::set<std::string> word_list;
    load_words(word_list, "test_words.txt");
    
    EXPECT_EQ(word_list.size(), 5);
    EXPECT_TRUE(word_list.find("blue") != word_list.end());
    EXPECT_TRUE(word_list.find("red") != word_list.end());
    EXPECT_TRUE(word_list.find("green") != word_list.end());
    EXPECT_TRUE(word_list.find("yellow") != word_list.end());
    EXPECT_TRUE(word_list.find("black") != word_list.end());
    
    // Test with non-existent file
    std::set<std::string> empty_list;
    load_words(empty_list, "non_existent_file.txt");
    EXPECT_TRUE(empty_list.empty());
    
    // Clean up
    remove("test_words.txt");
}

// We'll test verify_word_ladder indirectly since it calls generate_word_ladder
TEST(Ladder, Verify_Word_Ladder) {
    // Capture output to check if tests pass
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    // Run the verification function
    verify_word_ladder();
    
    // Restore stdout
    std::cout.rdbuf(old);
    
    // Check if "All word ladder verifications passed" appears in the output
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("All word ladder verifications passed") != std::string::npos || 
                output.find("passed") != std::string::npos);
}

TEST(Ladder, Print_Word_Ladder) {
    // Test empty ladder
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    print_word_ladder({});
    
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("No word ladder found") != std::string::npos);
    
    // Test non-empty ladder
    buffer.str("");
    old = std::cout.rdbuf(buffer.rdbuf());
    
    print_word_ladder({"cold", "gold", "golf", "wolf", "word", "wood"});
    
    std::cout.rdbuf(old);
    output = buffer.str();
    EXPECT_TRUE(output.find("Word ladder found: cold gold golf wolf word wood") != std::string::npos);
}

// ==================== Dijkstra's Tests ====================

// Helper function to create a test graph
Graph createTestGraph() {
    Graph G;
    G.numVertices = 4;
    G.resize(G.numVertices);
    
    // Add edges using Edge constructor: src, dst, weight
    G[0].push_back(Edge(0, 1, 1));
    G[0].push_back(Edge(0, 2, 4));
    G[1].push_back(Edge(1, 2, 2));
    G[1].push_back(Edge(1, 3, 6));
    G[2].push_back(Edge(2, 3, 3));
    
    return G;
}

TEST(Graph, Constructor) {
    // Create a graph and check it's empty
    Graph G;
    G.numVertices = 3;
    G.resize(G.numVertices);
    
    EXPECT_EQ(G.size(), 3);
    EXPECT_EQ(G.numVertices, 3);
    EXPECT_TRUE(G[0].empty());
    EXPECT_TRUE(G[1].empty());
    EXPECT_TRUE(G[2].empty());
    
    // Add an edge
    G[0].push_back(Edge(0, 1, 5));
    
    EXPECT_EQ(G[0].size(), 1);
    EXPECT_EQ(G[0][0].src, 0);
    EXPECT_EQ(G[0][0].dst, 1);
    EXPECT_EQ(G[0][0].weight, 5);
}

TEST(Graph, File_to_Graph) {
    // Create a temporary test file
    std::ofstream testFile("test_graph.txt");
    testFile << "3\n"; // 3 nodes
    testFile << "0 1 5\n"; // Edge from 0 to 1 with weight 5
    testFile << "0 2 10\n"; // Edge from 0 to 2 with weight 10
    testFile << "1 2 2\n"; // Edge from 1 to 2 with weight 2
    testFile.close();
    
    try {
        Graph G;
        file_to_graph("test_graph.txt", G);
        
        EXPECT_EQ(G.numVertices, 3);
        EXPECT_EQ(G.size(), 3);
        EXPECT_EQ(G[0].size(), 2); // Vertex 0 has 2 edges
        EXPECT_EQ(G[1].size(), 1); // Vertex 1 has 1 edge
        EXPECT_EQ(G[2].size(), 0); // Vertex 2 has 0 edges
        
        // Check edge details
        EXPECT_EQ(G[0][0].src, 0);
        EXPECT_EQ(G[0][0].dst, 1);
        EXPECT_EQ(G[0][0].weight, 5);
        EXPECT_EQ(G[0][1].src, 0);
        EXPECT_EQ(G[0][1].dst, 2);
        EXPECT_EQ(G[0][1].weight, 10);
        EXPECT_EQ(G[1][0].src, 1);
        EXPECT_EQ(G[1][0].dst, 2);
        EXPECT_EQ(G[1][0].weight, 2);
    } catch (const std::exception& e) {
        FAIL() << "Exception thrown: " << e.what();
    }
    
    // Clean up
    remove("test_graph.txt");
}

TEST(Dijkstras, Dijkstra_Shortest_Path) {
    Graph G = createTestGraph();
    std::vector<int> previous;
    int source = 0;
    
    std::vector<int> distances = dijkstra_shortest_path(G, source, previous);
    
    // Check distances
    ASSERT_EQ(distances.size(), 4);
    EXPECT_EQ(distances[0], 0);  // Distance to self is 0
    EXPECT_EQ(distances[1], 1);  // Distance to node 1 is 1
    EXPECT_EQ(distances[2], 3);  // Distance to node 2 is 3 (through node 1)
    EXPECT_EQ(distances[3], 6);  // Distance to node 3 is 6 (through nodes 1 and 2)
    
    // Check previous nodes
    ASSERT_EQ(previous.size(), 4);
    EXPECT_EQ(previous[0], -1); // Source has no previous
    EXPECT_EQ(previous[1], 0);  // Previous of 1 is 0
    EXPECT_EQ(previous[2], 1);  // Previous of 2 is 1
    EXPECT_EQ(previous[3], 2);  // Previous of 3 is 2
}

TEST(Dijkstras, Extract_Shortest_Path) {
    // Test case with a valid path
    std::vector<int> distances = {0, 1, 3, 6};
    std::vector<int> previous = {-1, 0, 1, 2}; // 0 -> 1 -> 2 -> 3
    
    // Path from 0 to 3
    std::vector<int> path = extract_shortest_path(distances, previous, 3);
    std::vector<int> expected = {0, 1, 2, 3};
    EXPECT_EQ(path, expected);
    
    // Path from 0 to 1
    path = extract_shortest_path(distances, previous, 1);
    expected = {0, 1};
    EXPECT_EQ(path, expected);
    
    // Path from 0 to 0 (should be just 0)
    path = extract_shortest_path(distances, previous, 0);
    expected = {0};
    EXPECT_EQ(path, expected);
    
    // Test case with no path
    distances = {0, INF, INF, INF};
    previous = {-1, -1, -1, -1}; // No paths
    path = extract_shortest_path(distances, previous, 3);
    EXPECT_TRUE(path.empty());
}

TEST(Dijkstras, Print_Path) {
    // Test a valid path
    std::vector<int> path = {0, 1, 2, 3};
    int totalCost = 6;
    
    // Capture stdout to check output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    print_path(path, totalCost);
    
    // Restore stdout
    std::cout.rdbuf(old);
    
    // Check the output contains expected information
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("0 1 2 3") != std::string::npos);
    EXPECT_TRUE(output.find("Total cost is 6") != std::string::npos);
    
    // Test empty path
    buffer.str("");
    old = std::cout.rdbuf(buffer.rdbuf());
    
    print_path({}, 0);
    
    std::cout.rdbuf(old);
    output = buffer.str();
    EXPECT_TRUE(output.find("Total cost is 0") != std::string::npos);
}

TEST(Graph, Printer) {
    // Test Edge printing with operator<<
    Edge e(0, 1, 5);
    std::stringstream buffer;
    buffer << e;
    
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("(0,1,5)") != std::string::npos);
    
    // Test printing multiple edges
    Edge e2(1, 2, 3);
    buffer.str("");
    buffer << e << " and " << e2;
    
    output = buffer.str();
    EXPECT_TRUE(output.find("(0,1,5) and (1,2,3)") != std::string::npos);
}
