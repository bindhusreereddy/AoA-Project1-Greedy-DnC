// fiber_network_analysis.cpp
// Real-world problem: Rural Fiber Optic Network Quality Analysis
// A telecommunications company deployed fiber-optic connections in a rural region
// Need to count pairs of communities that can have video calls with acceptable latency

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

struct Connection {
    int to;
    int latency_ms;  // Fiber connection latency in milliseconds
};

class FiberNetworkAnalyzer {
private:
    int n;  // Number of communities
    int max_acceptable_latency;  // Maximum latency for acceptable video call quality
    vector<vector<Connection>> network;
    vector<bool> analyzed;
    vector<int> community_size;
    long long acceptable_pairs;

    // Calculate community sizes in current network segment
    int calculateSize(int community, int parent) {
        community_size[community] = 1;
        for (const Connection& conn : network[community]) {
            if (conn.to != parent && !analyzed[conn.to]) {
                community_size[community] += calculateSize(conn.to, community);
            }
        }
        return community_size[community];
    }

    // Find network hub (centroid) for analysis
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

    // Collect latencies from hub to all communities in branch
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

    // Recursive network analysis using divide-and-conquer
    void analyzeNetwork(int start_community) {
        int segment_size = calculateSize(start_community, -1);
        int hub = findHub(start_community, -1, segment_size);
        analyzed[hub] = true;

        // Accumulator for latencies from hub
        multiset<int> hub_latencies;
        hub_latencies.insert(0);  // Hub can connect to itself (latency 0)

        // Process each branch from the hub
        for (const Connection& conn : network[hub]) {
            if (!analyzed[conn.to]) {
                vector<int> branch_latencies;
                collectLatencies(conn.to, hub, conn.latency_ms, branch_latencies);
                sort(branch_latencies.begin(), branch_latencies.end());

                // Count pairs between current branch and previous branches
                for (int latency : branch_latencies) {
                    auto it = hub_latencies.upper_bound(max_acceptable_latency - latency);
                    acceptable_pairs += distance(hub_latencies.begin(), it);
                }

                // Add current branch latencies to accumulator
                for (int latency : branch_latencies) {
                    hub_latencies.insert(latency);
                }
            }
        }

        // Recursively analyze remaining network segments
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

    void printNetworkInfo() {
        cout << "========================================" << endl;
        cout << "FIBER OPTIC NETWORK ANALYSIS" << endl;
        cout << "========================================" << endl;
        cout << "Communities connected: " << n << endl;
        cout << "Max acceptable latency: " << max_acceptable_latency << " ms" << endl;
        cout << "Network topology: Tree (no redundant paths)" << endl;
        cout << endl;
    }
};

int main() {
    cout << "\n=== RURAL FIBER OPTIC NETWORK QUALITY ASSESSMENT ===\n" << endl;

    cout << "SCENARIO:" << endl;
    cout << "A telecommunications company deployed fiber-optic internet to" << endl;
    cout << "15 rural communities. Due to mountainous terrain and limited" << endl;
    cout << "budget, they created a tree topology (no redundant connections)." << endl;
    cout << "Each fiber link has different latency based on distance and" << endl;
    cout << "number of optical repeaters needed." << endl;
    cout << endl;

    cout << "BUSINESS QUESTION:" << endl;
    cout << "For quality assurance, the company needs to know: How many" << endl;
    cout << "pairs of communities can have acceptable video call quality?" << endl;
    cout << "(Video calls require round-trip latency ≤ 150ms)" << endl;
    cout << endl;

    // Example network: 15 communities
    int num_communities = 15;
    int max_latency = 150;  // milliseconds for acceptable video quality

    FiberNetworkAnalyzer analyzer(num_communities, max_latency);
    analyzer.printNetworkInfo();

    // Build realistic fiber network topology
    // Central hub: Community 0 (small town with main data center)
    cout << "NETWORK TOPOLOGY:" << endl;
    cout << "Building tree-structured fiber network..." << endl;

    // Main trunk lines from hub
    analyzer.addFiberConnection(0, 1, 25);   // Nearby village
    analyzer.addFiberConnection(0, 2, 40);   // Medium distance
    analyzer.addFiberConnection(0, 3, 35);   // Medium distance

    // Branch connections from node 1
    analyzer.addFiberConnection(1, 4, 30);   // Mountain community
    analyzer.addFiberConnection(1, 5, 20);   // Valley community

    // Branch connections from node 2
    analyzer.addFiberConnection(2, 6, 45);   // Remote farming area
    analyzer.addFiberConnection(2, 7, 35);   // Mining town
    analyzer.addFiberConnection(2, 8, 40);   // Coastal village

    // Branch connections from node 3
    analyzer.addFiberConnection(3, 9, 30);   // Highland settlement
    analyzer.addFiberConnection(3, 10, 25);  // Forest community

    // Extended branches (distant communities)
    analyzer.addFiberConnection(6, 11, 50);  // Very remote ranch
    analyzer.addFiberConnection(7, 12, 45);  // Mountain resort
    analyzer.addFiberConnection(9, 13, 35);  // Border town
    analyzer.addFiberConnection(10, 14, 30); // Lake community

    cout << "Network connections established." << endl;
    cout << endl;

    // Analyze network
    cout << "ANALYZING VIDEO CALL QUALITY..." << endl;
    long long result = analyzer.countAcceptablePairs();

    cout << "========================================" << endl;
    cout << "RESULTS:" << endl;
    cout << "========================================" << endl;
    cout << "Total community pairs: " << (num_communities * (num_communities - 1)) / 2 << endl;
    cout << "Pairs with acceptable latency (≤" << max_latency << "ms): " << result << endl;
    cout << "Pairs with poor latency (>" << max_latency << "ms): "
         << ((num_communities * (num_communities - 1)) / 2 - result) << endl;
    cout << endl;

    double percentage = (100.0 * result) / ((num_communities * (num_communities - 1)) / 2);
    cout << "Quality Coverage: " << percentage << "%" << endl;
    cout << endl;

    cout << "BUSINESS INTERPRETATION:" << endl;
    if (percentage >= 80) {
        cout << "✓ EXCELLENT: Most community pairs can video conference reliably." << endl;
        cout << "  The network topology is well-designed for the region." << endl;
    } else if (percentage >= 60) {
        cout << "⚠ GOOD: Majority of pairs have acceptable quality." << endl;
        cout << "  Consider adding fiber links to reduce latency bottlenecks." << endl;
    } else {
        cout << "✗ NEEDS IMPROVEMENT: Many pairs have excessive latency." << endl;
        cout << "  Network redesign or additional trunk lines recommended." << endl;
    }
    cout << endl;

    cout << "TECHNICAL ANALYSIS METHOD:" << endl;
    cout << "Used centroid decomposition (divide-and-conquer) to efficiently" << endl;
    cout << "count pairs without computing all-pairs latencies explicitly." << endl;
    cout << "Time complexity: O(n log n) where n = " << num_communities << " communities" << endl;
    cout << "This is much faster than naive O(n²) all-pairs approach." << endl;
    cout << endl;

    return 0;
}