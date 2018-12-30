#include "graph_kcores.h"
#include "network.h"
#include "parameters.h"
#include <algorithm>
#include "types.h"
#include "vertex.h"

Graph_KCores::Graph_KCores(Graph* m_g) {
	g = m_g;
	maxShellIndex = 0;
	minShellIndex = 0;

}

void Graph_KCores::buildPList() {
	PList.clear();
	std::list<Vertex*>::iterator it;
	for (it=g->getVertices().begin();it!=g->getVertices().end();it++) {
//		std::cout << "Vertice " << (*it)->getNumber() << "str " << (*it)->getStrength() << std::endl;
		PList[(*it)->getP(pFunction)][(*it)->getNumber()]=*it;
	}
}

void Graph_KCores::buildPFunction() {
	int gr = parameters.getGranularity();
	if (gr==-1)
		gr = g->getMaxDegree();
	pFunction.push_back(0.0);

	std::list<Vertex*>::iterator it;

	for (it=g->getVertices().begin();it!=g->getVertices().end(); it++) {
		strengths.push_back((*it)->getStrength());
	}
	sort(strengths.begin(), strengths.end());

	if (parameters.getStrengthsIntervals() == "equalNodesPerInterval") {
		std::cout << "Equal Nodes " << gr << std::endl;
		pFunction.push_back(0.0);
		int n = ceil((double)g->tVertices()/gr);
		if (n==0)
			n = 1;
		int i = 0;
		for (int i=1;i<=gr;i++) {
			if (i!=gr) {
				//std::cout << i << " " << strengths[ceil(((double)g->tVertices()-1)*i/gr)] << std::endl;
				pFunction.push_back(strengths[ceil(((double)g->tVertices()-1)*i/gr)]);
			}
			else {
				//std::cout << i << " " << strengths[ceil(((double)g->tVertices()-1)*i/gr)] << std::endl;
				pFunction.push_back(strengths[g->tVertices()-1]);
			}
		}
	}
	else if (parameters.getStrengthsIntervals() == "equalLogIntervalSize") {
		std::cout << "Equal IntervalSize" << std::endl;
		double a = (double)strengths[0];
		double b;

		bool fixed_maximum=true;
		double fixed_maximum_strength=30000;

		if (parameters.getMaximumStrength()!=-1)
			b = parameters.getMaximumStrength();
		else
			b = (double)strengths[g->tVertices()-1];
		for (int i=1;i<=gr;i++) {
			if (i!=gr) {
				std::cout << i << " " << a*pow((b/a),((double)i/gr)) << std::endl;
				pFunction.push_back(a*pow((b/a),((double)i/gr)));
			}
			else {
				std::cout << i << " " << b << std::endl;
				pFunction.push_back(b);
			}
		}
	}
	else //equalIntervalSize
	{
		pFunction.push_back(0.0);
		bool fixed_maximum=true;
		double fixed_maximum_strength=30000;
		double n;

		if (parameters.getMaximumStrength()!=-1)
			n = (double)parameters.getMaximumStrength()/gr;
		else
			n = (double)strengths[g->tVertices()-1]/gr;
		for (int i=1;i<=gr;i++) {
			if (i!=gr) {
				//std::cout << i << " " << i*n << std::endl;
				pFunction.push_back(i*n);
			}
			else {
				if (parameters.getMaximumStrength()!=-1)
					pFunction.push_back(parameters.getMaximumStrength());
				else {
					//std::cout << i << " " << strengths[g->tVertices()-1] << std::endl;
					pFunction.push_back(strengths[g->tVertices()-1]);
				}
			}
		}
	}
}

std::map<int, Vertex*> Graph_KCores::getTopCore() {
	return (*coresList)[maxShellIndex];
}

