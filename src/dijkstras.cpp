#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int numVertices = G.numVertices;
    
    vector<int> distances(numVertices, INF);
    vector<bool> visited(numVertices, false);
    previous.resize(numVertices, -1);
    distances[source] = 0;
    // previous[source] = UNDEFINED;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
    minHeap.push({0, source});
    
    while (!minHeap.empty()) {
        int u = minHeap.top().second;
        minHeap.pop();
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            if (!visited[v] && distances[u] != INF && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                minHeap.push({distances[v], v});
            }
        }
    }
    
    return distances;
}
