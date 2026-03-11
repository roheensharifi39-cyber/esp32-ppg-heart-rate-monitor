import pandas as pd
from sklearn.ensemble import IsolationForest

data = pd.read_csv("pulse_data2.csv")

# remove bad BPM rows
clean = data[(data["bpm"] > 40) & (data["bpm"] < 180)]

features = clean[["bpm"]]

model = IsolationForest(contamination=0.05)
model.fit(features)

predictions = model.predict(features)

clean["anomaly"] = predictions

print(clean.head())