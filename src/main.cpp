#include "geometrycentral/numerical/linear_solvers.h"
#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/vertex_position_geometry.h"

#include "polyscope/messages.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"

#include "args/args.hxx"
#include "imgui.h"

#include <chrono>

#include "reconstruction.h"
#include "utils.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

// == Geometry-central data
std::unique_ptr<ManifoldSurfaceMesh> mesh;
std::unique_ptr<VertexPositionGeometry> geometry;

// Polyscope visualization handle, to quickly add data to the surface
polyscope::SurfaceMesh* psMesh;
std::string MESHNAME;

std::set<Vertex> VERTEX_SET;
MeshSubset SUBSET;

void functionCallback() {

    if (ImGui::Button("Test")) {
        MeshSubset S = determineDiskRegion(*mesh, VERTEX_SET);
        displayMeshSubset("S", S, *mesh, *geometry, psMesh);
    }

    if (ImGui::Button("Vertices from faces")) {

        // hard-code test set for now
        std::set<Face> face_set;
        face_set.insert(mesh->face(2882));
        face_set.insert(mesh->face(17615));
        face_set.insert(mesh->face(14538));
        face_set.insert(mesh->face(14591));
        face_set.insert(mesh->face(15758));

        std::vector<Vertex> vertices = getBoundaryVerticesFromFaceSet(*mesh, face_set);
        // display
        std::vector<Vector3> v_pos;
        std::vector<std::array<size_t, 2>> v_edges;
        for (Vertex v : vertices) {
            Vector3 p = geometry->inputVertexPositions[v];
            v_pos.push_back(p);
        }
        psMesh->addSurfaceGraphQuantity("b vertices", v_pos, v_edges)->setEnabled(true);
    }
}

int main(int argc, char** argv) {

    // Configure the argument parser
    args::ArgumentParser parser("subdomain-from-vertices");
    args::Positional<std::string> meshFilename(parser, "mesh", "A mesh file.");
    args::Positional<std::string> vertexFilename(parser, "vertices", "File containing vertices in the region.");

    // Parse args
    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    // If a mesh name was not given, use default mesh.
    std::string meshFilepath = "../data/bunny.obj";
    std::string vertexFilepath = "../data/bunny-disk.txt";
    if (meshFilename) {
        meshFilepath = args::get(meshFilename);
    }
    if (vertexFilename) {
        vertexFilepath = args::get(vertexFilename);
    }

    // Initialize polyscope
    polyscope::init();

    // Load the mesh
    std::tie(mesh, geometry) = readManifoldSurfaceMesh(meshFilepath);
    MESHNAME = polyscope::guessNiceNameFromPath(meshFilepath);

    geometry->requireVertexIndices();
    geometry->requireEdgeIndices();
    geometry->requireFaceIndices();

    // Register mesh to display with Polyscope.
    psMesh = polyscope::registerSurfaceMesh(MESHNAME, geometry->inputVertexPositions, mesh->getFaceVertexList(),
                                            polyscopePermutations(*mesh));
    // Set the callback function
    polyscope::state::userCallback = functionCallback;

    // Read in vertex set information.
    readVertexSet(*mesh, vertexFilepath, VERTEX_SET);

    SUBSET.vertices = VERTEX_SET;
    displayMeshSubset("V", SUBSET, *mesh, *geometry, psMesh);

    // Give control to the polyscope gui
    polyscope::show();

    return EXIT_SUCCESS;
}