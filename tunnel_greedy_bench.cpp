// tunnel_greedy_bench.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

vector<int> computeEffectiveCeiling(const vector<int>& H) {
    int m = H.size();
    vector<int> E(m);
    E[0] = H[0];
    for (int i = 1; i < m; i++) {
        E[i] = min(E[i-1], H[i]);
    }
    return E;
}

int greedyTunnelPacking(vector<int>& B, const vector<int>& E) {
    int n = B.size();
    int m = E.size();
    sort(B.begin(), B.end());
    int placed = 0;
    int i = m - 1;
    int j = 0;
    while (i >= 0 && j < n) {
        if (B[j] <= E[i]) {
            placed++;
            i--;
            j++;
        } else {
            i--;
        }
    }
    return placed;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <size>" << endl;
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    // Generate random test data
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000);
    
    vector<int> H(n), B(n);
    for (int i = 0; i < n; i++) {
        H[i] = dis(gen);
        B[i] = dis(gen);
    }
    
    auto start = chrono::high_resolution_clock::now();
    auto E = computeEffectiveCeiling(H);
    int result = greedyTunnelPacking(B, E);
    auto end = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << duration.count() / 1000.0 << endl;
    
    return 0;
}
