#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>  // Include for std::find
#include <cstdlib>
#include <ctime>

using namespace std;

// Helper function for DFS
void DFS(int v, vector<list<int>>& adj, vector<bool>& visited, stack<int>& Stack) {
    visited[v] = true;
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            DFS(neighbor, adj, visited, Stack);
        }
    }
    Stack.push(v);
}

// Function to get the transpose of the graph
vector<list<int>> getTranspose(vector<list<int>>& adj) {
    vector<list<int>> transpose(adj.size());
    for (int v = 0; v < adj.size(); v++) {
        for (int neighbor : adj[v]) {
            transpose[neighbor].push_back(v);
        }
    }
    return transpose;
}

// Helper function for DFS on transposed graph
void DFSUtil(int v, vector<list<int>>& adj, vector<bool>& visited, vector<int>& scc) {
    visited[v] = true;
    scc.push_back(v);
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            DFSUtil(neighbor, adj, visited, scc);
        }
    }
}

// Function to find strongly connected components using Kosaraju's algorithm
vector<vector<int>> findSCC(int n, vector<list<int>>& adj) {
    stack<int> Stack;
    vector<bool> visited(n, false);

    // Fill vertices in stack according to their finishing times
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFS(i, adj, visited, Stack);
        }
    }

    // Get the transpose of the graph
    vector<list<int>> transpose = getTranspose(adj);

    // Mark all the vertices as not visited (for second DFS)
    fill(visited.begin(), visited.end(), false);

    // Now process all vertices in order defined by Stack
    vector<vector<int>> sccs;
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            vector<int> scc;
            DFSUtil(v, transpose, visited, scc);
            sccs.push_back(scc);
        }
    }
    return sccs;
}
// Function to generate a random graph represented as vector of lists
vector<list<int>> generateRandomGraph(int n, int m) {
    vector<list<int>> adj(n);

    // Generate m random edges
    srand(time(0));  // Seed for random number generation
    int count = 0;
    while (count < m) {
        int u = rand() % n;  // Random vertex u
        int v = rand() % n;  // Random vertex v

        if (u != v && find(adj[u].begin(), adj[u].end(), v) == adj[u].end()) {
            adj[u].push_back(v);  // Add edge from u to v
            count++;
        }
    }

    return adj;
}

int main(int argc, char const* argv[]) {

    int n =1000;
    int m = 2000;
    vector<list<int>> vecList = generateRandomGraph(n, m);
    vector<vector<int>> ans = findSCC(n, vecList);

    // Output the strongly connected components
    cout << "The strongly connected components are:" << endl;
    for (int i = 0; i < ans.size(); i++) {
        for (int j = 0; j < ans[i].size(); j++) {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}



