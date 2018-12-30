#ifndef _CLIQUE_H_
#define _CLIQUE_H_

#include "network.h"
#include <map>

class Clique {
  friend class CliqueSorterComp;
  friend class CliqueSorterCompareHosts;
  private:
	std::list<std::list<Vertex*> > cliques;	

	std::map<Vertex*, int> clusteringCoefficients;

  public:	
	void buildCliques(Network& graph, int maxShellIndex);
	
	inline std::list<std::list<Vertex*> > getCliques() { return cliques;}
};

class CliqueSorterComp {
	Clique* c;
  public:
	CliqueSorterComp(Clique* cl) : c(cl) {}
	int operator()(Vertex* v1, Vertex* v2);
};

class CliqueSorterCompareHosts {
	Clique* c;
  public:
	CliqueSorterCompareHosts(Clique* cl) : c(cl) {}
	int operator()(Vertex* v1, Vertex* v2);
};

#endif

