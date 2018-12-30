#ifndef _GRAPH_KDENSES_H_
#define _GRAPH_KDENSES_H_

#include "vertex.h"
#include "graph_of_triangles.h"
#include <map>

class Vertex;
class Graph;

typedef struct {
	int denseIndex;
} tKDensesProperties;

class Graph_KDenses {

  protected:

	Graph* g;

	std::map<Vertex*, tKDensesProperties> kDensesProperties;

	//Map of vertices of the same k-dense
	std::map<int, std::list<Vertex*> > densesList;

	//Maximum dense index.
	int maxDenseIndex;

	//Minimum dense index.
	int minDenseIndex;

  public:

	std::map<std::pair<int, int>, int> map_vertices_pair_to_edge;
	Graph_of_triangles dual_graph;
	
	Graph_KDenses(Graph* m_g);

	void findDenses();
	void logDenses();

	void setKDensesProperties(Vertex* v, tKDensesProperties kp) { kDensesProperties[v] = kp; };
	tKDensesProperties getKDensesProperties(Vertex* v) { return kDensesProperties[v]; };

	void setDenseIndex(Vertex* v, int s) { kDensesProperties[v].denseIndex = s; };
	int getDenseIndex(Vertex* v) { return kDensesProperties[v].denseIndex; };

	inline std::list<Vertex*> getDense(int k) {return densesList[k];};
	inline int getMaxDenseIndex() { return maxDenseIndex;}
	inline void setMaxDenseIndex(int dnInd) { maxDenseIndex = dnInd;}
	inline int getMinDenseIndex() { return minDenseIndex;}
	inline void setMinDenseIndex(int dnInd) { minDenseIndex = dnInd;}

};

#endif

