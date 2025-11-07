#!/usr/bin/env python3
"""
benchmark_plot.py
Benchmarks C++ programs and generates runtime plots
"""

import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import random

def compile_cpp_files():
    """Compile the C++ programs"""
    print("Compiling C++ programs...")

    programs = [
        ('tunnel_greedy.cpp', 'tunnel_greedy'),
        ('fiber_network_analysis.cpp', 'fiber_network')
    ]

    for source, executable in programs:
        if not os.path.exists(source):
            print(f"Warning: {source} not found!")
            continue

        cmd = ['g++', '-std=c++17', '-O2', source, '-o', executable]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode == 0:
            print(f"✓ Compiled {source} -> {executable}")
        else:
            print(f"✗ Failed to compile {source}")
            print(result.stderr)

    print()

def create_tunnel_benchmark_cpp():
    """Create a benchmarkable version of tunnel_greedy.cpp"""
    benchmark_code = """// tunnel_greedy_bench.cpp
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
"""

    with open('tunnel_greedy_bench.cpp', 'w') as f:
        f.write(benchmark_code)

    print("Created tunnel_greedy_bench.cpp")

def create_fiber_benchmark_cpp():
    """Create a benchmarkable version of fiber_network_analysis.cpp"""
    benchmark_code = """// fiber_network_bench.cpp
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
"""

    with open('fiber_network_bench.cpp', 'w') as f:
        f.write(benchmark_code)

    print("Created fiber_network_bench.cpp")

def benchmark_program(executable, sizes, trials=5):
    """Run benchmark for a program with different input sizes"""
    results = []

    for size in sizes:
        times = []
        for _ in range(trials):
            try:
                result = subprocess.run(
                    [f'./{executable}', str(size)],
                    capture_output=True,
                    text=True,
                    timeout=10
                )
                if result.returncode == 0:
                    runtime = float(result.stdout.strip())
                    times.append(runtime)
            except (subprocess.TimeoutExpired, ValueError):
                continue

        if times:
            avg_time = sum(times) / len(times)
            results.append((size, avg_time))
            print(f"  n={size:5d}: {avg_time:8.3f} ms (avg of {len(times)} runs)")

    return results

