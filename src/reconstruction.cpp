#include "reconstruction.h"

/*
 * Construct the *unsigned* vertex-edge adjacency matrix A0.
 * TODO: make signed
 */
SparseMatrix<size_t> buildVertexEdgeAdjacencyMatrix(SurfaceMesh& mesh) {

    size_t V = mesh.nVertices();
    size_t E = mesh.nEdges();

    std::vector<Eigen::Triplet<size_t>> tripletList;
    SparseMatrix<size_t> M(E, V);
    for (Vertex v : mesh.vertices()) {
        for (Edge e : v.adjacentEdges()) {
            tripletList.emplace_back(e.getIndex(), v.getIndex(), 1);
        }
    }

    M.setFromTriplets(tripletList.begin(), tripletList.end());
    return M;
}

/*
 * Construct the *unsigned* face-edge adjacency matrix A1.
 * TODO: make signed
 */
SparseMatrix<size_t> buildFaceEdgeAdjacencyMatrix(SurfaceMesh& mesh) {

    size_t E = mesh.nEdges();
    size_t F = mesh.nFaces();

    std::vector<Eigen::Triplet<size_t>> tripletList;
    SparseMatrix<size_t> M(F, E);
    for (Face f : mesh.faces()) {
        for (Edge e : f.adjacentEdges()) {
            tripletList.emplace_back(f.getIndex(), e.getIndex(), 1);
        }
    }

    M.setFromTriplets(tripletList.begin(), tripletList.end());
    return M;
}

/*
 * Compute the simplicial star St(S) of the selected subset of simplices.
 */
MeshSubset star(const MeshSubset& subset) {

    MeshSubset S = subset.deepCopy();

    // For each vertex in S, add incident edges
    for (Vertex v : S.vertices) {
        for (Edge e : v.adjacentEdges()) {
            S.addEdge(e);
        }
    }

    // For each edge in S, add incident faces
    for (Edge e : S.edges) {
        for (Face f : e.adjacentFaces()) {
            S.addFace(f);
        }
    }

    return S;
}

/*
 * Compute the closure Cl(S) of the selected subset of simplices.
 */
MeshSubset closure(const MeshSubset& subset) {

    MeshSubset S = subset.deepCopy();

    // For each face in S, get all the edges it contains.
    for (Face f : S.faces) {
        for (Edge e : f.adjacentEdges()) {
            S.addEdge(e);
        }
    }

    // For each edge (including ones newly acquired), get all its incident vertices.
    for (Edge e : S.edges) {
        for (Vertex v : e.adjacentVertices()) {
            S.addVertex(v);
        }
    }

    return S;
}

/*
 * The basic "region-growing" operation. Takes as input a MeshSubset containing the selected simplices (vertices, edges,
 * and faces.) Returns Cl(St(S)), i.e. the closure of its star.
 */
void growDisk(MeshSubset& subset) {

    subset = closure(star(subset));
}