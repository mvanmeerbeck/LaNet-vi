#ifndef _GRAPH_KCORES_COMPONENTS_H_
#define _GRAPH_KCORES_COMPONENTS_H_

#include "vertex.h"
#include "graph_kcores.h"
#include "kcores_component.h"
#include "connectivity.h"
#include <map>


typedef struct {
	int kConnectivity;
} tkConnectivityProperties;

class Graph_KCores_Components {

  protected:

	Graph* g;
	Graph_KCores* kcores;
	Graph_Visual* visual;

	void computeKConnectivityStrict();
	void computeKConnectivityWide();

  public:
	
	Graph_KCores_Components(Graph* m_g, Graph_KCores* m_kcores, Graph_Visual* m_visual);

	std::map<Vertex*, tkConnectivityProperties> kConnectivityProperties;

	std::map<int, std::list<std::list<Vertex*>* > > clustersMap;

	//Component of core 0
	KCores_Component mainComponent;

	void findComponents();
	void printComponents();

	void findCoordinates();
	void logCoordinates();

	//COMPONENTS RELATED TO K-DENSES
	std::list<KCores_Component*> inline getComponents() { return mainComponent.getComponents(); };
	KCores_Component* getMainComponent() { return &mainComponent; };

	void buildClustersMap();

	void setKConnectivity(Vertex* v, int k) { kConnectivityProperties[v].kConnectivity = k; };
	int getKConnectivity(Vertex* v) { return kConnectivityProperties[v].kConnectivity; };

	void setkConnectivityProperties(Vertex* v, tkConnectivityProperties kp) { kConnectivityProperties[v] = kp; };
	tkConnectivityProperties getkConnectivityProperties(Vertex* v) { return kConnectivityProperties[v]; };

	//CONNECTIVITY INFORMATION
	bool getIsKConnected(Vertex* v) { return (kConnectivityProperties[v].kConnectivity!=0); }
	bool IsKConnected(Vertex* v);
	bool getIsInC(Vertex* v) { return (kConnectivityProperties[v].kConnectivity==kcores->getKCoresProperties(v)); }
	bool getIsInD(Vertex* v) { return (kConnectivityProperties[v].kConnectivity!=0); }

	void logKConn();

	void computeKConnectivity(std::string type);
	bool conditions(std::list<Vertex*>* Q);
	void setClusterConnectivity(std::list<Vertex*>* Q, int shellIndex);
	int evaluateCutC(Vertex* v);
	int evaluateVertexPhi(int cutWithNotFrontier2, int cutWithC);
	int intersection(std::vector<Vertex*>* l1, std::vector<Vertex*>* l2);

};

#endif

