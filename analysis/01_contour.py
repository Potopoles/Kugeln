import os, glob
import numpy as np
import matplotlib.pyplot as plt
from sys import argv
import nl_01 as nl
from data_io import load_field
from constants import int_fill, CON_PI


files = glob.glob(os.path.join(nl.bin_folder,
        '{}_np{}_*XPOS*'.format(nl.sim_tag, nl.resolution)))
# find sim_times from file names
sim_times = [np.float(file.split('_')[4])/10 for file in files]
## select only output from specified sim_time interval
#sim_times = [st for st in sim_times if \
#        ((st/600 >= nl.min_start) & (st/600 <= nl.min_end))]
## select only output from specified nl.hour_inc time increment
#sim_times = [st for st in sim_times if \
#        (st/600-nl.min_start)*600 % nl.sec_inc == 0]
for sim_time in sim_times:
    #print(sim_time/10)
    xpos = load_field('XPOS', nl.bin_folder, sim_time,
                        nl.resolution, nl.sim_tag, nl.dtype)
    zpos = load_field('ZPOS', nl.bin_folder, sim_time,
                        nl.resolution, nl.sim_tag, nl.dtype)
    xspeed = load_field('XSPEED', nl.bin_folder, sim_time,
                        nl.resolution, nl.sim_tag, nl.dtype)
    zspeed = load_field('ZSPEED', nl.bin_folder, sim_time,
                        nl.resolution, nl.sim_tag, nl.dtype)
    volume = load_field('VOLUME', nl.bin_folder, sim_time,
                        nl.resolution, nl.sim_tag, nl.dtype)
    temp = load_field('TEMP', nl.bin_folder, sim_time,
                        nl.resolution, nl.sim_tag, nl.dtype)

    if nl.var_name == 'XSPEED':
        plot_field = xspeed
    elif nl.var_name == 'VOLUME':
        plot_field = volume
    elif nl.var_name == 'ID':
        plot_field = np.zeros(len(xpos))
        for i in range(0,len(xpos),20):
            np.random.seed(i)
            plot_field[i] = np.random.rand()
    else: raise NotImplementedError()

    fig = plt.figure(figsize=(6,5.0))
    ax1 = fig.add_subplot(111)
    ax1.scatter(xpos, zpos, c=plot_field)
    ax1.set_xlim((0,1000))
    ax1.set_ylim((0,1000))
    #ax1.set_title(np.max(np.abs(plot_field)))
    ax1.set_title('{} seconds'.format(sim_time))
    
    fig.tight_layout()
    fig_path = os.path.join(nl.fig_folder, 
                '{}_{}_{}_{:07.2f}_sec.png'.format(
                    nl.sim_tag, nl.resolution, 
                    nl.var_name, sim_time))
    if nl.i_save:
        print(fig_path)
        plt.savefig(fig_path)
        plt.close(fig)
    else:
        plt.show()
        quit()
