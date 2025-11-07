// villages_divide.cpp
// Centroid decomposition for counting pairs within distance K
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

struct Edge {
    int to, weight;
};

class CentroidDecomposition {
private:
    int n;
    int K;
    vector<vector<Edge>> adj;
    vector<bool> removed;
    vector<int> subtree_size;
    long long pair_count;

    // Calculate subtree sizes
    int calcSize(int u, int parent) {
        subtree_size[u] = 1;
        for (const Edge& e : adj[u]) {
            if (e.to != parent && !removed[e.to]) {
                subtree_size[u] += calcSize(e.to, u);
            }
        }
        return subtree_size[u];
    }

    // Find centroid
    int findCentroid(int u, int parent, int tree_size) {
        for (const Edge& e : adj[u]) {
            if (e.to != parent && !removed[e.to]) {
                if (subtree_size[e.to] > tree_size / 2) {
                    return findCentroid(e.to, u, tree_size);
                }
            }
        }
        return u;
    }

    // Collect distances from centroid to all nodes in subtree
    void collectDistances(int u, int parent, int dist, vector<int>& distances) {
        if (dist > K) return;  // Pruning
        distances.push_back(dist);
        for (const Edge& e : adj[u]) {
            if (e.to != parent && !removed[e.to]) {
                collectDistances(e.to, u, dist + e.weight, distances);
            }
        }
    }

    // Count pairs using sorted distances and binary search
    long long countPairs(const vector<int>& distances) {
        long long count = 0;
        int left = 0, right = distances.size() - 1;
        while (left < right) {
            if (distances[left] + distances[right] <= K) {
                count += (right - left);
                left++;
            } else {
                right--;
            }
        }
        return count;
    }

    // Main decomposition recursion
    void decompose(int u) {
        int tree_size = calcSize(u, -1);
        int centroid = findCentroid(u, -1, tree_size);
        removed[centroid] = true;

        // Accumulator multiset A starts with {0} representing the centroid itself
        multiset<int> A;
        A.insert(0);

        // Process each child subtree
        for (const Edge& e : adj[centroid]) {
            if (!removed[e.to]) {
                vector<int> D_S;
                collectDistances(e.to, centroid, e.weight, D_S);
                sort(D_S.begin(), D_S.end());

                // Count cross-subtree pairs
                for (int d : D_S) {
                    // Count elements in A where a + d <= K
                    auto it = A.upper_bound(K - d);
                    pair_count += distance(A.begin(), it);
                }

                // Merge D_S into A
                for (int d : D_S) {
                    A.insert(d);
                }
            }
        }

        // Recurse on each subtree
        for (const Edge& e : adj[centroid]) {
            if (!removed[e.to]) {
                decompose(e.to);
            }
        }
    }

public:
    CentroidDecomposition(int n, int K) : n(n), K(K), adj(n), removed(n, false),
                                          subtree_size(n), pair_count(0) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    long long solve() {
        pair_count = 0;
        decompose(0);
        return pair_count;
    }
};

int main() {
    // Example: 5 nodes, K = 10
    int n = 5;
    int K = 10;

    CentroidDecomposition cd(n, K);

    // Build a tree: 0-1(3), 1-2(2), 1-3(4), 0-4(5)
    cd.addEdge(0, 1, 3);
    cd.addEdge(1, 2, 2);
    cd.addEdge(1, 3, 4);
    cd.addEdge(0, 4, 5);

    long long result = cd.solve();

    cout << "Number of pairs within distance K = " << K << ": " << result << endl;

    return 0;
}