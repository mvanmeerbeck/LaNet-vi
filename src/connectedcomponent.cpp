#include "connectedcomponent.h"
#include <iostream>

void ConnectedComponent::printConnectedComponent() {
	std::vector<Vertex*>::iterator vertices_it = vertices.begin();
	while (vertices_it!=vertices.end()) {
		std::cout << (*vertices_it)->getNumber() << std::endl;
		vertices_it++;
	}
	std::cout << std::endl;
}


