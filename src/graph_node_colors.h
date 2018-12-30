#ifndef _GRAPH_NODE_COLORS_H_
#define _GRAPH_NODE_COLORS_H_

#include "graph.h"
#include "graph_colored.h"

class Graph_node_colors {
  public:
	void readFile(Graph& g, Graph_Colored& colors, std::string fileName);
};

#endif
