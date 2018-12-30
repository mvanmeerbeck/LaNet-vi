#include "network.h"
#include <iostream>
#include "types.h"
#include "math.h"
#include "uniform.h"
#include <algorithm>
#include "connectivity.h"
#include "graph_of_triangles.h"

Network::Network(bool m_simple, bool m_weighted) : Graph(m_simple, m_weighted), kcores(this), kcores_components(this, &kcores, &visual), kdenses(this), kdenses_components(this, &kdenses, &visual) {
}

Network::~Network() {

}

Network Network::getInducedGraph(std::list<Vertex*> vertices) {
	Network new_graph;
	std::list<Vertex*>::iterator it = vertices.begin();
	while (it!=vertices.end()) {
		new_graph.addVertex(*it);
		it++;
	}
	return new_graph;
}

Network* Network::getLayer(int layer) {
	Network* new_graph = new Network(simple, weighted);
	std::list<Vertex*>::iterator it = vertices.begin();
	while (it!=vertices.end()) {
		if (kcores.getKCoresProperties(*it)>=layer)
			new_graph->addVertex(*it);
		it++;
	}
	return new_graph;
}

