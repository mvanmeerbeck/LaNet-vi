#include "graph_node_colors.h"
#include <fstream>
#include <iostream>
/*							let n = String.index !line '\"' in
							!line.[n] <- ' ';
							let n2 = String.rindex !line '\"' in
							!line.[n2] <- ' ';
*/
void Graph_node_colors::readFile(Graph& g, Graph_Colored& colors, std::string fileName) {
	std::ifstream file;
	file.open(fileName.c_str(), std::fstream::in);

	int n1;
	double red, green, blue;

	int pos1, pos2;
	file >> n1 >> red >> green >> blue;
	while (!file.eof()) {
		colors.setColor(g.getVertex(n1), red, green, blue);
		file >> n1 >> red >> green >> blue;
	}

	file.close();
}

