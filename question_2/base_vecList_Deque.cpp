
#include <algorithm>  // Include for std::find
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

// dfs Function to reach destination
bool dfs(int curr, int des, vector<list<int>>& adj, vector<int>& vis) {
    if (curr == des) {
        return true;
    }
    vis[curr] = 1;
    for (auto x : adj[curr]) {
        // Adjust for 0-based indexing
        int adjustedX = x - 1;
        if (adjustedX >= 0 && adjustedX < adj.size() && !vis[adjustedX]) {
            if (dfs(adjustedX, des, adj, vis)) {
                return true;
            }
        }
    }
    return false;
}


// To tell whether there is path from source to destination
bool isPath(int src, int des, vector<list<int>>& adj) {
    vector<int> vis(adj.size(), 0);
    deque<int> q;
    // Adjust for 0-based indexing
    q.push_back(src - 1);
    vis[src - 1] = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        for (auto v : adj[u]) {
            // Adjust for 0-based indexing
            int adjustedV = v - 1;
            if (adjustedV >= 0 && adjustedV < adj.size() && !vis[adjustedV]) {
                if (adjustedV == des - 1) {
                    return true;
                }
                vis[adjustedV] = 1;
                q.push_back(adjustedV);
            }
        }
    }
    return false;
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

// Function to return all the strongly connected components of a graph.
vector<vector<int>> findSCC(int n, vector<list<int>>& adj) {
    vector<vector<int>> ans;
    vector<int> is_scc(n, 0);
    for (int i = 0; i < n; i++) {
        if (!is_scc[i]) {
            vector<int> scc;
            scc.push_back(i + 1); // Adjust for 1-based indexing in output
            for (int j = 0; j < n; j++) {
                if (!is_scc[j] && isPath(i + 1, j + 1, adj) && isPath(j + 1, i + 1, adj)) {
                    is_scc[j] = 1;
                    scc.push_back(j + 1); // Adjust for 1-based indexing in output
                }
            }
            ans.push_back(scc);
        }
    }
    return ans;
}

int main(int argc, char const* argv[]) {
    vector<list<int>> vecList = {
        {2},     // Edge from 1 to 2
        {3},     // Edge from 2 to 3
        {1, 4},  // Edges from 3 to 1 and 3 to 4
        {5},     // Edge from 4 to 5
        {}       // 5 has no outgoing edges
    };
    vector<vector<int>> ans = findSCC(5, vecList);

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
