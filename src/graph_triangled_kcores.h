#ifndef _GRAPH_TRIANGLED_KCORES_H_
#define _GRAPH_TRIANGLED_KCORES_H_

#include "vertex.h"
#include <map>

class Vertex;
class Graph_of_triangles;

typedef struct {
	int shellIndex;
} tKTriangledCoresProperties;

class Graph_Triangled_KCores {

  protected:

	Graph_of_triangles* g;

	std::map<Vertex*, tKTriangledCoresProperties> kTriangledCoresProperties;

	//Map of vertices of the same core
	std::map<int, std::map<int, Vertex*> >* coresList;

	//Maximum shell index. Also called cmax
	int maxShellIndex;

	//Minimum shell index.
	int minShellIndex;

	std::vector<double> pFunction;
	void buildPFunction();
	void buildPList();
	std::vector<double> strengths;

  public:
	
	Graph_Triangled_KCores(Graph_of_triangles* m_g);

	void findTriangledCores();
	void logCores();
	void printCores();

	inline std::map<int, Vertex*> getCore(int c) {return (*coresList)[c];};

	//K-CORES INFORMATION
	void setKTriangledCoresProperties(Vertex* v, tKTriangledCoresProperties kp) { kTriangledCoresProperties[v] = kp; };
	tKTriangledCoresProperties getKTriangledCoresProperties(Vertex* v) { return kTriangledCoresProperties[v]; };

	void setShellIndex(Vertex* v, int s) { kTriangledCoresProperties[v].shellIndex = s; };
	int getShellIndex(Vertex* v) { return kTriangledCoresProperties[v].shellIndex; };

	inline int getMaxShellIndex() { return maxShellIndex;}
	inline void setMaxShellIndex(int shInd) { maxShellIndex = shInd;}
	inline int getMinShellIndex() { return minShellIndex;}
	inline void setMinShellIndex(int shInd) { minShellIndex = shInd;}

	//Map of vertices of the same p-value
	std::map<int, std::list<Vertex*> > PList;

};

#endif