void Graph_KCores::findCores(std::vector<double>* pf) {

	std::list<Vertex*>::iterator it;

	if (!(g->isWeighted())) {
		g->buildDegreesList();
//		std::cout << "Parte A " << std::endl;
		coresList = g->getDegreesList();
//		std::cout << "Parte B " << std::endl;
		int i=0;
		for (it=g->getVertices().begin();it!=g->getVertices().end(); it++) {
//			if (i%10000==0) std::cout << "i:  " << i << std::endl;
			i++;
			kCoresProperties[(*it)->getNumber()] = ((*it)->getDegree());
		}

	}
	else {
	//std::cout << "Parte 1 " << std::endl;
	if (pf != NULL) {
		pFunction = *pf;
	}
	else {
		buildPFunction();
	}
	//std::cout << "Parte 2 " << std::endl;
		buildPList();
	//std::cout << "Parte 3 " << std::endl;
		coresList = &PList;
	//std::cout << "Parte 4 " << std::endl;

		for (it=g->getVertices().begin();it!=g->getVertices().end(); it++) {
			kCoresProperties[(*it)->getNumber()] = ((*it)->getP(pFunction));
//			std::cout << "Vertice core " << (*it)->getP(pFunction) << std::endl;
		}

	}

	Vertex* v;
	std::vector<Vertex*>* v_neighbours;

	std::map<int, std::map<int, Vertex*> >::iterator list_it;
	std::map<int, Vertex*>::iterator vertex_it;
	std::vector<Vertex*>::iterator neighb_it;

	int actualShellIndex;
	int newP;
	
	std::map<int, Vertex*> core_bckp;
	//std::cout << "Parte 5 " << std::endl;
	
	int marca=0;
	int van=0;
	int iii=0;
	//For each list
	for (list_it=(*coresList).begin();list_it!=(*coresList).end(); list_it++) {
		actualShellIndex = list_it->first;
		//std::cout << "EMPIEZO CORE " << actualShellIndex << ", tiene " << list_it->second.size() << std::endl;
		marca = list_it->second.size();
		//std::cout << "marca: " << marca << std::endl;
		//For each vertex
		while (list_it->second.size()!=0) {
			v=list_it->second.begin()->second;
			van+=1;
			if (van==marca) {
				marca = list_it->second.size();
				//std::cout << "marca: " << marca << std::endl;
				van = 0;
			}
//			std::cout << "    Vertice " << v->getNumber() << std::endl;
			v_neighbours = v->getNeighbours();
			//For each neighbour
			for (neighb_it=v_neighbours->begin();neighb_it!=v_neighbours->end(); neighb_it++) {
				int shellIndex = kCoresProperties[(*neighb_it)->getNumber()];
				if (shellIndex > actualShellIndex) {
					//Removing from previous core
					(*coresList)[shellIndex].erase((*neighb_it)->getNumber());
					if ((*coresList)[shellIndex].size() == 0)
						(*coresList).erase(shellIndex);
					//Moving to a former core

					if (!g->isWeighted()) {
	                                        kCoresProperties[(*neighb_it)->getNumber()] = shellIndex-1;
        	                                (*coresList)[shellIndex-1][(*neighb_it)->getNumber()]=(*neighb_it);
					}
					else {
						newP = (*neighb_it)->getP(pFunction, this, actualShellIndex);
						kCoresProperties[(*neighb_it)->getNumber()] = max(newP, actualShellIndex);
						(*coresList)[max(newP, actualShellIndex)][(*neighb_it)->getNumber()]=(*neighb_it);
					}
				}			
			}
			iii+=1;
			core_bckp[v->getNumber()]=v;
			list_it->second.erase(v->getNumber());
		}
		//std::cout << iii << std::endl;
		//std::cout << "Tiene " << core_bckp.size() << std::endl;
		(*coresList)[actualShellIndex]=core_bckp;
		core_bckp.clear();
	}
	//Find maximum and minimum shell index
	std::map<int, std::map<int, Vertex*> >::iterator map_it;
	map_it = (*coresList).begin();
	minShellIndex = map_it->first;
	while (map_it!=(*coresList).end()) {
		maxShellIndex = map_it->first;
		map_it++;
	}
}

void Graph_KCores::logCores() {

	Vertex* v;
	//std::cout << "LOGUEANDO... " << "hay " << g->tVertices() << " vertices" << std::endl;
	std::map<int, std::map<int, Vertex*> >::iterator list_it;
	std::map<int, Vertex*>::iterator vertex_it;

	//For each list
	for (list_it=(*coresList).begin();list_it!=(*coresList).end(); list_it++) {
		//For each vertex
		for (vertex_it=list_it->second.begin();vertex_it!=list_it->second.end(); vertex_it++) {
			v=vertex_it->second;
			logCore << list_it->first << " " << v->getNumber() << "\n";
		}
	}
}

void Graph_KCores::printCores() {

	Vertex* v;

	std::map<int, std::map<int, Vertex*> >::iterator list_it;
	std::map<int, Vertex*>::iterator vertex_it;

	std::cout << "Cores BEGIN" << std::endl;
	//For each list
	for (list_it=(*coresList).begin();list_it!=(*coresList).end(); list_it++) {
		std::cout << list_it->first << ": ";
		//For each vertex
		for (vertex_it=list_it->second.begin();vertex_it!=list_it->second.end(); vertex_it++) {
			v=vertex_it->second;
			std::cout << v->getNumber() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Cores END" << std::endl;
}

void Graph_KCores::loadCores(std::string fileName) {

	std::ifstream entrada;
	entrada.open(fileName.c_str());

	int n1, core;
	entrada >> n1 >> core;
	while (!entrada.eof()) {
		kCoresProperties[n1] = core;
		entrada >> n1 >> core;
	}
}


