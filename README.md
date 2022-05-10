# Region reconstruction demo
Given a set of vertices on a mesh that represent a (possibly sparse) sampling of a region on the mesh, reconstruct that region.

![teaser](images/teaser.png?raw=true)

# Getting started
Clone the repo, including the submodules:
```
git clone --recursive https://github.com/nzfeng/subregion-reconstruction-demo
```

This demo takes the following as input:
* a mesh file, representing the surface
* a txt file, representing the vertices that are contained in the region

```
mkdir build
cd build
cmake ..
make -j8	# or however many cores you have
bin/main <surface_mesh> <vertex_occupancy_file>
```

# Data format
The vertex information is a txt file. The file contains the indices of occupied vertices, each on a separate line, each preceded the char "v":

```
v 125
v 109
v 115
v 132
v 156
...	# the rest of the file
```

The program handles duplicate entries.

# Explanation of algorithm
This demo was motivated by the desire to orient a set of vertices meant to represent the boundary of a region. A straightforward way of orienting such a set is to reconstruct the region, and let the orientation of the boundary be the one induced by the region.

In this case, we are not just given boundary data, we are given a set of vertices that represents an occupancy function: a vertex is included in the set if it belongs to the region. The "volumetric" nature of the input data points us towards a region-growing approach. But how do we know when to stop growing the regions? I.e., we don't want to accidentally fill in "large" areas where there is little to no samples, because such an area likely represents a hole in the region. The idea is therefore to use _persistent homology_ to determine the correct topology of the region. Roughly speaking, we grow balls around each point in the dataset, and keep track of when loops are "born" and when they "die". Loops that are "persistent", i.e. have a long life between birth and death, are likely to be actual features of the dataset, and unlikely to be due to noise or uneven sampling.

Although persistent homology gets us a region of the correct topology, it may not be faithful geometrically to the data -- it may not be very smooth, may contain extra faces (i.e. is not well-concentrated around the data), etc. Trying to minimize the area of the region while maintaining its topology should get us closer to what we want.

Assumptions of this method: The input vertex set may have some false negatives, i.e. the set may not contain every vertex in the region and is thus a sparse sampling of sorts, although we assume that false positives are relatively rare. We assume that the desired region is manifold.

Upon some further reading, it turns out the basic idea of this method is quite similar to [this paper](https://arxiv.org/pdf/1811.12543.pdf).