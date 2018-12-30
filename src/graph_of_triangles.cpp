#include "graph_of_triangles.h"
#include <algorithm>

Graph_of_triangles::Graph_of_triangles(bool m_simple, bool m_weighted) : Graph(m_simple, m_weighted), kcores(this) {
}

void Graph_of_triangles::addTriangle(int v1, int v2, int v3) {
	vertices_list.insert(v1);
	vertices_list.insert(v2);
	vertices_list.insert(v3);
	neighbours[v1].push_back(std::make_pair(v2, v3));
	neighbours[v2].push_back(std::make_pair(v1, v3));
	neighbours[v3].push_back(std::make_pair(v1, v2));
	addEdge(v1, v2);
	addEdge(v2, v3);
	addEdge(v1, v3);

}

