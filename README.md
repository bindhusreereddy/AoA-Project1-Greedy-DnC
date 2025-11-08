# Greedy and Divide-and-Conquer Approaches for Real-World Optimization

**Course**: Analysis of Algorithms (Fall 2025)  
**Institution**: University of Florida  
**Authors**: Bindhu Sree Reddy Alla, Parvathi Nalla

---

## 📋 Project Overview

This project demonstrates the practical application of classical algorithmic paradigms—**greedy algorithms** and **divide-and-conquer**—to solve two real-world optimization problems:

1. **Disaster Relief Tunnel Packing** (Greedy Algorithm)
2. **Fiber Optic Network Quality Assessment** (Divide-and-Conquer)

Both algorithms achieve **O(n log n)** time complexity and are validated through comprehensive experimental analysis.

---

## 🚨 Problem 1: Disaster Relief Tunnel Packing (Greedy)

### Real-World Scenario

Following severe flooding and landslides in a mountainous region, a humanitarian relief agency must rapidly stage emergency supplies in a narrow mountain tunnel—the only accessible secure storage location. The challenge is to maximize the number of supply crates that can be safely stored given physical and safety constraints.

### Problem Constraints

- **Tunnel Structure**: Consecutive segments from entrance to interior, each with varying ceiling heights
- **Crate Movement**: Crates enter from a single entrance and move forward only
- **Height Restriction**: A crate can only pass through segments where ceiling height ≥ crate height
- **Safety Regulation**: Maximum one crate per segment
- **Objective**: Maximize the total number of crates staged

### Solution Approach

**Algorithm**: Greedy placement strategy
- **Step 1**: Compute effective ceiling heights (minimum clearance to reach each position)
- **Step 2**: Sort crates by height (smallest first)
- **Step 3**: Use two-pointer technique to place smallest crates in deepest positions
- **Key Insight**: Small crates can reach deep positions that tall crates cannot access

**Time Complexity**: O(n log n) — dominated by sorting  
**Space Complexity**: O(m) where m = number of tunnel segments

### Why Greedy Works

The algorithm is proven optimal through an exchange argument: placing smaller crates deeper always allows for at least as many placements as any other strategy, since smaller crates can fit in more restrictive positions.

---

## 🌐 Problem 2: Fiber Optic Network Quality Assessment (Divide-and-Conquer)

### Real-World Scenario

A telecommunications provider deployed fiber-optic broadband infrastructure across rural mountainous communities using a **tree topology** (no redundant links) to minimize costs. The company needs to efficiently assess how many community pairs can support high-quality video calls (latency ≤ 150ms) for:

- Marketing claims and service level agreements (SLAs)
- Infrastructure investment planning
- Service tier pricing justification

### Problem Constraints

- **Network Structure**: Tree topology connecting n communities
- **Link Latency**: Each fiber segment has measured propagation delay (15-100ms)
- **Path Latency**: Sum of all link latencies on the unique path between two communities
- **Quality Threshold**: Video calls require latency ≤ 150ms (ITU-T G.114 standard)
- **Objective**: Count all community pairs meeting the quality threshold

### Solution Approach

**Algorithm**: Centroid Decomposition (divide-and-conquer on trees)
- **Step 1**: Find centroid node (whose removal creates balanced subtrees ≤ n/2)
- **Step 2**: Measure latencies from centroid to all reachable nodes
- **Step 3**: Count valid pairs crossing through centroid using sorted matching
- **Step 4**: Recursively process each subtree independently
- **Key Insight**: Every path crosses through some centroid in the recursion tree

**Time Complexity**: O(n log n)
- Recursion depth: O(log n) due to balanced splits
- Work per level: O(n log n) for DFS, sorting, and binary search

**Space Complexity**: O(n)

### Why Divide-and-Conquer Works

The algorithm is proven correct by showing:
1. **Completeness**: Every valid pair crosses through exactly one centroid (the first to separate them)
2. **No double-counting**: After processing a centroid, its endpoints never meet again in recursion

