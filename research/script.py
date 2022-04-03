import matplotlib.pyplot as plt
import numpy as np
import PythonGraphMod

process_vec = np.arange(2, 21)
time_collection = np.fromfile('data.dat', sep='\n')

process_vec_dence, time_collection_dence = PythonGraphMod.InterpolateSet(process_vec, time_collection, 'cubic', min(process_vec), max(process_vec))

fig1, ax1 = PythonGraphMod.CreateSimpleGraph()

ax1.scatter(process_vec, time_collection, color='black', marker='o', s=2, alpha=1)
ax1.plot(process_vec, time_collection, color='red', alpha=0.75, lw=1.5, ls='--')
ax1.plot(process_vec_dence, time_collection_dence, color='blue', alpha=0.75, lw=1.5, ls='--')
ax1.bar(process_vec, time_collection)

# сетка
ax1.minorticks_on()
ax1.grid(which = 'major', color = 'black', linewidth = 1, linestyle = '-', alpha = 0.75)
ax1.grid(which = 'minor', color = 'grey', linewidth = 1, linestyle = '--', alpha = 0.5)

# оформление
plt.ylabel('time (seconds)')
plt.xlabel('process amount')
plt.show()

# fig1.savefig("../images/DeltaPsi.pdf")