#ifndef _KDENSES_COMPONENT_H_
#define _KDENSES_COMPONENT_H_

#include <list>
#include "vertex.h"
#include "graph.h"
#include "graph_visual.h"

class Vertex;
class Graph;
class Community;
class Graph_KDenses;

extern float central_core_ratio_denses;

class KDenses_Component {
  private:

	std::list<std::list<Vertex*>* > clusters;
	//All vertices in this component (including vertices in internal components)
	std::list<Vertex*> all_vertices;
	std::list<KDenses_Component*> components;

	KDenses_Component* parentComponent;
	Graph* n;
	Graph_KDenses* kdenses;
	Graph_Visual* visual;

	int size; //Vertices in this component (including ones in inner components)
	double tLogDegree; //Degrees of Vertices in this component (including ones in inner components)
	double tLog2Degree; //Degrees of Vertices in this component (including ones in inner components)
	double tLogBetaDegree; //Degrees of Vertices in this component (including ones in inner components)
	int denseCardinal; //Vertices in this component clusters with this denseIndex
	double denseTLogDegree; //Degrees of Vertices in this component clusters with this denseIndex
	double denseTLog2Degree; //Degrees of Vertices in this component clusters with this denseIndex
	double denseTLogBetaDegree; //Degrees of Vertices in this component clusters with this denseIndex
	int denseIndex;

	std::map<Vertex*, int> mark;

//	float central_core_ratio;
	float rho, phi, xCoord, yCoord, u;
	float ratio;
	float endRatio;

	//Auxiliar functions for computeComponents
	void neighboursClusterPropagation(Vertex* v, std::list<Vertex*>* cluster);
	void neighboursComponentPropagation(Vertex* v, KDenses_Component* component, std::list<Vertex*>* cluster);

	void computeComponents(float &c, float maxDenseIndex, float minDenseIndex, float maxDegree);
	
  public:
	KDenses_Component(int m_DenseIndex, KDenses_Component* m_parentComponent, Graph* n, Graph_KDenses* m_kdense, Graph_Visual* visual);

	void computeComponents(std::list<Vertex*>* vertices, float maxShellIndex, float minShellIndex, float maxDegree);
	void printComponent();
	void printCommunities();
	Graph* getNetwork() { return n; };

	void findClusterCoordinates(float maxShellIndex, float minDenseIndex, std::list<Vertex*>& cluster, float partialClusterSum);
	void findCoordinates(int maxDenseIndex, int minDenseIndex, int maxDegree, int phiIni);
	void computeCliqueHostCoordinates(std::list<Vertex*>& clique, int hostsSum, int hostsTotal, int cliqueHosts, float randomm);
	void placeInCircularSector(float ratio, float alfa, int n, int total, float random, float& rho, float& phi, bool unique);
	void logCoordinates();

	inline int getAmountComponents() {return components.size();}
	inline int getSize() {return size;}
	inline std::list<KDenses_Component*>& getComponents() {return components;}
	inline KDenses_Component* getParentComponent() {return parentComponent;}
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
	inline float getCentral_core_ratio() { return central_core_ratio_denses; }
	inline float getDenseIndex() { return denseIndex;}

	int getBrotherComponentsCardinal();
	int getPreviousComponentsCardinal();
	inline bool hasParentComponent() { if (parentComponent!=NULL) return true; else return false;}

};

#endif

