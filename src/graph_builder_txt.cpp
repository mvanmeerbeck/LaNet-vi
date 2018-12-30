#include "graph_builder_txt.h"
#include <fstream>
#include <iostream>

void Graph_builder_txt::readFile(std::string fileName, Graph* g) {
	std::ifstream file;
	int n1, n2;
	float n3;

	file.open(fileName.c_str(), std::fstream::in);

	int cols = 0;
	char prevc = ' ', c = 'a';
	file.get(c);
	while ((c!='\n')&&(!file.eof())) {
		if (((prevc == '\t')||(prevc == ' '))&&(c != ' ')) {
			cols += 1;
		}
		prevc = c;
		file.get(c);
	}
	file.seekg(0);

	int i = 0;
	int maxVertex = 0;
	if (cols == 2) {
		file >> n1 >> n2;
		while (!file.eof()) {
			if (n1 > maxVertex)
				maxVertex = n1;
			if (n2 > maxVertex)
				maxVertex = n2;
			file >> n1 >> n2;
		}
		file.clear();
		file.seekg(0, std::ios::beg);
		file >> n1 >> n2;
		while (!file.eof()) {
			g->addEdge(n1, n2);
			//std::cout << n1 << " " << n2 << std::endl;
			file >> n1 >> n2;
		}
	}
	else {
		file >> n1 >> n2 >> n3;
		while (!file.eof()) {
			if (n1 > maxVertex)
				maxVertex = n1;
			if (n2 > maxVertex)
				maxVertex = n2;
			file >> n1 >> n2 >> n3;
		}
		file.clear();
		file.seekg(0, std::ios::beg);
		file >> n1 >> n2 >> n3;
		while (!file.eof()) {
			if (g->isWeighted())
				g->addEdge(n1, n2, n3);
			else
				g->addEdge(n1, n2);
			//std::cout << n1 << " " << n2 << std::endl;
			file >> n1 >> n2 >> n3;
		}
	}
	file.close();
}

void Graph_builder_txt::writeFile(std::string fileName, Graph* g) {
	std::ofstream file;
	int n1, n2, n3;

	file.open(fileName.c_str(), std::fstream::out);

	std::list<Vertex*>::iterator it = g->getVertices().begin();
	while (it!=g->getVertices().end()) {
		std::vector<Vertex*>::iterator it2 = (*it)->getNeighbours()->begin();
		std::vector<double>::iterator it3 = (*it)->getWeights().begin();
		while (it2!=(*it)->getNeighbours()->end()) {
			if ((g->isWeighted()==false)&&(g->isSimple()==true)) {
				//std::cout << "Agregando eje " << (*it)->getVertices()[0] << " " << (*it2)->getVertices()[0] << std::endl;
				file << (*it)->getNumber() << " "  << (*it2)->getNumber() << "\n";
				it2++;
			}
			else {
				file << (*it)->getNumber() << " "  << (*it2)->getNumber() << " " << (*it3) << "\n";
				it2++;
				it3++;
			}
		}
		it++;
	}
}

/*void Graph_builder_txt::writeFile(std::string fileName, ConnectedComponent* comp) {
	std::vector<Community*>::iterator it = comp->vertices.begin();
	while (it!=comp->vertices.end()) {
		std::vector<Community*>::iterator it2 = (*it)->community_cuts1.begin();
		while (it2!=(*it)->community_cuts1.end()) {
			//std::cout << "Agregando eje " << (*it)->getVertices()[0] << " " << (*it2)->getVertices()[0] << std::endl;
			std::cout << (*it)->getVertices()[0] << " "  << (*it2)->getVertices()[0] << "\n";
			it2++;
		}
		it++;
	}
}
*/
