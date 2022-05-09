#pragma once

#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/surface_mesh.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include <algorithm>
#include <set>

#include "mesh-subset.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

void growDisk(MeshSubset& subset);