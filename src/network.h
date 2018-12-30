#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "graph_visual.h"
#include "graph_named.h"
#include "graph_colored.h"
#include "graph_kcores.h"
#include "graph_kcores_components.h"
#include "kcores_component.h"
#include "graph_kdenses.h"
#include "graph_kdenses_components.h"
#include "kdenses_component.h"

class Network: public Graph {
  friend class Connectivity;
  private:

	void addNodesToConnectedComponent(ConnectedComponent* comp, Vertex* v);

	int id; //To follow a community through time

  public:

	Graph_Named names;
	Graph_Colored colors;
	Graph_KCores kcores;
	Graph_KDenses kdenses;
	Graph_Visual visual;
	Graph_KCores_Components kcores_components;
	Graph_KDenses_Components kdenses_components;

	Network(bool m_simple = true, bool m_weighted = false);
	~Network();

	Network getInducedGraph(std::list<Vertex*> vertices);

	//Gets the induced graph obtained from a core from the k-core decomposition
	Network* getLayer(int layer);

	inline int getId() { return id; }
	inline void setId(int i) { id = i; }

	bool aux;
};

#endif