This avoids the naive O(n²) approach of testing all pairs individually.

---

## 🔬 Experimental Validation

Both algorithms were implemented in C++ and benchmarked with varying input sizes:

### Tunnel Packing (Greedy)
- **Test Range**: n = 100 to 50,000 (tunnel segments and crates)
- **Observation**: Runtime closely matches O(n log n) theoretical prediction
- **Result**: Sorting dominates execution time as expected

### Fiber Network Analysis (Divide-and-Conquer)
- **Test Range**: n = 100 to 20,000 communities
- **Observation**: Runtime exhibits clear O(n log n) behavior
- **Result**: Validates centroid decomposition efficiency for practical deployment

See `plots/` directory for experimental graphs comparing observed vs. theoretical runtimes.


## 🚀 How to Run

### Prerequisites
- **C++ Compiler**: g++ 11.4.0 or later (C++17 support)
- **Python**: 3.10+ (for plotting)
- **Libraries**: matplotlib, pandas, numpy

### Compilation

```bash
# Compile Tunnel Packing (Greedy)
g++ -O2 -std=c++17 src/tunnel_greedy.cpp -o tunnel_greedy

# Compile Fiber Network Analysis (Divide-and-Conquer)
g++ -O2 -std=c++17 src/fiber_network_analysis.cpp -o fiber_network

# Compile Benchmarks
g++ -O2 -std=c++17 src/tunnel_greedy_benchmark.cpp -o tunnel_benchmark
g++ -O2 -std=c++17 src/fiber_network_benchmark.cpp -o fiber_benchmark
```

### Execution

```bash
# Run algorithms with example inputs
./tunnel_greedy
./fiber_network

# Run benchmarks (outputs CSV data)
./tunnel_benchmark > data/tunnel_results.csv
./fiber_benchmark > data/fiber_results.csv

# Generate plots
python3 plots/generate_plots.py
```

---

## 📊 Key Results

| Algorithm | Problem Size | Time Complexity | Space Complexity | Experimental Validation |
|-----------|--------------|-----------------|------------------|------------------------|
| Greedy (Tunnel) | n ≈ 50,000 | O(n log n) | O(n) | ✅ Confirmed |
| Centroid Decomposition (Fiber) | n ≈ 20,000 | O(n log n) | O(n) | ✅ Confirmed |

Both implementations scale efficiently and match theoretical predictions, making them suitable for production deployment.

---

## 📄 Academic Paper

The complete analysis, proofs, and detailed explanations are available in the academic paper:

**Title**: *Greedy and Divide-and-Conquer Approaches for Real-World Optimization: Tunnel Loading and Fiber Network Quality Analysis*

See `report/main.tex` for the full LaTeX source following ACM conference format.

---

## 🎯 Learning Outcomes

This project demonstrates:

1. **Algorithm Design**: How classical paradigms solve modern problems
2. **Proof Techniques**: Exchange arguments (greedy) and recursive invariants (divide-and-conquer)
3. **Complexity Analysis**: Rigorous time and space analysis with experimental validation
4. **Domain Translation**: Bridging computer science and real-world applications
5. **Implementation Skills**: Efficient C++ implementations with STL data structures

---

## 👥 Authors

- **Bindhu Sree Reddy Alla** - [bindhusreer.alla@ufl.edu](mailto:bindhusreer.alla@ufl.edu)
- **Parvathi Nalla** - [parvathinalla@ufl.edu](mailto:parvathinalla@ufl.edu)

---

## 📝 License

This project is submitted as coursework for Analysis of Algorithms (Fall 2025) at the University of Florida.

---

## 🙏 Acknowledgments

- Course Instructor: [Instructor Name]
- LLM Assistance: Documented in `report/llm_disclosure.tex`
- Template: ACM Conference Proceedings Format

---

## 📚 References

- ITU-T G.114: One-way transmission time recommendations
- Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.)
- Centroid Decomposition: Classical divide-and-conquer technique for tree problems

---

**Last Updated**: November 2025