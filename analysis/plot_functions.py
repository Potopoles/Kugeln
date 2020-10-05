import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib.patches import Polygon
from matplotlib.collections import PatchCollection



def create_polygon_2D(value,
            corners_cc, cells_cc, corner_inds,
            polygons, colors, lat_max):
    pos = np.full((len(corner_inds),2),np.nan)
    for cind in range(len(corner_inds)):
        pos[cind, 0] = corners_cc[corner_inds[cind],0]
        pos[cind, 1] = corners_cc[corner_inds[cind],1]

    colors.append(value)
    changed_x = []
    changed_y = []
    # correct the polygons that overlap at the periodic boundaries
    for cind in range(len(corner_inds)):
        #print(np.abs(pos[cind,1] - cells_cc[1]))
        #print(np.abs(pos[cind,1] + lat_max*2 - cells_cc[1]))
        #print()
        if (    np.abs(pos[cind,1] - cells_cc[1]) > 
                np.abs(pos[cind,1] + lat_max*2 - cells_cc[1]) ):
            pos[cind,1] += lat_max*2 
            chy = lat_max*2
            if chy not in changed_y: changed_y.append(chy)

        if (    np.abs(pos[cind,1] - cells_cc[1]) > 
                np.abs(pos[cind,1] - lat_max*2 - cells_cc[1]) ):
            pos[cind,1] -= lat_max*2 
            chy = - lat_max*2
            if chy not in changed_y: changed_y.append(chy)

        #print(np.abs(pos[cind,0] - cells_cc[0]))
        #print(np.abs(pos[cind,0] - 360. - cells_cc[0]))
        #print()

        if (    np.abs(pos[cind,0] - cells_cc[0]) > 
                np.abs(pos[cind,0] + 360. - cells_cc[0]) ):
            pos[cind,0] += 360.
            chx = 360
            if chx not in changed_x: changed_x.append(chx)

        if (    np.abs(pos[cind,0] - cells_cc[0]) > 
                np.abs(pos[cind,0] - 360. - cells_cc[0]) ):
            pos[cind,0] -= 360.
            chx = -360
            if chx not in changed_x: changed_x.append(chx)
    
    # duplicate and shift left the polygons that overlap at 
    # the right x-periodic boundary
    polygon = Polygon(pos, closed=True, linewidth=1)
    polygons.append(polygon)
    for chx in changed_x:
        pos[:,0] -= chx
        polygon = Polygon(pos, closed=True, linewidth=1)
        polygons.append(polygon)
        colors.append(value)
    for chy in changed_y:
        pos[:,1] -= chy
        polygon = Polygon(pos, closed=True, linewidth=1)
        polygons.append(polygon)
        colors.append(value)


def create_2D_plot(field, nl,
            vertices_cc, cells_cc, cvi,
            fig, ax, 
            zmin=None, zmax=None, cmap='cubehelix'):
    polygons = []
    colors = []

    # find exact lat extent of domain
    lat_max = np.abs(np.min(vertices_cc[:,1]))
    ax.set_xlim(0,360)
    ax.set_ylim(-lat_max,lat_max)
    ax.set_xticks(np.arange(0,361,60))

    #ax.scatter(vertices_cc[:,0], vertices_cc[:,1])
    #ax.scatter(cells_cc[:,0], cells_cc[:,1])

    if zmax is None:
        max_val = np.max(field)
    else:
        max_val = zmax
    if zmin is None:
        min_val = np.min(field)
    else:
        min_val = zmin
    for ci in range(len(cvi)):
       create_polygon_2D(field[ci],
                    vertices_cc, cells_cc[ci], cvi[ci],
                    polygons, colors, lat_max)
    #ci = 1
    #create_polygon_2D(field[ci],
    #            vertices_cc, cells_cc[ci], cvi[ci],
    #            polygons, colors, lat_max)
    pc = PatchCollection(polygons, alpha=0.5, cmap=cmap)
    pc.set_array(np.array(colors))
    pc.set_clim([min_val,max_val])
    ax.add_collection(pc)
    fig.colorbar(pc, ax=ax)









