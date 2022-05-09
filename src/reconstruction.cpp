#include "reconstruction.h"


/*
 * Construct the *signed* face-edge adjacency matrix. This is also the boundary operator.
 */
SparseMatrix<int> buildFaceEdgeAdjacencyMatrix(SurfaceMesh& mesh) {

    size_t E = mesh.nEdges();
    size_t F = mesh.nFaces();

    std::vector<Eigen::Triplet<int>> tripletList;
    for (Face f : mesh.faces()) {
        for (Halfedge he : f.adjacentHalfedges()) {
            int sgn = (he.edge().halfedge() == he) ? 1 : -1;
            tripletList.emplace_back(f.getIndex(), he.edge().getIndex(), sgn);
        }
    }
    SparseMatrix<int> M(F, E);
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

/*
 * Determine the topology of the reconstructed region.
 */
// TODO

int eulerCharacteristic(const MeshSubset& S) {

    int V = S.vertices.size();
    int E = S.edges.size();
    int F = S.faces.size();
    return V - E + F;
}

void updateVertexOccupancyData(const std::set<Vertex>& set, VertexData<bool>& vInSet) {

    vInSet.fill(false);
    for (Vertex v : set) {
        vInSet[v] = true;
    }
}

/*
 * Assume for now that the region doesn't have any holes. Grow the regions until there is only a single boundary loop.
 * Then start removing faces. Remove faces only if (1) they are not adjacent to more than 1 vertex in the vertex set;
 * (2) removing it does not cause any vertex in the vertex set to be isolated; (3) that removing them does not induce
 * another boundary loop.
 */
MeshSubset determineDiskRegion(SurfaceMesh& mesh, const std::set<Vertex>& vertex_set) {

    // Grow the regions until there is only a single boundary loop.
    // Determining the Euler characteristic is easier than determining the # of boundary loops, and Euler characteristic
    // changes iff # of boundary loops changes, so keep track of Euler characteristic instead.
    MeshSubset region(vertex_set);
    int chi = eulerCharacteristic(region);
    while (chi != 1) {
        growDisk(region);
        chi = eulerCharacteristic(region);
    }

    // return region;

    // Now start removing faces.
    // Construct the following mesh containers for efficiency when making adjacency queries.
    VertexData<bool> vInSet(mesh, false);
    updateVertexOccupancyData(vertex_set, vInSet);

    MeshSubset temp;
    VertexData<bool> vInRegion(mesh);
    while (true) {
        bool exit = true;
        for (Face f : region.faces) {
            // Check (1)
            int nAdjacentVertices = 0;
            for (Vertex v : f.adjacentVertices()) {
                if (vInSet[v]) {
                    nAdjacentVertices += 1;
                }
            }
            if (nAdjacentVertices > 1) continue;

            // Check (2) & (3)
            temp = region;
            temp.deleteFace(f);
            for (Halfedge he : f.adjacentHalfedges()) {
                temp.deleteEdge(he.edge());
                temp.deleteVertex(he.vertex());
            }
            temp = closure(temp);

            // (2)
            updateVertexOccupancyData(temp.vertices, vInRegion);
            bool skip = false;
            for (Vertex v : vertex_set) {
                if (!vInRegion[v]) {
                    skip = true;
                    break;
                }
            }
            if (skip) continue;

            // (3)
            int newChi = eulerCharacteristic(temp);
            if (newChi == chi) {
                region = temp;
                exit = false;
                break; // don't want to iterate over region.faces while we're changing it
            }
        }

        // Only exit if we make a full sweep of the faces in <region> and don't change anything.
        if (exit) break;
    }

    return region;
}