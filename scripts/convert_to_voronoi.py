#!/usr/bin/env python

import rospy
import numpy as np
from scipy.spatial import Voronoi
import cv2

# Load the occupancy grid map (PGM format)
def load_pgm(filename):
    with open(filename, 'rb') as f:
        # Read through the binary header until a newline character is found
        while True:
            line = f.readline().decode('utf-8', 'ignore')
            if not line or line.strip() == "":
                continue
            if line.startswith("P5"):
                break

        # Read the dimensions from the header
        while True:
            line = f.readline().decode('utf-8', 'ignore')
            if not line or line.strip() == "":
                continue
            try:
                width, height = map(int, line.split())
                break
            except ValueError:
                continue

        # Read the binary data
        data = np.fromfile(f, dtype=np.uint8, count=width * height).reshape((height, width))

        # Convert to occupancy grid format (0 for free space, 100 for obstacles)
        occupancy_grid = np.where(data > 127, 100, 0)
    
    return occupancy_grid

# Convert the occupancy grid map to Voronoi diagram and save it as a PGM map
def occupancy_grid_to_voronoi_pgm(occupancy_grid, output_filename):
    # Find coordinates of free cells (0 represents free space)
    free_cells = np.transpose(np.where(occupancy_grid == 0))
    
    # Compute Voronoi diagram
    vor = Voronoi(free_cells)

    # Create a new map to represent the Voronoi diagram
    voronoi_map = np.zeros_like(occupancy_grid, dtype=np.uint8)

    # Mark Voronoi edges in the map
    for edge in vor.ridge_vertices:
        if edge[0] >= 0 and edge[1] >= 0:
            x0, y0 = map(int, vor.vertices[edge[0]])
            x1, y1 = map(int, vor.vertices[edge[1]])
            cv2.line(voronoi_map, (x0, y0), (x1, y1), 255, 1)

    # Save the Voronoi map as a PGM file
    with open(output_filename, 'wb') as f:
        f.write(b'P5\n')
        f.write(f'{voronoi_map.shape[1]} {voronoi_map.shape[0]}\n'.encode('utf-8'))
        f.write(b'255\n')
        voronoi_map.tofile(f)

    print(f'Voronoi map saved as {output_filename}')

# Convert the occupancy grid map to Voronoi diagram
def occupancy_grid_to_voronoi(occupancy_grid):
    # Find coordinates of free cells (0 represents free space)
    free_cells = np.transpose(np.where(occupancy_grid == 0))
    
    # Compute Voronoi diagram
    vor = Voronoi(free_cells)
    return vor

if __name__ == '__main__':
    # Initialize ROS node
    rospy.init_node('voronoi_publisher')

    # Load the "allroomedited.pgm" map file (provide the correct path if needed)
    pgm_filename = 'allroomedited.pgm'
    occupancy_grid = load_pgm(pgm_filename)

    # Convert occupancy grid to Voronoi diagram
    vor = occupancy_grid_to_voronoi(occupancy_grid)

    # Save the Voronoi diagram as a PGM file
    voronoi_map_filename = 'voronoi_map.pgm'
    occupancy_grid_to_voronoi_pgm(occupancy_grid, voronoi_map_filename)

    rospy.spin()
