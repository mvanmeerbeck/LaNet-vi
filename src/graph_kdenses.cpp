#include "graph_kdenses.h"
#include "network.h"
#include "parameters.h"
#include <algorithm>
#include "types.h"

Graph_KDenses::Graph_KDenses(Graph* m_g) {
	g = m_g;
	maxDenseIndex = 0;
	minDenseIndex = 0;
}


void Graph_KDenses::findDenses() {

	int edges_counter = 0;
	int e1, e2, e3;
	std::map<std::pair<int, int>, int>::iterator map_it;

	//Building dual graph

	std::list<Vertex*>::iterator it;
	std::vector<Vertex*>::iterator nb_it, nb2_it;
	Vertex* v, *v1, *v2, *v3;
	for (it=g->getVertices().begin();it!=g->getVertices().end();it++) {
		v = *it;
		//For each edge
		for (nb_it=v->getNeighbours()->begin();nb_it!=v->getNeighbours()->end();nb_it++) {
			if ((*nb_it)->getNumber()>v->getNumber()) {
				v2 = *nb_it;
				for (nb2_it=v2->getNeighbours()->begin();nb2_it!=v2->getNeighbours()->end();nb2_it++) {
					if (((*nb2_it)->getNumber()>v2->getNumber())&&((*nb2_it)->hasNeighbour(v))) {
						v3 = *nb2_it;

						map_it = map_vertices_pair_to_edge.find(std::make_pair(v->getNumber(), v2->getNumber()));
						if (map_it != map_vertices_pair_to_edge.end())
							e1 = map_it->second;
						else {
							e1 = edges_counter++;
							map_vertices_pair_to_edge[std::make_pair(v->getNumber(), v2->getNumber())] = e1;
						}

						map_it = map_vertices_pair_to_edge.find(std::make_pair(v2->getNumber(), v3->getNumber()));
						if (map_it != map_vertices_pair_to_edge.end())
							e2 = map_it->second;
						else {
							e2 = edges_counter++;
							map_vertices_pair_to_edge[std::make_pair(v2->getNumber(), v3->getNumber())] = e2;
						}

						map_it = map_vertices_pair_to_edge.find(std::make_pair(v->getNumber(), v3->getNumber()));
						if (map_it != map_vertices_pair_to_edge.end())
							e3 = map_it->second;
						else {
							e3 = edges_counter++;
							map_vertices_pair_to_edge[std::make_pair(v->getNumber(), v3->getNumber())] = e3;
						}

						dual_graph.addTriangle(e1, e2, e3);

						//std::cout << v->getNumber() << "-" << v2->getNumber() << "-" << v3->getNumber() << std::endl;
					}
				}
			}
		}
	}

	//std::cout << "   Buscando kcores en el grafo dual de triángulos..." << std::endl;

	dual_graph.kcores.findTriangledCores();

	//std::cout << "   Asignando el kdense a cada vértice del original..." << std::endl;
	//std::cout << "   (el kdense de un vértice es el mayor de los kcores de los ejes adyacentes a él en el dual)" << std::endl;

	//Getting k-dense for each vertex
	int edge_id;
	int edge_k_dense;
	int v_k_dense = -1;
	for (it=g->getVertices().begin();it!=g->getVertices().end();it++) {
		v_k_dense = 2;
		v = *it;
//		std::cout << "Vertice " << v->getNumber() << std::endl;
		//For each edge
		v->cleanEdgeDenses();
		for (nb_it=v->getNeighbours()->begin();nb_it!=v->getNeighbours()->end();nb_it++) {
			if ((*nb_it)->getNumber()>v->getNumber()) {
				v1 = v;
				v2 = *nb_it;
			}
			else {
				v1 = *nb_it;
				v2 = v;
			}
			map_it = map_vertices_pair_to_edge.find(std::make_pair(v1->getNumber(), v2->getNumber()));
			if (map_it != map_vertices_pair_to_edge.end()) {
				edge_id = map_it->second;
				edge_k_dense = dual_graph.kcores.getShellIndex(dual_graph.getVertex(edge_id))/2 + 2;
//				std::cout << "    Eje con " << (*nb_it)->getNumber() << " dense " << edge_k_dense << std::endl;
				if (edge_k_dense > v_k_dense)
					v_k_dense = edge_k_dense;
				v->getEdgeDenses()->push_back(edge_k_dense);
			}
			else {
//				std::cout << "    Eje con " << (*nb_it)->getNumber() << " dense " << 2 << std::endl;
				v->getEdgeDenses()->push_back(2);
			}
		}
		//std::cout << "Vertex " << (*it)->getNumber() << " kdense " << v_k_dense << std::endl;
		kDensesProperties[*it].denseIndex = v_k_dense;
		densesList[v_k_dense].push_back(*it);
	}

	//Find maximum and minimum shell index
	std::map<int, std::list<Vertex*> >::iterator map2_it;
	map2_it = densesList.begin();
	minDenseIndex = map2_it->first;
	while (map2_it!=densesList.end()) {
		maxDenseIndex = map2_it->first;
		map2_it++;
	}

	int msi = dual_graph.kcores.getMaxShellIndex();
	int a = dual_graph.kcores.getCore(msi).size();
	std::cout << "(" << msi << ") Hay " <<  a << " ejes en el top-dense" << std::endl;

}

void Graph_KDenses::logDenses() {

	Vertex* v;

	std::map<int, std::list<Vertex*> >::iterator list_it;
	std::list<Vertex*>::iterator vertex_it;

	//For each list
	for (list_it=densesList.begin();list_it!=densesList.end(); list_it++) {
		std::cout << "Dense " << list_it->first << " [" << list_it->second.size() << " nodos]" << std::endl;
		//For each vertex
		for (vertex_it=list_it->second.begin();vertex_it!=list_it->second.end(); vertex_it++) {
			v=*vertex_it;
			std::cout << v->getNumber() << " ";
			logCore << list_it->first << " " << v->getNumber() << "\n";
		}
		std::cout << std::endl;

	}
}

