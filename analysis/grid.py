import os
import numpy as np


class Grid:
    grid_tag = "" 

    hexagons_cc = None
    triangles_cc = None
    triangles_pol_lon = None
    triangles_pol_lat = None
    triangles_vol = None
    hexagons_pol_lon = None
    hexagons_pol_lat = None
    hexagons_vol = None
    tvi = None
    hvi = None

    ntriangles = 0
    nhexagons = 0
    nfaces = 0

    def __init__(self, res, grid_folder, grid_tag, dtype):
        #hexag = np.genfromtxt(os.path.join(nl.grid_folder,
        #                        'GRID_RES_3_std_HEXAGONS.dat'),
        #                        np.str)
        #print(hexag[:,7])
        #quit()
        self.grid_tag = grid_tag
        grid_name = '{}_{}'.format(res, grid_tag)

        hexagons_cc_x = self.load_grid_file(grid_folder,
                            '{}_hexagons_cc_x.bin'.format(grid_name),
                            dtype)
        hexagons_cc_y = self.load_grid_file(grid_folder,
                            '{}_hexagons_cc_y.bin'.format(grid_name),
                            dtype)
        hexagons_cc_z = self.load_grid_file(grid_folder,
                            '{}_hexagons_cc_z.bin'.format(grid_name),
                            dtype)
        self.hexagons_cc = np.stack([hexagons_cc_x,
                                    hexagons_cc_y,
                                    hexagons_cc_z], axis=1)
        self.nhexagons = self.hexagons_cc.shape[0]

        triangles_cc_x = self.load_grid_file(grid_folder,
                            '{}_triangles_cc_x.bin'.format(grid_name),
                            dtype)
        triangles_cc_y = self.load_grid_file(grid_folder,
                            '{}_triangles_cc_y.bin'.format(grid_name),
                            dtype)
        triangles_cc_z = self.load_grid_file(grid_folder,
                            '{}_triangles_cc_z.bin'.format(grid_name),
                            dtype)
        self.triangles_cc = np.stack([triangles_cc_x,
                                    triangles_cc_y,
                                    triangles_cc_z], axis=1)
        self.ntriangles = self.triangles_cc.shape[0]
        self.triangles_vol = self.load_grid_file(grid_folder,
                            '{}_triangles_vol.bin'.format(grid_name),
                            dtype)
        #triangles_cc = None

        self.triangles_pol_lon = self.load_grid_file(grid_folder,
                        '{}_triangles_pol_lon.bin'.format(grid_name),
                            dtype)
        self.triangles_pol_lat = self.load_grid_file(grid_folder,
                        '{}_triangles_pol_lat.bin'.format(grid_name),
                            dtype)

        self.hexagons_pol_lon = self.load_grid_file(grid_folder,
                        '{}_hexagons_pol_lon.bin'.format(grid_name),
                            dtype)
        self.hexagons_pol_lat = self.load_grid_file(grid_folder,
                        '{}_hexagons_pol_lat.bin'.format(grid_name),
                            dtype)
        self.hexagons_vol = self.load_grid_file(grid_folder,
                            '{}_hexagons_vol.bin'.format(grid_name),
                            dtype)
        #triangles_pol = np.stack([triangles_pol_lon, triangles_pol_lat],
        #                            axis=1)

        for i in range(3):
            tvi_path = os.path.join(grid_folder,
                        '{}_tvi_{}.bin'.format(grid_name, i))
            tvi_col = np.fromfile(tvi_path,
                        dtype=dtype).astype(np.int)
            try:
                tvi[:,i] = tvi_col
            except NameError:
                tvi = np.zeros((len(tvi_col),3))
                tvi[:,0] = tvi_col
        self.tvi = tvi.astype(np.int)

        for i in range(6):
            hvi_path = os.path.join(grid_folder,
                            '{}_hvi_{}.bin'.format(grid_name, i))
            hvi_col = np.fromfile(hvi_path,
                                dtype=dtype).astype(np.int)
            try:
                hvi[:,i] = hvi_col
            except NameError:
                hvi = np.zeros((len(hvi_col),6))
                hvi[:,0] = hvi_col
        self.hvi = hvi.astype(np.int)

        #face_normals_cc_x = load_field(grid_folder, 'face_normals_cc_x.bin')
        #face_normals_cc_y = load_field(grid_folder, 'face_normals_cc_y.bin')
        #face_normals_cc_z = load_field(grid_folder, 'face_normals_cc_z.bin')
        #face_normals_cc = np.stack([face_normals_cc_x, face_normals_cc_y,
        #                            face_normals_cc_z], axis=1)

        #triangles_volume_path = os.path.join(grid_folder, 'triangles_volume.bin')
        #triangles_flxwght_path = os.path.join(grid_folder, 'triangles_flxwght.bin')
        #triangles_normvec_path = os.path.join(grid_folder, 'triangles_normvec.bin')
        #
        #triangles_volume = np.fromfile(triangles_volume_path, dtype=np.float32)
        #triangles_flxwght = np.fromfile(triangles_flxwght_path, dtype=np.float32)
        #triangles_normvec = np.fromfile(triangles_normvec_path, dtype=np.float32)
        #print(np.sum(triangles_volume))
        #quit()

    def load_grid_file(self, grid_folder, file, dtype):
        path = os.path.join(grid_folder, file)
        field = np.fromfile(path, dtype=dtype)
        return(field)

    #def penalty_function(self, lons, lats):
    #    print(lons)

    #def optimize(self):
    #    print(self.triangles_pol_lat)
    #    self.penalty_function(self.triangles_pol_lon, 
    #                            self.triangles_pol_lat)
