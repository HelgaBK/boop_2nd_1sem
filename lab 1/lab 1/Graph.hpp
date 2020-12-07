#pragma once

#include <ostream>
#include <vector>
#include "random.hpp"
#include "Vertex.hpp"

template<typename T>
class Graph {
public:
	//Creates an empty graph
	Graph() {}

	//Creates a random graph
	Graph(size_t verticesCount, double density, unsigned int maxEdgeLength, T* dataGenerator());

	~Graph();

	//Adds a vertex to the graph with given data. One pointer to data mush not be added to more than one vertex
	Vertex<T> *addVertex(T*);

	//Adds an edge between given vertices of given length
	void connect(Vertex<T> *vertex1, Vertex<T> *vertex2, unsigned int length);

	//Removes an edge between given vertices
	void disconnect(Vertex<T> *vertex1, Vertex<T> *vertex2);

	//Returns a new graph that is a minimal spanning tree of this
	Graph<T> *getSpanningTree();

	void print(std::ostream&);

protected:
	std::vector<Vertex<T>*> vertices;
};
