import serial
import matplotlib.pyplot as plt
import csv
import time

ser = serial.Serial("COM4", 115200, timeout=1)

ir_data = []
start_time = time.time()

filename = "pulse_data2.csv"

with open(filename, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["time_s", "ir", "bpm", "avg_bpm"])

    plt.ion()

    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if not line:
            continue

        parts = line.split(",")
        if len(parts) != 3:
            continue

        try:
            ir = float(parts[0])
            bpm = float(parts[1])
            avg_bpm = float(parts[2])
        except ValueError:
            continue

        t = time.time() - start_time
        writer.writerow([t, ir, bpm, avg_bpm])
        f.flush()

        ir_data.append(ir)
        if len(ir_data) > 200:
            ir_data.pop(0)

        plt.clf()
        plt.plot(ir_data)
        plt.title(f"BPM: {bpm:.1f} | Avg BPM: {avg_bpm:.1f}")
        plt.xlabel("Sample")
        plt.ylabel("IR Signal")
        plt.pause(0.01)

