#include <ostream>
#include <vector>
#include "Vertex.hpp"

template<typename T>
Vertex<T>::Vertex(T *data) {
	this->data = data;
}

template<typename T>
Vertex<T>::~Vertex() {
	delete this->data;
	for (Edge<T> *edge : this->incidentEdges) {
		delete edge;
	}
}

template<typename T>
void Vertex<T>::addEdge(Vertex<T>* edgeEnd, unsigned int length) {
	this->incidentEdges.push_back(new Edge<T>(edgeEnd, length));
}

template<typename T>
void Vertex<T>::removeEdge(Vertex<T> *edgeEnd) {
	Edge<T> *edge;
	for (typename std::vector<Edge<T>*>::iterator edgesIterator = this->incidentEdges.begin(); edgesIterator != this->incidentEdges.end(); edgesIterator++) {
		edge = *edgesIterator;
		if (edge->vertex == edgeEnd) {
			delete edge;
			this->incidentEdges.erase(edgesIterator);
			return;
		}
	}
}

template class Vertex<int>;
