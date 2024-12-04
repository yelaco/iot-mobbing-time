from sklearn.preprocessing import StandardScaler
import pandas as pd
from sklearn.ensemble import IsolationForest
import matplotlib.pyplot as plt
import random

# Simulate bin data
data = {
    "timestamp": pd.date_range(start="2024-01-01", periods=1000, freq="h"),
    "fill_level": [random.randint(0, 100) for _ in range(1000)],
    "temperature": [random.uniform(15, 35) for _ in range(1000)],
}

df = pd.DataFrame(data)
df.to_csv("bin_data.csv", index=False)

# Load data
df = pd.read_csv("bin_data.csv")

# Convert timestamps
df["timestamp"] = pd.to_datetime(df["timestamp"])

# Optional: Add derived features
df["fill_rate"] = df["fill_level"].diff().fillna(0)  # Rate of change
df["hour"] = df["timestamp"].dt.hour  # Time of day feature

# Scale features
scaler = StandardScaler()
features = ["fill_level", "fill_rate", "temperature"]
df[features] = scaler.fit_transform(df[features])

# Train Isolation Forest
model = IsolationForest(n_estimators=100, random_state=42)
df["anomaly_score"] = model.fit_predict(df[features])

# Add anomaly label
df["anomaly"] = df["anomaly_score"].apply(lambda x: 1 if x == -1 else 0)

# Simulate incoming data
new_data = [[60, 0.5, 25]]  # Example: Fill level, fill rate, temperature
new_anomaly = model.predict(new_data)
print("Anomaly detected!" if new_anomaly == -1 else "Normal operation.")

# Plot fill levels and anomalies
plt.figure(figsize=(12, 6))
plt.plot(df["timestamp"], df["fill_level"], label="Fill Level")
plt.scatter(
    df["timestamp"][df["anomaly"] == 1],
    df["fill_level"][df["anomaly"] == 1],
    color="red",
    label="Anomaly",
)
plt.legend()
plt.xlabel("Time")
plt.ylabel("Fill Level")
plt.title("Anomaly Detection in Smart Bin Data")
plt.show()
