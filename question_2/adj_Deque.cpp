#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime> // for srand and rand
using namespace std;

    // Function to generate a random adjacency matrix of size n x n
    vector<vector<int>> generateRandomAdjMatrix(int n, int m) {
        vector<vector<int>> adj(n, vector<int>(n, 0));

        // Generate m random edges
        srand(time(0)); // Seed for random number generation
        int count = 0;
        while (count < m) {
            int u = rand() % n; // Random vertex u
            int v = rand() % n; // Random vertex v

            if (u != v && adj[u][v] == 0) {
                adj[u][v] = 1; // Mark edge from u to v
                count++;
            }
        }

        return adj;
    }

    // dfs Function to reach destination
    bool dfs(int curr, int des, vector<vector<int>>& adj, vector<int>& vis) {
        // If curr node is destination return true
        if (curr == des) {
            return true;
        }
        vis[curr] = 1;
        for (int x = 0; x < adj.size(); x++) {
            if (adj[curr][x] && !vis[x]) {
                if (dfs(x, des, adj, vis)) {
                    return true;
                }
            }
        }
        return false;
    }

    // To tell whether there is path from source to destination
    bool isPath(int src, int des, vector<vector<int>>& adj) {
        vector<int> vis(adj.size(), 0);
        deque<int> q;
        q.push_back(src);
        vis[src] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            for (int v = 0; v < adj.size(); v++) {
                if (adj[u][v] && !vis[v]) {
                    if (v == des) {
                        return true;
                    }
                    vis[v] = 1;
                    q.push_back(v);
                }
            }
        }
        return false;
    }

    // Function to return all the strongly connected components of a graph.
    vector<vector<int>> findSCC(int n, vector<vector<int>>& adj) {
        // Stores all the strongly connected components.
        vector<vector<int>> ans;

        // Stores whether a vertex is a part of any Strongly Connected Component
        vector<int> is_scc(n, 0);

        // Traversing all the vertices
        for (int i = 0; i < n; i++) {
            if (!is_scc[i]) {
                // If a vertex i is not a part of any SCC
                // insert it into a new SCC list and check
                // for other vertices whether they can be
                // the part of this list.
                vector<int> scc;
                scc.push_back(i);

                for (int j = i + 1; j < n; j++) {
                    // If there is a path from vertex i to vertex j and vice versa
                    // put vertex j into the current SCC list.
                    if (!is_scc[j] && isPath(i, j, adj) && isPath(j, i, adj)) {
                        is_scc[j] = 1;
                        scc.push_back(j);
                    }
                }

                // Insert the SCC containing vertex i into the final list.
                ans.push_back(scc);
            }
        }
        return ans;
    }




int main(int argc, char const* argv[]) {

    vector<vector<int>> adjMatrix =generateRandomAdjMatrix(1000, 2000);
    vector<vector<int>> ans = findSCC(1000, adjMatrix);
    // Output the strongly connected components
    cout << "The strongly connected components are:" << endl;
    for (int i = 0; i < ans.size(); i++) {
        for (int j = 0; j < ans[i].size(); j++) {
            cout << ans[i][j]+1<< " ";
        }
        cout << endl;
    }

    return 0;
}
