import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("sequential_results_sort_1.csv")

grouped = df.groupby(["bins"]).agg(
    gen_mean=("gen", "mean"),
    gen_std=("gen", "std"),

    dist_mean=("dist", "mean"),
    dist_std=("dist", "std"),

    sort_mean=("sort", "mean"),
    sort_std=("sort", "std"),

    merge_mean=("merge", "mean"),
    merge_std=("merge", "std"),

    total_mean=("total", "mean"),
    total_std=("total", "std"),
).reset_index()

# =========================
# 1. TOTAL vs BINS
# =========================

plt.figure(figsize=(10,6))

plt.errorbar(
    grouped["bins"],
    grouped["total_mean"],
    yerr=grouped["total_std"],
    marker='o',
    capsize=4,
    label="total"
)

plt.xlabel("Bins")
plt.ylabel("Time [s]")
plt.title("Sequential bucket sort: total time vs bins")
plt.grid(True)
plt.legend()

plt.savefig("seq_total_vs_bins.png", dpi=300)
plt.close()

# =========================
# 2. PARTS vs BINS
# =========================

plt.figure(figsize=(10,6))

for part in ["dist", "sort", "merge"]:
    plt.errorbar(
        grouped["bins"],
        grouped[f"{part}_mean"],
        yerr=grouped[f"{part}_std"],
        marker='o',
        capsize=4,
        label=part
    )

plt.xlabel("Bins")
plt.ylabel("Time [s]")
plt.title("Sequential bucket sort: parts vs bins")
plt.grid(True)
plt.legend()

plt.savefig("seq_parts_vs_bins.png", dpi=300)
plt.close()