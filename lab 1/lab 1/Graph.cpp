#include "Graph.hpp"

#include <map>
#include <ostream>
#include <vector>
#include "clone.hpp"
#include "random.hpp"

template<typename T>
Graph<T>::Graph(size_t verticesCount, double density, unsigned int maxEdgeLength, T* dataGenerator()) {
	Vertex<T> *currentVertex;
	bool isConnectedToAny;
	this->vertices.reserve(verticesCount);
	for (size_t i = 0; i < verticesCount; i++) {
		currentVertex = this->addVertex(dataGenerator());
		if (i == 0) {
			continue;
		}

		isConnectedToAny = false;
		for (size_t j = 0; j < i; j++) {
			if (getRandomDouble() < density) {
				this->connect(currentVertex, this->vertices[j], 1 + getRandomInt(maxEdgeLength));
				isConnectedToAny = true;
			}
		}
		//To assure connectivity
		if (!isConnectedToAny) {
			this->connect(currentVertex, this->vertices[getRandomInt(i)], 1 + getRandomInt(maxEdgeLength));
		}
	}
}

template<typename T>
Graph<T>::~Graph() {
	for (size_t i = 0; i < this->vertices.size(); i++) {
		delete vertices[i];
	}
}

template<typename T>
Vertex<T> *Graph<T>::addVertex(T* data) {
	Vertex<T> *newVertex = new Vertex<T>(data);

	this->vertices.push_back(newVertex);

	return newVertex;
}

template<typename T>
void Graph<T>::connect(Vertex<T> *vertex1, Vertex<T> *vertex2, unsigned int length) {
	vertex1->addEdge(vertex2, length);
	vertex2->addEdge(vertex1, length);
}

template<typename T>
void Graph<T>::disconnect(Vertex<T> *vertex1, Vertex<T> *vertex2) {
	vertex1->removeEdge(vertex2);
	vertex2->removeEdge(vertex1);
}

template<typename T>
Graph<T> *Graph<T>::getSpanningTree() {
	Graph<T> *result = new Graph<T>;
	if (this->vertices.size() == 0) {
		return result;
	}

	//Key is a pointer to vertex in original graph, value is a pointer to vertex in a tree
	std::map<Vertex<T>*, Vertex<T>*> inTree;
	Edge<T> *shortestEdge;
	Vertex<T> *shortestEdgeBegin;
	Vertex<T> *newVertex;
	Vertex<T> *currentVertex;

	newVertex = result->addVertex(cloneObject<T>(this->vertices[0]->data));
	inTree[this->vertices[0]] = newVertex;
	while (inTree.size() < this->vertices.size()) {
		shortestEdge = NULL;
		for (std::pair<Vertex<T>*, Vertex<T>*> verticesPair : inTree) {
			currentVertex = verticesPair.first;
			for (Edge<T> *currentEdge : currentVertex->incidentEdges) {
				if (inTree.count(currentEdge->vertex) > 0) {
					continue;
				}

				if (shortestEdge == NULL || currentEdge->length < shortestEdge->length) {
					shortestEdge = currentEdge;
					shortestEdgeBegin = currentVertex;
				}
			}
		}
		newVertex = result->addVertex(cloneObject<T>(shortestEdge->vertex->data));
		result->connect(inTree[shortestEdgeBegin], newVertex, shortestEdge->length);
		inTree[shortestEdge->vertex] = newVertex;
	}

	return result;
}

template<typename T>
void Graph<T>::print(std::ostream &stream) {
	for (Vertex<T> *vertex : this->vertices) {
		stream << *(vertex->data);
		if (vertex->incidentEdges.size() == 0) {
			stream << "\n";
			continue;
		}
		stream << " |";
		for (Edge<T> *edge : vertex->incidentEdges) {
			stream << " " << *(edge->vertex->data) << "(" << edge->length << ")";
		}
		stream << "\n";
	}
}

template class Graph<int>;
