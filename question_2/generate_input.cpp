#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

// Function to generate a random directed graph with num_vertices vertices and num_edges edges
void generateGraph(int num_vertices, int num_edges, vector<pair<int, int>>& edges) {
    // Initialize a random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(1, num_vertices);

    // Create edges randomly
    for (int i = 0; i < num_edges; ++i) {
        int u = distrib(gen);
        int v = distrib(gen);
        edges.push_back({u, v});
    }
}

// Function to write edges to a file in the specified format
void writeGraphToFile(const vector<pair<int, int>>& edges, const string& filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error: Unable to open file: " << filename << endl;
        return;
    }

    for (const auto& edge : edges) {
        outfile << edge.first << " " << edge.second << "\n";
    }

    outfile.close();
    cout << "Graph edges written to " << filename << endl;
}

int main() {
    int num_vertices = 1000;
    int num_edges = 1000;
    int num_scc = 100; // Number of SCCs
    int scc_size = num_vertices / num_scc; // Size of each SCC

    vector<pair<int, int>> edges;

    // Generate the random graph
    generateGraph(num_vertices, num_edges, edges);

    // Ensure presence of SCCs (add cycles)
    for (int i = 0; i < num_vertices; i += scc_size) {
        for (int j = 0; j < scc_size; ++j) {
            int u = (i + j) % num_vertices + 1;
            int v = (i + j + 1) % num_vertices + 1;
            edges.push_back({u, v});
        }
    }

    // Write the graph to file
    string filename = "input.txt";
    writeGraphToFile(edges, filename);

    return 0;
}