def create_polygon_3D(corners_cc, corner_inds):
    if -1 in corner_inds:
        ncorners = len(corner_inds) - 1
    else:
        ncorners = len(corner_inds)
    pos = np.full((ncorners,3),np.nan)
    for cind in range(ncorners):
        pos[cind, 0] = corners_cc[corner_inds[cind],0]
        pos[cind, 1] = corners_cc[corner_inds[cind],1]
        pos[cind, 2] = corners_cc[corner_inds[cind],2]
    return(pos)
    

def create_3D_plot(field,
            vertices_cc, cells_cc, cvi,
            elev_rotation, azim_rotation,
            fig, ax, 
            zmin=None, zmax=None, cmap='cubehelix'):
    polygons = []
    colors = []

    ax.set_xlim((-1,1))
    ax.set_ylim((-1,1))
    ax.set_zlim((-1,1))
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.view_init(elev_rotation, azim_rotation)

    #ax.scatter(vertices_cc[:,0], vertices_cc[:,1], vertices_cc[:,2])
    #ax.scatter(cells_cc[:,0], cells_cc[:,1], cells_cc[:,2])

    if zmax is None:
        max_val = np.max(field)
    else:
        max_val = zmax
    if zmin is None:
        min_val = np.min(field)
    else:
        min_val = zmin

    for ci in range(len(cvi)):
       poly = create_polygon_3D(vertices_cc, cvi[ci])
       polygons.append(poly)
       colors.append(field[ci])

    pc = Poly3DCollection(polygons, alpha=1.0, cmap=cmap)

    #pc.set_edgecolor('dimgrey')
    pc.set_edgecolor(None)
    #pc.set_edgecolor((0.1,0.1,0.1,0.9))
    pc.set_array(np.array(colors))
    pc.set_clim([min_val,max_val])
    ax.add_collection(pc)
    fig.colorbar(pc, ax=ax)


#def plot_icosphere(ax, fig, cvi, vert_cc, cells_cc, field):
#
#    if i_show_vectors:
#        collection.set_array(None)
#        collection.set_facecolor((0.3,0.3,0.3,0.3))
#        i = 5
#        j = 0
#        x = 0.5 * (vert_cc[cvi[i][0],0] + vert_cc[cvi[i][1],0])
#        y = 0.5 * (vert_cc[cvi[i][0],1] + vert_cc[cvi[i][1],1])
#        z = 0.5 * (vert_cc[cvi[i][0],2] + vert_cc[cvi[i][1],2])
#        plot_lon_lat_unit_vectors(x, y, z)
#        plt.show()
#        quit()

#def plot_lon_lat_unit_vectors(x, y, z):
#    ax.scatter(x, y, z, color='red')
#    lon_vec = get_lon_vec(x, y, z)
#    lat_vec = get_lat_vec(x, y, z)
#    ax.plot([x, x+lon_vec[0]],
#            [y, y+lon_vec[1]],
#            [z, z+lon_vec[2]])
#    ax.plot([x, x+lat_vec[0]],
#            [y, y+lat_vec[1]],
#            [z, z+lat_vec[2]])
#
#def get_lon_vec(x, y, z):
#    vec = ( - np.sin(y*np.pi/2),
#              np.sin(x*np.pi/2),
#              0
#          )
#    return(vec)
#
#def get_lat_vec(x, y, z):
#    lon_vec = get_lon_vec(x, y, z)
#    vec = np.cross((x,y,z), lon_vec)
#    #vec = ( - np.sin(z*np.pi/2) * np.cos(y*np.pi/2) * np.sign(x),
#    #        - np.sin(z*np.pi/2) * np.cos(x*np.pi/2) * np.sign(y),
#    #          np.cos(z*np.pi/2) * np.sign(z)
#    #      )
#    return(vec)


