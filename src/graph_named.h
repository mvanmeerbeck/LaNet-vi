#ifndef _GRAPH_NAMED_H_
#define _GRAPH_NAMED_H_

#include "vertex.h"
#include <map>

class Vertex;

class Graph_Named {

  protected:
	std::map<Vertex*, std::string> name;

  public:
	
	void setName(Vertex* v, std::string n) { name[v] = n; };
	std::string getName(Vertex* v) { return name[v]; };

};

#endif

