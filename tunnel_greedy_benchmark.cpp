// tunnel_greedy_benchmark.cpp
// Benchmark harness for greedy tunnel packing algorithm
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;
using namespace chrono;

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
            placed++;
            i--;
            j++;
        } else {
            i--;
        }
    }

    return placed;
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> height_dist(1, 10000);

    ofstream outfile("greedy_timings.csv");
    outfile << "n_boxes,m_rooms,time_ms\n";

    // Test sizes
    vector<int> sizes = {100, 500, 1000, 2000, 3000, 4000, 5000,
                         7500, 10000, 15000, 20000, 30000, 40000, 50000};

    int trials = 5;  // Number of trials per size

    for (int size : sizes) {
        cout << "Testing size n = m = " << size << "..." << endl;

        for (int trial = 0; trial < trials; trial++) {
            // Generate random tunnel ceilings
            vector<int> H(size);
            for (int i = 0; i < size; i++) {
                H[i] = height_dist(gen);
            }

            // Generate random crate heights
            vector<int> B(size);
            for (int i = 0; i < size; i++) {
                B[i] = height_dist(gen);
            }

            // Time the algorithm
            auto start = steady_clock::now();

            auto E = computeEffectiveCeiling(H);
            int result = greedyTunnelPacking(B, E);

            auto end = steady_clock::now();
            auto duration = duration_cast<microseconds>(end - start).count();
            double time_ms = duration / 1000.0;

            outfile << size << "," << size << "," << time_ms << "\n";
        }
    }

    outfile.close();
    cout << "Benchmark complete. Results written to greedy_timings.csv" << endl;

    return 0;
}