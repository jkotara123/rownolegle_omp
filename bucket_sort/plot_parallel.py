import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("parallel_results_sort_1.csv")

grouped = df.groupby(["N", "threads"]).agg(
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

Ns = sorted(grouped["N"].unique())

for N in Ns:

    sub = grouped[grouped["N"] == N]

    # ==================================
    # TOTAL TIME
    # ==================================

    plt.figure(figsize=(10,6))

    plt.errorbar(
        sub["threads"],
        sub["total_mean"],
        yerr=sub["total_std"],
        marker='o',
        capsize=4,
        label="total"
    )

    plt.xlabel("Threads")
    plt.ylabel("Time [s]")
    plt.title(f"Parallel bucket sort: total time vs threads (N={N})")
    plt.grid(True)
    plt.legend()

    plt.savefig(f"par_{N}_total.png", dpi=300)
    plt.close()

    # ==================================
    # PARTS
    # ==================================

    plt.figure(figsize=(10,6))

    for part in ["dist", "sort", "merge"]:
        plt.errorbar(
            sub["threads"],
            sub[f"{part}_mean"],
            yerr=sub[f"{part}_std"],
            marker='o',
            capsize=4,
            label=part
        )

    plt.xlabel("Threads")
    plt.ylabel("Time [s]")
    plt.title(f"Parallel bucket sort: parts vs threads (N={N})")
    plt.grid(True)
    plt.legend()

    plt.savefig(f"par_{N}_parts.png", dpi=300)
    plt.close()

    # ==================================
    # SPEEDUP
    # ==================================

    baseline = sub[sub["threads"] == 1].iloc[0]

    plt.figure(figsize=(10,6))

    for part in ["dist", "sort", "merge", "total"]:

        speedup = baseline[f"{part}_mean"] / sub[f"{part}_mean"]

        plt.plot(
            sub["threads"],
            speedup,
            marker='o',
            label=part
        )

    # ideal
    plt.plot(
        sub["threads"],
        sub["threads"],
        linestyle="--",
        label="ideal"
    )

    plt.xlabel("Threads")
    plt.ylabel("Speedup")
    plt.title(f"Parallel bucket sort: speedup (N={N})")
    plt.grid(True)
    plt.legend()

    plt.savefig(f"par_{N}_speedup.png", dpi=300)
    plt.close()