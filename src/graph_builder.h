#ifndef _GRAPH_BUILDER_H_
#define _GRAPH_BUILDER_H_

#include <fstream>
#include "graph.h"

class Graph_builder {
  public:
	virtual void readFile(std::string fileName, Graph* g) = 0;
	virtual void writeFile(std::string fileName, Graph* g) = 0;
};

#endif

