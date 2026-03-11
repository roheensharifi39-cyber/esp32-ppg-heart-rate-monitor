import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("pulse_data2.csv")

# remove fake zeros and unrealistic BPM values
clean = data[(data["bpm"] > 40) & (data["bpm"] < 180)].copy()

print("Average BPM:", clean["bpm"].mean())
print("Max BPM:", clean["bpm"].max())
print("Min BPM:", clean["bpm"].min())

plt.plot(clean["time_s"], clean["bpm"])
plt.title("Heart Rate vs Time")
plt.xlabel("Time (s)")
plt.ylabel("BPM")
plt.grid(True)
plt.show()