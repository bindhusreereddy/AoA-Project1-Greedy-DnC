#!/usr/bin/env python3
"""
plot_graphs.py
Reads CSV files from benchmark harnesses and generates runtime plots
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_greedy_timings():
    """Plot greedy algorithm runtime"""
    try:
        df = pd.read_csv('greedy_timings.csv')

        # Group by size and compute mean
        grouped = df.groupby('n_boxes')['time_ms'].mean().reset_index()
        n = grouped['n_boxes'].values
        time = grouped['time_ms'].values

        plt.figure(figsize=(10, 6))
        plt.plot(n, time, 'bo-', linewidth=2, markersize=6, label='Observed Runtime')

        # Fit O(n log n) curve for comparison
        if len(n) > 1:
            # Find coefficient c such that c * n * log(n) fits the data
            nlogn = n * np.log(n)
            c = np.median(time / nlogn)
            theoretical = c * nlogn
            plt.plot(n, theoretical, 'r--', linewidth=2, alpha=0.7,
                     label=f'O(n log n) fit (c={c:.4f})')

        plt.xlabel('Input Size (n = number of crates = tunnel segments)', fontsize=12)
        plt.ylabel('Runtime (milliseconds)', fontsize=12)
        plt.title('Greedy Tunnel Packing: Empirical Runtime Analysis', fontsize=14, fontweight='bold')
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('greedy_runtime.png', dpi=300, bbox_inches='tight')
        print("Saved greedy_runtime.png")
        plt.close()

    except FileNotFoundError:
        print("Error: greedy_timings.csv not found")
    except Exception as e:
        print(f"Error plotting greedy timings: {e}")

def plot_centroid_timings():
    """Plot centroid decomposition runtime"""
    try:
        df = pd.read_csv('centroid_timings.csv')

        # Group by size and compute mean
        grouped = df.groupby('n_nodes')['time_ms'].mean().reset_index()
        n = grouped['n_nodes'].values
        time = grouped['time_ms'].values

        plt.figure(figsize=(10, 6))
        plt.plot(n, time, 'go-', linewidth=2, markersize=6, label='Observed Runtime')

        # Fit O(n log n) curve
        if len(n) > 1:
            nlogn = n * np.log(n)
            c = np.median(time / nlogn)
            theoretical = c * nlogn
            plt.plot(n, theoretical, 'r--', linewidth=2, alpha=0.7,
                     label=f'O(n log n) fit (c={c:.4f})')

        plt.xlabel('Number of Nodes (n)', fontsize=12)
        plt.ylabel('Runtime (milliseconds)', fontsize=12)
        plt.title('Centroid Decomposition: Empirical Runtime Analysis', fontsize=14, fontweight='bold')
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('centroid_runtime.png', dpi=300, bbox_inches='tight')
        print("Saved centroid_runtime.png")
        plt.close()

    except FileNotFoundError:
        print("Error: centroid_timings.csv not found")
    except Exception as e:
        print(f"Error plotting centroid timings: {e}")

def main():
    print("Generating runtime plots...")
    plot_greedy_timings()
    plot_centroid_timings()
    print("Done!")

if __name__ == "__main__":
    main()