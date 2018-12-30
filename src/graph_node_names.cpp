#include "graph_node_names.h"
#include <fstream>
#include <iostream>
/*							let n = String.index !line '\"' in
							!line.[n] <- ' ';
							let n2 = String.rindex !line '\"' in
							!line.[n2] <- ' ';
*/
void Graph_node_names::readFile(Graph& g, Graph_Named& names, std::string fileName) {
	std::ifstream file;
	file.open(fileName.c_str(), std::fstream::in);

	int n1;

	int pos1, pos2;

	int adic = 0;
	std::string line;
	while ((std::getline(file, line))&&(line.length()>1)) {
		std::stringstream strm(line);
		strm >> n1;

		pos1 = line.find(" ");

		int hasComm = line.find("\"");
		if (hasComm!=std::string::npos)
			adic=1;
		else
			adic=0;

		if (pos1==std::string::npos)
			pos1=0;
		else
			pos1=pos1+1+adic;
		pos2 = line.size();

		if (pos2-pos1>0) {
			if (g.hasVertex(n1))
				names.setName(g.getVertex(n1), line.substr(pos1, pos2-pos1-adic));
		}
	}

	file.close();
}

