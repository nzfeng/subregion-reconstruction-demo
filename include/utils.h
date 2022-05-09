#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/vertex_position_geometry.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"

#include "mesh-subset.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

void readVertexSet(SurfaceMesh& mesh, const std::string& filename, std::set<Vertex>& vertexSet);

void displayMeshSubset(const std::string& name, const MeshSubset& S, SurfaceMesh& mesh,
                       VertexPositionGeometry& geometry, polyscope::SurfaceMesh* psMesh);