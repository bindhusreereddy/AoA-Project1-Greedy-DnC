// fiber_network_bench.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono>
#include <random>

using namespace std;

struct Connection {
    int to;
    int latency_ms;
};

class FiberNetworkAnalyzer {
private:
    int n;
    int max_acceptable_latency;
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
        if (cumulative_latency > max_acceptable_latency) return;
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
                    auto it = hub_latencies.upper_bound(max_acceptable_latency - latency);
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
    FiberNetworkAnalyzer(int num_communities, int max_latency)
        : n(num_communities),
          max_acceptable_latency(max_latency),
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <size>" << endl;
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(10, 100);
    
    FiberNetworkAnalyzer analyzer(n, 150);
    
    // Build random tree
    for (int i = 1; i < n; i++) {
        int parent = uniform_int_distribution<>(0, i-1)(gen);
        int latency = dis(gen);
        analyzer.addFiberConnection(parent, i, latency);
    }
    
    auto start = chrono::high_resolution_clock::now();
    long long result = analyzer.countAcceptablePairs();
    auto end = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << duration.count() / 1000.0 << endl;
    
    return 0;
}
