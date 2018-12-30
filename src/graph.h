#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "vertex.h"
#include <map>
#include <list>
#include <vector>
#include <string>
#include "connectedcomponent.h"
#include <iostream>
#include <exception>
#include <sstream>

using namespace std;

class ExceptionVertexNotExistent: public exception {
  private:
	unsigned int number;
  public:
	ExceptionVertexNotExistent(unsigned int n) { number = n; };
	~ExceptionVertexNotExistent() throw() {};
	virtual const char* what() const throw()
	{
		std::stringstream strm;
		strm << "Vertex " << number << " does not exist.";
		return strm.str().c_str();
	}
};

class ExceptionGraphNotWeighted: public exception {
  public:
	ExceptionGraphNotWeighted() {};
	~ExceptionGraphNotWeighted() throw() {};
	virtual const char* what() const throw()
	{
		std::stringstream strm;
		strm << "Trying to add weighted edge in a simple graph.";
		return strm.str().c_str();
	}
};

class ExceptionImproperOrder: public exception {
  private:
	std::string prev, post;
  public:
	ExceptionImproperOrder(std::string m_prev, std::string m_post) { prev.insert(0, m_prev); post.insert(0, m_post); };
	~ExceptionImproperOrder() throw() {};
	virtual const char* what() const throw()
	{
		std::stringstream strm;
		strm << prev.c_str() << " must be called before calling " << post.c_str();
		return strm.str().c_str();
	}
};

class Vertex;

class Graph {

  protected:
	int iq;

	int sumSquareDegrees;

	bool simple, weighted, directed;

	int maxVertex;

	unsigned int total2Edges, totalVertices;

	double total2Weights;

	//Fast access to vertices
	std::map<int, Vertex*> vertices_map;

	std::list<Vertex*> vertices;

	std::vector<ConnectedComponent*> connectedComponents;

	//Maximum degree of a vertex. Initially set to -1 (unknown)
	int maxDegree;
	int maxOutDegree;
	int maxInDegree;

	//Minimum degree of an edge. Initially set to -1 (unknown)
	double minWeight;
	//Maximum degree of an edge. Initially set to -1 (unknown)
	double maxWeight;

	double maxStrength;

	//Map of vertices of the same degree
	std::map<int, std::map<int, Vertex*> > degreesList;

	std::map<Vertex*, int> mark;

	void addNodesToConnectedComponent(ConnectedComponent* comp, Vertex* v);

  public:
	Graph(bool m_simple = true, bool m_weighted = false, bool m_directed=false);
	~Graph();

	bool isWeighted() { return weighted; }
	bool isSimple() { return simple; }
	bool isDirected() { return directed; }

	virtual void addEdge(unsigned int n1, unsigned int n2, double w = 1.0);

	std::map<double, int> getDegreeDistribution();

	void printGraph();
	
	void buildDegreesList();
	void printDegreesList();
	inline std::map<int, std::map<int, Vertex*> >* getDegreesList() { return &degreesList; };

	//Adds a list of vertices to the graph. Those vertex neighbours that don't belong to the graph, are removed from the list of neighbours. The vertices are copies from the parameter. The vertices are returned
	std::vector<Vertex*> addVertices(std::vector<Vertex*>& vertices);
	std::list<Vertex*> addVertices(std::list<Vertex*>& vertices);

	//Adds a vertex to the graph. Those vertex neighbours that don't belong to the graph, are removed from the list of neighbours. The vertex is a copy from the parameter. The vertex is returned.
	Vertex* addVertex(Vertex* v);

	void delVertex(Vertex* v);

	inline std::list<Vertex*>& getVertices() { return vertices;}
	inline std::map<int, Vertex*>& getMapIntVertices() { return vertices_map;}

	Vertex* getVertex(int i);

	//Returns amount of vertices
	inline int tVertices() { return totalVertices; }
	//Returns amount of edges
	inline int tEdges() { return total2Edges; }
	inline double tWeights() { return total2Weights; }

	inline int tDegrees() { return total2Edges; }
	inline int tSqDegrees() { return sumSquareDegrees; }

	inline int getMaxDegree() { return maxDegree;}
	int getMinDegree();

	inline double getMaxWeight() { return maxWeight;}
	inline double getMinWeight() { return minWeight;}

	inline bool hasVertex(int i) { return vertices_map.find(i)!=vertices_map.end();}

	inline double getMaxStrength() { return maxStrength; }

	inline int getMaxVertex() { return maxVertex; }

	Graph* induceGraph(std::list<Vertex*> vl);

	//For this, findCommunities() must not have been called previously
	void findConnectedComponents();
	void printConnectedComponents();
	virtual Graph* extractComponent(ConnectedComponent* comp);

	//For this, findConnectedComponents() must have been called previously
	ConnectedComponent* getBiggestConnectedComponent();
	std::vector<ConnectedComponent*> &getConnectedComponents();

	bool computeDiameter2();

};

#endif

