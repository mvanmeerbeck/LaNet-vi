#ifndef _CONNECTEDCOMPONENT_H_
#define _CONNECTEDCOMPONENT_H_

#include <vector>
#include "vertex.h"

class ConnectedComponent {
  public:
	std::vector<Vertex*> vertices;

	inline void addVertex(Vertex* c) { vertices.push_back(c);}

	void printConnectedComponent();
};

#endif

