import pandas as pd

df = pd.read_csv("results.csv", sep=";")

schedule_map = {0: "static", 1: "dynamic", 2: "guided"}
df["schedule"] = df["schedule_type"].map(schedule_map)

df["time"] = df["time"].round(6)

table = df.pivot_table(
    index=["schedule", "chunk_size"],
    columns="N",
    values="time"
).reset_index()

table.columns = ["schedule", "chunk_size", "1e7", "1e9"]

print("\n=== TABELA ===\n")
print(table)

print("\n=== LATEX ===\n")
print(table.to_latex(index=False, float_format="%.6f"))
