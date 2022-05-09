#include "utils.h"

/*
 * Read in vertices that are contained in the region.
 */
void readVertexSet(SurfaceMesh& mesh, const std::string& filename, std::set<Vertex>& vertexSet) {

    vertexSet.clear();
    std::ifstream curr_file(filename.c_str());
    std::string line;
    std::string X; // leading (indicator) char
    size_t idx;    // element index

    if (curr_file.is_open()) {
        while (!curr_file.eof()) {
            getline(curr_file, line);
            // Ignore any newlines
            if (line == "") {
                continue;
            }
            std::istringstream iss(line);
            iss >> X;
            if (X == "v") {
                iss >> idx;
                vertexSet.insert(mesh.vertex(idx));
            }
        }
        curr_file.close();
    } else {
        std::cerr << "Could not open file <" << filename << ">" << std::endl;
    }
}

/*
 * Display the simplices in a MeshSubset.
 */
void displayMeshSubset(const std::string& name, const MeshSubset& S, SurfaceMesh& mesh,
                       VertexPositionGeometry& geometry, polyscope::SurfaceMesh* psMesh) {

    std::array<double, 3> POLYSCOPE_BLUE = {0.11, 0.388, 0.89};
    std::vector<Vector3> v_pos, e_pos;
    std::vector<std::array<size_t, 2>> v_edges, e_edges;
    std::vector<std::array<double, 3>> fColor(mesh.nFaces(), POLYSCOPE_BLUE);

    for (Vertex v : S.vertices) {
        Vector3 p = geometry.inputVertexPositions[v];
        v_pos.push_back(p);
    }
    for (Edge e : S.edges) {
        Vector3 a = geometry.inputVertexPositions[e.firstVertex()];
        Vector3 b = geometry.inputVertexPositions[e.secondVertex()];
        e_pos.push_back(a);
        e_pos.push_back(b);
        size_t N = e_pos.size();
        e_edges.push_back({N - 2, N - 1});
    }
    for (Face f : S.faces) {
        fColor[geometry.faceIndices[f]] = {1.0, 0.0, 0.0};
    }

    psMesh->addSurfaceGraphQuantity(name + " vertices", v_pos, v_edges)->setEnabled(true);
    psMesh->addSurfaceGraphQuantity(name + " edges", e_pos, e_edges)->setEnabled(true);
    psMesh->addFaceColorQuantity(name + " faces", fColor)->setEnabled(true);
}