def plot_results(tunnel_results, fiber_results):
    """Create comparison plots"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    # Plot 1: Tunnel Greedy
    if tunnel_results:
        sizes = [r[0] for r in tunnel_results]
        times = [r[1] for r in tunnel_results]

        ax1.plot(sizes, times, 'bo-', linewidth=2, markersize=8,
                 label='Observed Runtime', alpha=0.7)

        # Fit O(n log n)
        n = np.array(sizes)
        t = np.array(times)
        nlogn = n * np.log(n)
        c = np.median(t / nlogn)
        theoretical = c * nlogn

        ax1.plot(sizes, theoretical, 'r--', linewidth=2, alpha=0.7,
                 label=f'O(n log n) fit (c={c:.6f})')

        ax1.set_xlabel('Input Size (n)', fontsize=13, fontweight='bold')
        ax1.set_ylabel('Runtime (milliseconds)', fontsize=13, fontweight='bold')
        ax1.set_title('Tunnel Greedy Algorithm\nEmpirical Runtime Analysis',
                      fontsize=14, fontweight='bold')
        ax1.legend(fontsize=11)
        ax1.grid(True, alpha=0.3, linestyle='--')

    # Plot 2: Fiber Network (Centroid Decomposition)
    if fiber_results:
        sizes = [r[0] for r in fiber_results]
        times = [r[1] for r in fiber_results]

        ax2.plot(sizes, times, 'go-', linewidth=2, markersize=8,
                 label='Observed Runtime', alpha=0.7)

        # Fit O(n log n)
        n = np.array(sizes)
        t = np.array(times)
        nlogn = n * np.log(n)
        c = np.median(t / nlogn)
        theoretical = c * nlogn

        ax2.plot(sizes, theoretical, 'r--', linewidth=2, alpha=0.7,
                 label=f'O(n log n) fit (c={c:.6f})')

        ax2.set_xlabel('Number of Nodes (n)', fontsize=13, fontweight='bold')
        ax2.set_ylabel('Runtime (milliseconds)', fontsize=13, fontweight='bold')
        ax2.set_title('Fiber Network Analysis (Centroid Decomposition)\nEmpirical Runtime Analysis',
                      fontsize=14, fontweight='bold')
        ax2.legend(fontsize=11)
        ax2.grid(True, alpha=0.3, linestyle='--')

    plt.tight_layout()
    plt.savefig('combined_runtime_analysis.png', dpi=300, bbox_inches='tight')
    print("\n✓ Saved combined_runtime_analysis.png")
    plt.close()

    # Create individual plots too
    if tunnel_results:
        plt.figure(figsize=(10, 6))
        sizes = [r[0] for r in tunnel_results]
        times = [r[1] for r in tunnel_results]
        plt.plot(sizes, times, 'bo-', linewidth=2, markersize=8)
        n = np.array(sizes)
        t = np.array(times)
        nlogn = n * np.log(n)
        c = np.median(t / nlogn)
        plt.plot(sizes, c * nlogn, 'r--', linewidth=2, alpha=0.7,
                 label=f'O(n log n) fit')
        plt.xlabel('Input Size (n)', fontsize=12)
        plt.ylabel('Runtime (milliseconds)', fontsize=12)
        plt.title('Tunnel Greedy Algorithm - Runtime Analysis', fontsize=14, fontweight='bold')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('tunnel_greedy_runtime.png', dpi=300, bbox_inches='tight')
        print("✓ Saved tunnel_greedy_runtime.png")
        plt.close()

    if fiber_results:
        plt.figure(figsize=(10, 6))
        sizes = [r[0] for r in fiber_results]
        times = [r[1] for r in fiber_results]
        plt.plot(sizes, times, 'go-', linewidth=2, markersize=8)
        n = np.array(sizes)
        t = np.array(times)
        nlogn = n * np.log(n)
        c = np.median(t / nlogn)
        plt.plot(sizes, c * nlogn, 'r--', linewidth=2, alpha=0.7,
                 label=f'O(n log n) fit')
        plt.xlabel('Number of Nodes (n)', fontsize=12)
        plt.ylabel('Runtime (milliseconds)', fontsize=12)
        plt.title('Fiber Network Analysis - Runtime Analysis', fontsize=14, fontweight='bold')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('fiber_network_runtime.png', dpi=300, bbox_inches='tight')
        print("✓ Saved fiber_network_runtime.png")
        plt.close()

def main():
    print("=" * 60)
    print("BENCHMARK AND PLOT C++ ALGORITHMS")
    print("=" * 60)
    print()

    # Create benchmarkable versions
    create_tunnel_benchmark_cpp()
    create_fiber_benchmark_cpp()
    print()

    # Compile
    print("Compiling benchmark programs...")
    subprocess.run(['g++', '-std=c++17', '-O2', 'tunnel_greedy_bench.cpp',
                    '-o', 'tunnel_bench'], capture_output=True)
    subprocess.run(['g++', '-std=c++17', '-O2', 'fiber_network_bench.cpp',
                    '-o', 'fiber_bench'], capture_output=True)
    print("✓ Compilation complete\n")

    # Define test sizes
    tunnel_sizes = [100, 500, 1000, 2500, 5000, 7500, 10000]
    fiber_sizes = [50, 100, 250, 500, 1000, 2000, 3000]

    # Run benchmarks
    print("Benchmarking Tunnel Greedy Algorithm...")
    tunnel_results = benchmark_program('tunnel_bench', tunnel_sizes, trials=3)
    print()

    print("Benchmarking Fiber Network Analysis...")
    fiber_results = benchmark_program('fiber_bench', fiber_sizes, trials=3)
    print()

    # Generate plots
    print("Generating plots...")
    plot_results(tunnel_results, fiber_results)

    print("\n" + "=" * 60)
    print("BENCHMARK COMPLETE!")
    print("=" * 60)
    print("\nGenerated files:")
    print("  • combined_runtime_analysis.png (side-by-side comparison)")
    print("  • tunnel_greedy_runtime.png (individual)")
    print("  • fiber_network_runtime.png (individual)")

if __name__ == "__main__":
    main()