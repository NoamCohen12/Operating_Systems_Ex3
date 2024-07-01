

#include <algorithm>  // Include for std::find
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <list>
#include <stack>
#include <vector>
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

auto Newgraph(int n, int m) {  // function to create a new graph and get m edge inputs
    vector<list<int>> vecList(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cout << "Enter edge " << i + 1 << ": ";
        cin >> u >> v;
        vecList[u - 1].push_back(v - 1);
    }
    cout << "Graph created" << endl;
    return vecList;
}
void kosaraju(int n, vector<list<int>>& vecList) {
    vector<vector<int>> ans = findSCC(n, vecList);

    // Output the strongly connected components
    cout << "The strongly connected components are:" << endl;
    for (int i = 0; i < ans.size(); i++) {
        for (int j = 0; j < ans[i].size(); j++) {
            cout << ans[i][j] + 1 << " ";
        }
        cout << endl;
    }
}
void addEdge(int u, int v, vector<list<int>>& vecList) {
    vecList[u - 1].push_back(v - 1);
    cout << "added edge from " << u << " to " << v << endl;
}

void removeEdge(int u, int v, vector<list<int>>& vecList) {
    u = u - 1;
    v = v - 1;
    // For simplicity, assume the edge exists and remove it
    for (auto it = vecList[u].begin(); it != vecList[u].end(); ++it) {
        if (*it == v) {
            vecList[u].erase(it);
            cout << "removed edge from " << u + 1 << " to " << v + 1 << endl;
            return;
        }
    }
}

int main(int argc, char const* argv[]) {
    string input;
    int n, m;
    vector<list<int>> vecList;

    // vector<list<int>> vecList{
    //     {2},     // Edge from 1 to 2
    //     {3},     // Edge from 2 to 3
    //     {1, 4},  // Edges from 3 to 1 and 3 to 4
    //     {5},     // Edge from 4 to 5
    //     {}       // 5 has no outgoing edges
    // };

while (true) {
    // cout << "Enter input: ";
    if (!(cin >> input)) {  // exit if EOF
        break;
    }
    if (input == "Newgraph") {
        cout << "Enter number of vertices and edges: <vertices> <edges>";
        cin >> n >> m;
        vecList = Newgraph(n, m);
    } else if (input == "Kosaraju") {
        kosaraju(n, vecList);
    } else if (input == "Newedge") {
        int u, v;
        cout << "Enter edge:<u> to <v> ";
        cin >> u >> v;
        addEdge(u, v, vecList);
    } else if (input == "Removeedge") {
        int u, v;
        cout << "Enter edge:<u> to <v> ";
        cin >> u >> v;
        removeEdge(u, v, vecList);
    }
    else{
        cout << "Invalid input" << endl;
    }
}

return 0;
}