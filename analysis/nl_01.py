import os
import numpy as np
from sys import argv
from constants import CON_PI

var_name = str(argv[1])
resolution = int(argv[2])

sim_tag = "test"

alpha=0
#alpha=0.05
#alpha=CON_PI/2 - 0.05
#alpha=CON_PI/2

min_start = 0
min_end = 10
sec_inc = 1
i_save = 1

#dtype=np.float32
dtype=np.float64


# max 26 & min 27 (& min 35)
# 24, 26, 27, 60, 62, 63
bin_folder = os.path.join('..', 'output')
fig_folder = os.path.join('..', 'figures')
grid_folder = os.path.join('..', 'grid')

i_show_vectors = 0
plot_3D = True

#elev_rotation = 25
#azim_rotation = 45

elev_rotation = 0
azim_rotation = 90