#ifndef _GRAPH_KCORES_H_
#define _GRAPH_KCORES_H_

#include "connectivity.h"
#include "vertex.h"
#include <map>
#include <vector>
#include <string>

class Graph;

//Como sólo tiene un atributo, directamente se puso un int en el mapa kCoresProperties
typedef struct {
	int shellIndex;
} tKCoresProperties;

class Graph_KCores {

  protected:

	Graph* g;

	std::map<int, int> kCoresProperties;

	//Maximum shell index. Also called cmax
	int maxShellIndex;

	//Minimum shell index.
	int minShellIndex;

	std::vector<double> pFunction;
	void buildPFunction();
	void buildPList();
	std::vector<double> strengths;

  public:

	Graph_KCores(Graph* m_g);

	//Map of vertices of the same core
	std::map<int, std::map<int,Vertex*> >* coresList;

	void findCores(std::vector<double>* pf = NULL);
	void logCores();
	void printCores();

	//K-CORES INFORMATION
	void setKCoresProperties(Vertex* v, int kp) { kCoresProperties[v->getNumber()] = kp; };
	int getKCoresProperties(Vertex* v) { return kCoresProperties[v->getNumber()]; };

	void setShellIndex(Vertex* v, int s) { kCoresProperties[v->getNumber()] = s; };
	int getShellIndex(Vertex* v) { return kCoresProperties[v->getNumber()]; };

	inline int getMaxShellIndex() { return maxShellIndex;}
	inline void setMaxShellIndex(int shInd) { maxShellIndex = shInd;}
	inline int getMinShellIndex() { return minShellIndex;}
	inline void setMinShellIndex(int shInd) { minShellIndex = shInd;}

	//Map of vertices of the same p-value
	std::map<int, std::map<int, Vertex*> > PList;

	inline std::vector<double> getPFunction() { return pFunction; };
	std::map<int, Vertex*> getTopCore();

	void loadCores(std::string fileName);

};

#endif

