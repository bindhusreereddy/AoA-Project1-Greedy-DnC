// tunnel_greedy.cpp
// Greedy algorithm for tunnel loading problem
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

// Compute prefix minimum (effective ceiling) array
vector<int> computeEffectiveCeiling(const vector<int>& H) {
    int m = H.size();
    vector<int> E(m);
    E[0] = H[0];
    for (int i = 1; i < m; i++) {
        E[i] = min(E[i-1], H[i]);
    }
    return E;
}

// Greedy algorithm to place maximum number of crates
int greedyTunnelPacking(vector<int>& B, const vector<int>& E) {
    int n = B.size();
    int m = E.size();

    // Sort crates by height (non-decreasing)
    sort(B.begin(), B.end());

    int placed = 0;
    int i = m - 1;  // Start from deepest tunnel segment
    int j = 0;      // Start from smallest crate

    while (i >= 0 && j < n) {
        if (B[j] <= E[i]) {
            // Crate j fits in segment i
            placed++;
            i--;
            j++;
        } else {
            // Crate too tall, try shallower segment
            i--;
        }
    }

    return placed;
}

// Main function with example
int main() {
    // Example from paper
    vector<int> H = {5, 4, 3, 2};
    vector<int> B = {1, 2, 3, 4, 5};

    auto E = computeEffectiveCeiling(H);

    cout << "Tunnel ceilings: ";
    for (int h : H) cout << h << " ";
    cout << "\nEffective ceilings: ";
    for (int e : E) cout << e << " ";
    cout << "\nCrate heights: ";
    for (int b : B) cout << b << " ";

    int result = greedyTunnelPacking(B, E);

    cout << "\nMaximum crates placed: " << result << endl;

    return 0;
}