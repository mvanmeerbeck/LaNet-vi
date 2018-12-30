#ifndef _KCORES_COMPONENT_H_
#define _KCORES_COMPONENT_H_

#include <list>
#include <vector>
#include "vertex.h"
#include "graph.h"
#include "graph_visual.h"

class Vertex;
class Graph;
class Community;
class Graph_KCores;

class KCores_Component {
  private:

	float central_core_k;
	float central_core_ratio;
	
	std::list<std::list<Vertex*>* > clusters;
	//All vertices in this component (including vertices in internal components)
	std::list<Vertex*> all_vertices;
	std::list<KCores_Component*> components;

	KCores_Component* parentComponent;
	Graph* n;
	Graph_KCores* kcores;
	Graph_Visual* visual;

	int size; //Vertices in this component (including ones in inner components)
	double tLogDegree; //Degrees of Vertices in this component (including ones in inner components)
	double tLog2Degree; //Degrees of Vertices in this component (including ones in inner components)
	double tLogBetaDegree; //Degrees of Vertices in this component (including ones in inner components)
	int shellCardinal; //Vertices in this component clusters with this shellIndex
	double shellTLogDegree; //Degrees of Vertices in this component clusters with this shellIndex
	double shellTLog2Degree; //Degrees of Vertices in this component clusters with this shellIndex
	double shellTLogBetaDegree; //Degrees of Vertices in this component clusters with this shellIndex
	int shellIndex;

	float rho, phi, xCoord, yCoord, u;
	float ratio;
	float endRatio;

	//Auxiliar functions for computeComponents
	void neighboursClusterPropagation(Vertex* v, std::list<Vertex*>* cluster);
	void neighboursComponentPropagation(Vertex* v, KCores_Component* component, std::list<Vertex*>* cluster);

	void computeComponents(float &c, float maxShellIndex, float minShellIndex, float maxDegree, int level = 0);

	void findCoordinatesClassic(int maxShellIndex, int minShellIndex, int maxDegree, int phiIni, int part);
	void findCoordinatesModern(int maxShellIndex, int minShellIndex, int maxDegree, int phiIni);
	
  public:
	KCores_Component(int m_shellIndex, KCores_Component* m_parentComponent, Graph* n, Graph_KCores* m_kcores, Graph_Visual* visual);

	void computeComponents(std::list<Vertex*>* vertices, float maxShellIndex, float minShellIndex, float maxDegree, int level = 0);
	void printComponent();
	Graph* getNetwork() { return n; };

	void findClusterCoordinates(float maxShellIndex, float minShellIndex, std::list<Vertex*>& cluster, float partialClusterSum);
	void findCoordinates(int maxShellIndex, int minShellIndex, int maxDegree, int phiIni);
	void computeCliqueHostCoordinates(std::list<Vertex*>& clique, int hostsSum, int hostsTotal, int cliqueHosts, float randomm);
	void placeInCircularSector(float ratio, float alfa, int n, int total, float random, float& rho, float& phi, bool unique);
	void logCoordinates();

	void findCommunities(std::list<Community*>& communities, Graph* partial_graph);

	inline int getAmountComponents() {return components.size();}
	inline int getSize() {return size;}
	inline std::list<KCores_Component*>& getComponents() {return components;}
	inline KCores_Component* getParentComponent() {return parentComponent;}
	inline std::list<Vertex*>& getVertices() {return all_vertices;}
	inline std::list<std::list<Vertex*>* >& getClusters() { return clusters; };

	void buildClustersMap(std::map<int, std::list<std::list<Vertex*>* > >& clustersMap);

	inline void setRho(float r) { rho = r;}
	inline void setPhi(float p) { phi = p;}
	inline void setX(float x) { xCoord = x;}
	inline void setY(float y) { yCoord = y;}
	inline void setU(float ui) { u = ui;}
	inline void setRatio(float r) { ratio = r;}
	inline float getRho() { return rho;}
	inline float getPhi() { return phi;}
	inline float getX() { return xCoord;}
	inline float getY() { return yCoord;}
	inline float getU() { return u;}
	inline float getRatio() { return ratio;}
	inline float getCentral_core_k() { return central_core_k; }
	inline float getCentral_core_ratio() { return central_core_ratio; }
	inline float getShellIndex() { return shellIndex;}

	int getBrotherComponentsCardinal();
	int getPreviousComponentsCardinal();
	inline bool hasParentComponent() { if (parentComponent!=NULL) return true; else return false;}

};

#endif

