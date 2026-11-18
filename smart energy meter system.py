
import numpy as np
import matplotlib.pyplot as plt
import time

# Smart Energy Meter Parameters
TARIFF = 8.0             # Electricity cost in Rs/kWh
OVERLOAD_LIMIT = 0.040   # 40 mA
SIMULATION_TIME = 20     # seconds
SAMPLE_TIME = 1          # seconds

# Simulated input values
voltage_values = [220, 220, 220, 220, 220,
                  220, 220, 220, 220, 220,
                  220, 220, 220, 220, 220,
                  220, 220, 220, 220, 220]

current_values = [0.02, 0.025, 0.03, 0.035, 0.04,
                  0.045, 0.05, 0.035, 0.03, 0.025,
                  0.02, 0.03, 0.04, 0.05, 0.045,
                  0.035, 0.03, 0.025, 0.02, 0.015]

# Variables
energy_kwh = 0.0
time_values = []
power_values = []
energy_values = []
bill_values = []
status_values = []

print("SMART ENERGY METER SIMULATION")
print("-----------------------------------------------")
print("Time  Voltage  Current  Power  Energy  Bill  Status")

for i in range(SIMULATION_TIME):

    voltage = voltage_values[i]
    current = current_values[i]

    # Calculate power
    power = voltage * current

    # Calculate energy in kWh
    energy_kwh += (power * SAMPLE_TIME) / 3600000

    # Calculate electricity bill
    bill = energy_kwh * TARIFF

    # Overload detection
    if current > OVERLOAD_LIMIT:
        status = "OVERLOAD"
    else:
        status = "NORMAL"

    # Store values for graphs
    time_values.append(i + 1)
    power_values.append(power)
    energy_values.append(energy_kwh)
    bill_values.append(bill)
    status_values.append(status)

    print(f"{i+1:4d}  {voltage:7.1f}  {current:7.3f}  "
          f"{power:6.2f}  {energy_kwh:.8f}  "
          f"{bill:.6f}  {status}")

# Plot results
plt.figure(figsize=(10, 8))

plt.subplot(3, 1, 1)
plt.plot(time_values, power_values, marker='o')
plt.title("Power Consumption")
plt.ylabel("Power (W)")
plt.grid(True)

plt.subplot(3, 1, 2)
plt.plot(time_values, energy_values, marker='o')
plt.title("Energy Consumption")
plt.ylabel("Energy (kWh)")
plt.grid(True)

plt.subplot(3, 1, 3)
plt.plot(time_values, bill_values, marker='o')
plt.title("Electricity Bill")
plt.xlabel("Time (seconds)")
plt.ylabel("Bill (Rs)")
plt.grid(True)

plt.tight_layout()
plt.show()
