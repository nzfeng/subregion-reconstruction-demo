#pragma once

#include "geometrycentral/surface/surface_mesh.h"
#include <set>

using namespace geometrycentral;
using namespace geometrycentral::surface;

class MeshSubset {

  public:
    std::set<Vertex> vertices;
    std::set<Edge> edges;
    std::set<Face> faces;

    /* Initialize an empty MeshSubset. */
    MeshSubset() {}

    /* Initialize from just a set of one of type of element */
    MeshSubset(const std::set<Vertex>& vertices_) : vertices(vertices_) {}
    MeshSubset(const std::set<Edge>& edges_) : edges(edges_) {}
    MeshSubset(const std::set<Face>& faces_) : faces(faces_) {}

    /* Initialize a MeshSubset with the given vertices, edges, and faces, as well as a reference to underlying mesh. */
    MeshSubset(const std::set<Vertex>& vertices_, const std::set<Edge>& edges_, const std::set<Face>& faces_)
        : vertices(vertices_), edges(edges_), faces(faces_) {}

    /* Make a deep copy of the input MeshSubset and return it as a new MeshSubset.
     */
    MeshSubset deepCopy() const {
        return MeshSubset(vertices, edges, faces);
    }

    /* Add a vertex to this subset. */
    void addVertex(const Vertex& v) {
        vertices.insert(v);
    }

    /* Add a set of vertices to this subset. */
    void addVertices(const std::set<Vertex>& V) {
        for (Vertex v : V) {
            vertices.insert(v);
        }
    }

    /* Delete a vertex from this subset. */
    void deleteVertex(const Vertex& v) {
        vertices.erase(v);
    }

    /* Delete a set of vertices from this subset. */
    void deleteVertices(const std::set<Vertex>& V) {
        for (Vertex v : V) {
            vertices.erase(v);
        }
    }

    /* Add an edge to this subset. */
    void addEdge(const Edge& e) {
        edges.insert(e);
    }

    /* Add a set of edges to this subset. */
    void addEdges(const std::set<Edge>& E) {
        for (Edge e : E) {
            edges.insert(e);
        }
    }

    /* Delete an edge from this subset. */
    void deleteEdge(const Edge& e) {
        edges.erase(e);
    }

    /* Delete a set of edges from this subset. */
    void deleteEdges(const std::set<Edge>& E) {
        for (Edge e : E) {
            edges.erase(e);
        }
    }

    /* Add a face to this subset. */
    void addFace(const Face& face) {
        faces.insert(face);
    }

    /* Add a set of faces to this subset. */
    void addFaces(const std::set<Face>& F) {
        for (Face f : F) {
            faces.insert(f);
        }
    }

    /* Delete a face from this subset. */
    void deleteFace(const Face& f) {
        faces.erase(f);
    }

    /* Delete a set of faces from this subset. */
    void deleteFaces(const std::set<Face>& F) {
        for (Face f : F) {
            faces.erase(f);
        }
    }

    /* Returns true if subsets are equivalent. */
    bool equals(const MeshSubset& other) const {
        // == compares elements at each position; but std::set always orders elements upon insertion/initialization
        return (vertices == other.vertices) && (edges == other.edges) && (faces == other.faces);
    }

    /* Adds a subset's vertices, edges, and faces to this subset. */
    void addSubset(const MeshSubset& other) {
        addVertices(other.vertices);
        addEdges(other.edges);
        addFaces(other.faces);
    }

    /* Removes a subset's vertices, edges, and faces from this subset. */
    void deleteSubset(const MeshSubset& other) {
        deleteVertices(other.vertices);
        deleteEdges(other.edges);
        deleteFaces(other.faces);
    }

    /* Print vertices. */
    void printVertices() {
        std::cout << "Vertices: ";
        for (Vertex v : vertices) {
            std::cout << v << ", ";
        }
        std::cerr << std::endl;
    }

    /* Print edges. */
    void printEdges() {
        std::cout << "Edges: ";
        for (Edge e : edges) {
            std::cout << e << ", ";
        }
        std::cerr << std::endl;
    }

    /* Print Faces. */
    void printFaces() {
        std::cout << "Faces: ";
        for (Face f : faces) {
            std::cout << f << ", ";
        }
        std::cerr << std::endl;
    }
};