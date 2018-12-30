#ifndef _GRAPH_BUILDER_TXT_H_
#define _GRAPH_BUILDER_TXT_H_

#include "graph_builder.h"

class Graph_builder_txt : public Graph_builder {
  public:
	void readFile(std::string fileName, Graph* g);
	void writeFile(std::string fileName, Graph* g);
};

#endif
