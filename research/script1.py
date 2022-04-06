import readline
import matplotlib.pyplot as plt
import numpy as np
import PythonGraphMod

ProcessAmount = 128

fd = open('data_file_list.dat', 'r')
measures_data_files = fd.read().splitlines()
# print(measures_data_files)
fd.close()

process_vec = np.array([2, 4, 8, 16, 32, 64, 128])
time_value = np.array([])
time_var = np.array([]) # дисперсия

for file in measures_data_files:
    # print(file)
    time_collection = np.fromfile(file, sep='\n')
    val = np.average(time_collection)
    var = np.var(time_collection)
    time_value = np.append(time_value, val)
    time_var = np.append(time_var, var)
# print(time_var)

process_vec_dence, time_value_dence = PythonGraphMod.InterpolateSet(process_vec, time_value, 'cubic', min(process_vec), max(process_vec))

fig1, ax1 = PythonGraphMod.CreateSimpleGraph()
fig2, ax2 = PythonGraphMod.CreateSimpleGraph()

plt.errorbar(process_vec, time_value, yerr=time_var, fmt='.', ecolor='black', color='black')
ax1.scatter(process_vec, time_value, color='black', marker='o', s=2, alpha=1)
ax1.plot(process_vec_dence, time_value_dence, color='blue', alpha=0.75, lw=1.5, ls='--')
ax2.scatter(process_vec, time_value, color='black', marker='o', s=2, alpha=1)
ax2.plot(process_vec, time_value, color='red', alpha=0.75, lw=1.5, ls='--')
ax2.plot(process_vec_dence, time_value_dence, color='blue', alpha=0.75, lw=1.5, ls='--')
ax1.bar(process_vec, time_value)

# сетка
ax1.minorticks_on()
ax1.grid(which = 'major', color = 'black', linewidth = 1, linestyle = '-', alpha = 0.75)
ax1.grid(which = 'minor', color = 'grey', linewidth = 1, linestyle = '--', alpha = 0.5)
ax2.minorticks_on()
ax2.grid(which = 'major', color = 'black', linewidth = 1, linestyle = '-', alpha = 0.75)
ax2.grid(which = 'minor', color = 'grey', linewidth = 1, linestyle = '--', alpha = 0.5)

# оформление
plt.ylabel('time (seconds)')
plt.xlabel('process amount')
plt.show()

fig1.savefig("./images/with_bar.pdf")
fig2.savefig("./images/without_bar.pdf")