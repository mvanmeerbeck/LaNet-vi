#include "graph_triangled_kcores.h"
#include "network.h"
#include "parameters.h"
#include <algorithm>
#include "graph_of_triangles.h"

Graph_Triangled_KCores::Graph_Triangled_KCores(Graph_of_triangles* m_g) {
	g = m_g;
	maxShellIndex = 0;
	minShellIndex = 0;
}

void Graph_Triangled_KCores::buildPList() {
	PList.clear();
	std::list<Vertex*>::iterator it;
	for (it=g->getVertices().begin();it!=g->getVertices().end();it++) {
		PList[(*it)->getP(pFunction)].push_back(*it);
	}
}

void Graph_Triangled_KCores::buildPFunction() {
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
		int n = ceil((double)g->tVertices()/gr);
		if (n==0)
			n = 1;
		int i = 0;
		while (i < g->tVertices()) {
			i = i + n;
			if (i >= g->tVertices())
				pFunction.push_back(strengths[g->tVertices()-1]);
			else
				pFunction.push_back(strengths[i]);
		}
	}
	else //equalIntervalSize
	{
		double n = (double)strengths[g->tVertices()-1]/gr;
		for (int i=1;i<=gr;i++) {
			if (i!=gr)
				pFunction.push_back(i*n);
			else
				pFunction.push_back(strengths[g->tVertices()-1]);
		}
	}
}

void Graph_Triangled_KCores::findTriangledCores() {

	g->buildDegreesList();
	coresList = g->getDegreesList();

	std::list<Vertex*>::iterator it;

	for (it=g->getVertices().begin();it!=g->getVertices().end(); it++) {
		kTriangledCoresProperties[*it].shellIndex = ((*it)->getDegree());
		(*it)->mark = false; //Vertex marked as not processed
	}


	std::list<std::pair<int, int> > v_neighbours;

	std::map<int, std::map<int, Vertex*> >::iterator list_it;
	std::map<int, Vertex*>::iterator vertex_it;
	std::list<std::pair<int, int> >::iterator neighb_it;

	int actualShellIndex;

	int shellIndex1, shellIndex2;

	Vertex* v;

	int upper_nodes;

	std::map<int, Vertex*> core_bckp;

	//For each list
//	std::cout <<"hay " << coresList.size() << " listas" << std::endl;
	for (list_it=(*coresList).begin();list_it!=(*coresList).end(); list_it++) {
		actualShellIndex = list_it->first;
//		std::cout <<"lista " << actualShellIndex << std::endl;
	}
	for (list_it=(*coresList).begin();list_it!=(*coresList).end(); list_it++) {
		actualShellIndex = list_it->first;
//		std::cout <<"lista " << actualShellIndex << ", " << list_it->second.size() << " nodos" << std::endl;
		//For each vertex
		while (list_it->second.size()!=0) {
			v=list_it->second.begin()->second;
//			std::cout << "  nodo " << v->getNumber() << std::endl;
			v_neighbours = g->neighbours[v->getNumber()];
			//For each neighbour
			for (neighb_it=v_neighbours.begin();neighb_it!=v_neighbours.end(); neighb_it++) {
				std::list<Vertex*>* core;
				shellIndex1 = g->kcores.kTriangledCoresProperties[g->getVertex(neighb_it->first) ].shellIndex;
				shellIndex2 = g->kcores.kTriangledCoresProperties[g->getVertex(neighb_it->second) ].shellIndex;
//				std::cout << "    vecino " << neighb_it->first << "(" << shellIndex1 << ") " << neighb_it->second << "(" << shellIndex2 << ") " << std::endl;
				if ((g->getVertex(neighb_it->first)->mark==false)&&(g->getVertex(neighb_it->second)->mark==false)) {

					//Removing from previous core
					(*coresList)[shellIndex1].erase((neighb_it->first));
					if ((*coresList)[shellIndex1].size() == 0)
						(*coresList).erase(shellIndex1);

					//Moving to a former core
					g->kcores.setShellIndex(g->getVertex(neighb_it->first), max(shellIndex1-2, actualShellIndex));
					(*coresList)[max(shellIndex1-2, actualShellIndex)][neighb_it->first]=g->getVertex(neighb_it->first);

					//Removing from previous core
					(*coresList)[shellIndex2].erase((neighb_it->second));
					if ((*coresList)[shellIndex2].size() == 0)
						(*coresList).erase(shellIndex2);

					//Moving to a former core
					g->kcores.setShellIndex(g->getVertex(neighb_it->second), max(shellIndex2-2, actualShellIndex));
					(*coresList)[max(shellIndex2-2, actualShellIndex)][neighb_it->second]=g->getVertex(neighb_it->second);
				}			

			}
			core_bckp[list_it->second.begin()->first]=v;
			list_it->second.erase(v->getNumber());
			v->mark = true; //Vertex marked as processed
		}
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

void Graph_Triangled_KCores::logCores() {

	Vertex* v;

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

void Graph_Triangled_KCores::printCores() {

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

