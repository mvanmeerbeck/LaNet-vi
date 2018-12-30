#include "graph_kdenses_components.h"
#include "graph_kdenses.h"
#include "uniform.h"

Graph_KDenses_Components::Graph_KDenses_Components(Graph* m_g, Graph_KDenses* m_kdenses, Graph_Visual* m_visual) : mainComponent(1, NULL, m_g, m_kdenses, m_visual) {
	g = m_g;
	kdenses = m_kdenses;
	visual = m_visual;
}

void Graph_KDenses_Components::findComponents() {
	mainComponent.computeComponents(&g->getVertices(), kdenses->getMaxDenseIndex(), kdenses->getMinDenseIndex(), g->getMaxDegree());
}

void Graph_KDenses_Components::printComponents() {
	std::cout << "------K-DENSE Components BEGIN------" << std::endl;
	mainComponent.printComponent();
	std::cout << "------K-DENSE Components END--------" << std::endl;
}

void Graph_KDenses_Components::printCommunities() {
	mainComponent.printCommunities();
}

//computeComponents must have been invoked before
void Graph_KDenses_Components::findCoordinates() {
	//Start recursive invocation
	double phiIni = uniform2Pi.getValue();
	mainComponent.findCoordinates(kdenses->getMaxDenseIndex(), kdenses->getMinDenseIndex(), g->getMaxDegree(), phiIni);
}

//findCoordinates must have been invoked before
void Graph_KDenses_Components::logCoordinates() {
	mainComponent.logCoordinates();
}

void Graph_KDenses_Components::buildClustersMap() {
	mainComponent.buildClustersMap(clustersMap);

	/*std::map<int, std::list<std::list<Vertex*>* > >::iterator map_it;
	for (map_it=clustersMap.begin();map_it!=clustersMap.end();map_it++) {
		std::cout << "k = " << map_it->first << std::endl;
		std::list<std::list<Vertex*>* > clusters = map_it->second;
		std::list<std::list<Vertex*>* >::iterator clusters_it;
		for (clusters_it=clusters.begin();clusters_it!=clusters.end();clusters_it++) {
			std::list<Vertex*>* cluster = *clusters_it;
			std::list<Vertex*>::iterator cluster_it;
			std::cout << "    ";
			for (cluster_it=cluster->begin();cluster_it!=cluster->end();cluster_it++) {
				std::cout << (*cluster_it)->getNumber() << " ";
			}
			std::cout << std::endl;
		}
	}*/
}

