#include "dijkstras.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <graph_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    Graph G;

    try {
        file_to_graph(filename, G);
        int source = 0;
        vector<int> previous;
        vector<int> distances = dijkstra_shortest_path(G, source, previous);
        cout << "Shortest paths from vertex " << source << ":" << endl;
        for (int dest = 0; dest < G.numVertices; ++dest) {
            if (distances[dest] == INF) {
                cout << "To vertex " << dest << ": No path exists" << endl;
            } else {
                vector<int> path = extract_shortest_path(distances, previous, dest);
                cout << "To vertex " << dest << ": ";
                print_path(path, distances[dest]);
            }
        }
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}


// g++ -std=c++17 -o dijkstras src/dijkstras_main.cpp src/dijkstras.cpp
// ./dijkstras src/largest.txt