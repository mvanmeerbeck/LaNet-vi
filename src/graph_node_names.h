#ifndef _GRAPH_NODE_NAMES_H_
#define _GRAPH_NODE_NAMES_H_

#include "graph.h"
#include "graph_named.h"

class Graph_node_names {
  public:
	void readFile(Graph& g, Graph_Named& names, std::string fileName);
};

#endif
