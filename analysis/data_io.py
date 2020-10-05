import os
import numpy as np
from constants import int_fill


def load_field(field_name, bin_folder, sim_time,
                res, sim_tag, dtype):
    bin_path = os.path.join(bin_folder,
            '{}_np{}_{}_t_{}_sec.bin'.format(sim_tag, 
                    res, field_name,
                    str(sim_time).zfill(int_fill)))

    if not os.path.exists(bin_path):
        print(bin_path)
        raise ValueError()
    else:
        field = np.fromfile(bin_path, dtype=dtype)
        return(field)
