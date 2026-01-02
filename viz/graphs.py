import matplotlib.pyplot as plt
import pandas as pd
import sys

csv_path = sys.argv[1]

df = pd.read_csv(csv_path)
df["date"] = pd.to_datetime(df["date"])
finals_start = pd.to_datetime("2025-12-08")
finals_end   = pd.to_datetime("2025-12-19")

plt.figure(figsize=(10,4)) # sleep vs hrv

ax = plt.gca()
ax.plot(df["date"], df["hrv"], "ro--", label="HRV")
ax.set_ylabel("HRV")
ax.set_xlabel("Date")

ax2 = ax.twinx()
ax2.plot(df["date"], df["sleep_min"], "bo--", label="Sleep (in min)")
ax2.set_ylabel("Sleep (in min)")

plt.axvspan(finals_start, finals_end, color="red", alpha = 0.15)

hrv_min =df["hrv"].idxmin()
sleep_min = df["sleep_min"].idxmin()

ax.text(df.loc[hrv_min, "date"], df.loc[hrv_min, "hrv"] - 2, "lowest hrv")
ax2.text(df.loc[sleep_min, "date"], df.loc[sleep_min, "sleep_min"] - 20, "Lowest sleep")

plt.title("Sleep duration vs HRV")

lines, labels = ax.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax.legend(lines + lines2, labels+labels2)

plt.tight_layout()
plt.savefig("output/hrv_vs_sleep.png", dpi = 150)
plt.close()

plt.figure(figsize=(10,4)) #hrv vs rhr

ax = plt.gca()
ax.plot(df["date"], df["hrv"], "ro-", label="HRV")
ax.set_ylabel("HRV")
ax.set_xlabel("Date")

ax2 = ax.twinx()
ax2.plot(df["date"], df["rhr"], "bo--", label="Resting HR")
ax2.set_ylabel("Resting HR")

plt.axvspan(finals_start, finals_end, color="red", alpha = 0.15)
hrv_min = df["hrv"].idxmin()
rhr_min = df["rhr"].idxmin()

ax.text(df.loc[hrv_min, "date"], df.loc[hrv_min, "hrv"] - 2, "lowest hrv")

ax2.text(df.loc[rhr_min, "date"], df.loc[rhr_min, "rhr"] + 1, "lowest rhr")

plt.title("Resting Heart Rate vs HRV")

lines, labels = ax.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax.legend(lines + lines2, labels + labels2)

plt.tight_layout()
plt.savefig("output/hrv_vs_rhr.png", dpi=150)
plt.close()


plt.figure(figsize=(12,5)) #compare major metrics

ax = plt.gca()
ax.plot(df["date"], df["hrv"], "ro-", label="HRV")
ax.set_ylabel("HRV")
ax.set_xlabel("Date")
ax2 = ax.twinx()
ax2.plot(df["date"], df["sleep_min"], "b--o", label="Sleep (min)")
ax2.set_ylabel("Sleep (min)")

ax3 = ax.twinx()
ax3.spines["right"].set_position(("axes", 1.1))
ax3.plot(df["date"], df["rhr"], "g:o", label="Resting HR")
ax3.set_ylabel("Resting HR")

plt.axvspan(finals_start, finals_end, color="red", alpha = 0.15)

hrv_min = df["hrv"].idxmin()
ax.text(df.loc[hrv_min, "date"], df.loc[hrv_min, "hrv"] - 2, "lowest hrv")

plt.title("recovery signals during finals week (HRV, Sleep, RHR)")

lines, labels = ax.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
lines3, labels3 = ax3.get_legend_handles_labels()
ax.legend(lines + lines2+ lines3, labels + labels2 + labels3, loc = "upper left")

plt.tight_layout()
plt.savefig("output/all_metrics_overview.png", dpi=150)
plt.close()
