// fiber_network_bench.cpp
// Benchmark harness for fiber optic network quality analysis
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <set>

using namespace std;
using namespace chrono;

struct Connection {
    int to, latency_ms;
};

class FiberNetworkAnalyzer {
private:
    int n, max_latency;
    vector<vector<Connection>> network;
    vector<bool> analyzed;
    vector<int> community_size;
    long long acceptable_pairs;

    int calculateSize(int community, int parent) {
        community_size[community] = 1;
        for (const Connection& conn : network[community]) {
            if (conn.to != parent && !analyzed[conn.to]) {
                community_size[community] += calculateSize(conn.to, community);
            }
        }
        return community_size[community];
    }

    int findHub(int community, int parent, int segment_size) {
        for (const Connection& conn : network[community]) {
            if (conn.to != parent && !analyzed[conn.to]) {
                if (community_size[conn.to] > segment_size / 2) {
                    return findHub(conn.to, community, segment_size);
                }
            }
        }
        return community;
    }

    void collectLatencies(int community, int parent, int cumulative_latency,
                         vector<int>& latencies) {
        if (cumulative_latency > max_latency) return;
        latencies.push_back(cumulative_latency);

        for (const Connection& conn : network[community]) {
            if (conn.to != parent && !analyzed[conn.to]) {
                collectLatencies(conn.to, community,
                               cumulative_latency + conn.latency_ms,
                               latencies);
            }
        }
    }

    void analyzeNetwork(int start_community) {
        int segment_size = calculateSize(start_community, -1);
        int hub = findHub(start_community, -1, segment_size);
        analyzed[hub] = true;

        multiset<int> hub_latencies;
        hub_latencies.insert(0);

        for (const Connection& conn : network[hub]) {
            if (!analyzed[conn.to]) {
                vector<int> branch_latencies;
                collectLatencies(conn.to, hub, conn.latency_ms, branch_latencies);
                sort(branch_latencies.begin(), branch_latencies.end());

                for (int latency : branch_latencies) {
                    auto it = hub_latencies.upper_bound(max_latency - latency);
                    acceptable_pairs += distance(hub_latencies.begin(), it);
                }

                for (int latency : branch_latencies) {
                    hub_latencies.insert(latency);
                }
            }
        }

        for (const Connection& conn : network[hub]) {
            if (!analyzed[conn.to]) {
                analyzeNetwork(conn.to);
            }
        }
    }

public:
    FiberNetworkAnalyzer(int num_communities, int max_latency_ms)
        : n(num_communities),
          max_latency(max_latency_ms),
          network(num_communities),
          analyzed(num_communities, false),
          community_size(num_communities),
          acceptable_pairs(0) {}

    void addFiberConnection(int community1, int community2, int latency_ms) {
        network[community1].push_back({community2, latency_ms});
        network[community2].push_back({community1, latency_ms});
    }

    long long countAcceptablePairs() {
        acceptable_pairs = 0;
        fill(analyzed.begin(), analyzed.end(), false);
        if (n > 0) analyzeNetwork(0);
        return acceptable_pairs;
    }
};

// Generate random tree topology (Prüfer sequence method)
void generateRandomFiberNetwork(FiberNetworkAnalyzer& analyzer, int n, mt19937& gen) {
    // Realistic latency distribution for fiber optics: 15ms to 100ms
    uniform_int_distribution<> latency_dist(15, 100);

    // Generate random tree by connecting each node i to a random node in [0, i-1]
    for (int i = 1; i < n; i++) {
        uniform_int_distribution<> parent_dist(0, i - 1);
        int parent = parent_dist(gen);
        int latency = latency_dist(gen);
        analyzer.addFiberConnection(parent, i, latency);
    }
}

int main() {
    random_device rd;
    mt19937 gen(rd());

    ofstream outfile("fiber_network_timings.csv");
    outfile << "n_communities,time_ms\n";

    // Video call quality threshold: 150ms round-trip latency
    int quality_threshold = 150;

    // Test sizes (number of communities in network)
    vector<int> sizes = {100, 200, 500, 1000, 1500, 2000, 3000,
                         4000, 5000, 7500, 10000, 15000, 20000};

    int trials = 5;  // Trials per size

    cout << "========================================" << endl;
    cout << "FIBER OPTIC NETWORK ANALYSIS BENCHMARK" << endl;
    cout << "========================================" << endl;
    cout << "Quality threshold: " << quality_threshold << " ms" << endl;
    cout << "Latency range: 15-100 ms per connection" << endl;
    cout << "Trials per size: " << trials << endl;
    cout << endl;

    for (int size : sizes) {
        cout << "Testing network size: " << size << " communities..." << flush;

        for (int trial = 0; trial < trials; trial++) {
            FiberNetworkAnalyzer analyzer(size, quality_threshold);
            generateRandomFiberNetwork(analyzer, size, gen);

            auto start = steady_clock::now();
            long long result = analyzer.countAcceptablePairs();
            auto end = steady_clock::now();

            auto duration = duration_cast<microseconds>(end - start).count();
            double time_ms = duration / 1000.0;

            outfile << size << "," << time_ms << "\n";
        }

        cout << " Done." << endl;
    }

    outfile.close();

    cout << endl;
    cout << "========================================" << endl;
    cout << "Benchmark complete!" << endl;
    cout << "Results written to: fiber_network_timings.csv" << endl;
    cout << "========================================" << endl;

    return 0;
}