#ifndef _GRAPH_KDENSES_COMPONENTS_H_
#define _GRAPH_KDENSES_COMPONENTS_H_

#include "vertex.h"
#include "kdenses_component.h"
#include "connectivity.h"
#include <map>

class Vertex;
class Graph_KDenses;

class Graph_KDenses_Components {

  protected:

	Graph* g;
	Graph_KDenses* kdenses;
	Graph_Visual* visual;

  public:
	
	Graph_KDenses_Components(Graph* m_g, Graph_KDenses* m_kdenses, Graph_Visual* m_visual);

	std::map<int, std::list<std::list<Vertex*>* > > clustersMap;

	//Component of core 0
	KDenses_Component mainComponent;

	void findComponents();
	void printComponents();
	void printCommunities();

	void findCoordinates();
	void logCoordinates();

	//COMPONENTS RELATED TO K-DENSES
	std::list<KDenses_Component*> inline getComponents() { return mainComponent.getComponents(); };
	KDenses_Component* getMainComponent() { return &mainComponent; };

	void buildClustersMap();

};

#endif

