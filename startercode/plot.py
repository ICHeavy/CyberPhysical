import matplotlib.pyplot as plt

# Data
# valuesL = [14005, 13916, 13967, 13942, 13977, 13996, 13980, 14020, 13959, 13968]
# valuesR = [13611, 13324, 13667, 14863, 13425, 13659, 14697, 13617, 13099, 13145]
lt = [224,241,236,238,246]
# rt = [7794,7814,7808,7846,7889,7901,8162,8228,8139,7888]
time_increments = [i*10 for i in range(len(lt))]  # assuming 10ms time increments

# Plotting
plt.plot(time_increments, lt, label='Channel 1')
# plt.plot(time_increments, rt, label='Right Tachometer Output')
plt.title('Average Tachometer Outputs')
# plt.xlabel('Time (ms)')
plt.ylabel('Output')
plt.legend()
plt.show()
