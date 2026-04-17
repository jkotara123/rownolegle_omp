import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv", sep=";")

schedule_map = {0: "static", 1: "dynamic", 2: "guided"}
df["schedule"] = df["schedule_type"].map(schedule_map)

df["time"] = df["time"].round(6)

df = df.sort_values(by=["N", "schedule", "chunk_size"])

for n_value in sorted(df["N"].unique()):
    subset = df[df["N"] == n_value]

    pivot = subset.pivot(index="chunk_size", columns="schedule", values="time")
    pivot = pivot[["static", "dynamic", "guided"]]

    ax = pivot.plot(kind="bar", figsize=(8, 5))

    ax.set_xlabel("Chunk size")
    ax.set_ylabel("Execution time [s]")
    ax.set_title(f"Comparison of scheduling strategies (N = {n_value})")
    ax.grid(True, axis="y")

    plt.tight_layout()
    plt.savefig(f"plot_N_{n_value}.png", dpi=300)
    plt.close()

print("2 bar plots have been saved.")
