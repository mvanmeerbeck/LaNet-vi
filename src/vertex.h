#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <list>
#include <vector>
#include <map>
#include "types.h"
#include <exception>
#include <sstream>

class Component;
class Community;
class Graph_KCores;

using namespace std;

class ExceptionGraphNotDirected: public exception {
  public:
	ExceptionGraphNotDirected() {};
	~ExceptionGraphNotDirected() throw() {};
	virtual const char* what() const throw()
	{
		std::stringstream strm;
		strm << "Graph is not directed.";
		return strm.str().c_str();
	}
};

class ExceptionGraphDirected: public exception {
  public:
	ExceptionGraphDirected() {};
	~ExceptionGraphDirected() throw() {};
	virtual const char* what() const throw()
	{
		std::stringstream strm;
		strm << "Graph is directed.";
		return strm.str().c_str();
	}
};

class Vertex {
  private:
	int number;

	Community* community;
	std::list<Vertex*>* cluster;
	std::vector<Vertex*> neighbours;
	std::vector<Vertex*> is_neighbour_of;
	std::list<int> edgeDenses; //For k-denses
	std::vector<double> w;
	double strength;
	bool directed_links;
	int p;
	void applyPFunction(std::vector<double>& pFunction, Graph_KCores* kcores = NULL, int i = -1);

  public:
	Vertex(int m_number, bool m_directed_links=false);
	Vertex(Vertex* v);
	void addNeighbour(Vertex* v, double weight = 1.0);
	bool hasNeighbour(Vertex* v);

	void addIsNeighbourOf(Vertex* v, double weight = 1.0);
	bool isNeighbourOf(Vertex* v);

	void deleteNeighbour(Vertex* v) { std::vector<Vertex*>::iterator v_it; for (v_it=neighbours.begin();*v_it!=v;v_it++); if (v_it!=neighbours.end()) neighbours.erase(v_it); }
	void deleteIsNeighbourOf(Vertex* v) { std::vector<Vertex*>::iterator v_it; for (v_it=is_neighbour_of.begin();*v_it!=v;v_it++); if (v_it!=is_neighbour_of.end()) is_neighbour_of.erase(v_it); }
	inline std::vector<Vertex*>* getNeighbours() { return &neighbours; }
	inline std::vector<Vertex*>* getIsNeighbourOf() { return &is_neighbour_of; }
	inline std::list<int>* getEdgeDenses() { return &edgeDenses; }
	inline void cleanEdgeDenses() { edgeDenses.clear(); }
	inline std::vector<double> &getWeights() { return w; }
	inline int getNumber() { return number; }
	int getDegree();
	int getInDegree();
	int getOutDegree();
	double getWeight(int i);
	double getStrength();
	int getP(std::vector<double>& pFunction, Graph_KCores* kcores = NULL, int lim = -1) {
		applyPFunction(pFunction, kcores, lim);
		return p;
	}
	void setCluster(std::list<Vertex*>* m_cluster);
	inline std::list<Vertex*>* getCluster() {return cluster;}

	inline void setCommunity(Community* m_community) { community = m_community; }
	inline Community* getCommunity() {return community;}

	std::list<int> listNeighbours();

	bool mark;

};

#endif

