#include <iostream>
#include "clique.h"
#include <algorithm>
#include "uniform.h"

int CliqueSorterComp::operator()(Vertex* v1, Vertex* v2) {
	if (c->clusteringCoefficients[v1] > c->clusteringCoefficients[v2])
		return -1;
	else
		if (c->clusteringCoefficients[v1] < c->clusteringCoefficients[v2])
			return 1;
		else
			return 0;
}

int CliqueSorterCompareHosts::operator()(Vertex* v1, Vertex* v2) {
	if (v1->getNumber() > v2->getNumber())
		return 1;
	else
		return -1;
}

std::list<Vertex*> l;
bool not_exist_l (const Vertex* v) { return (std::find(l.begin(), l.end(), v)==l.end()); };

void Clique::buildCliques(Network& graph, int maxShellIndex) {
	cliques.clear();
	
	//std::cout << "Building cliques...\n";
	
	//graph.printGraph();

	std::list<Vertex*> lv = graph.getVertices();
	std::list<Vertex*>::iterator lv_it;
	//Initially, all vertexes in the same list
	for (lv_it=lv.begin();lv_it!=lv.end();lv_it++) {
		l.push_back(*lv_it);
	}
	
	for (lv_it=l.begin();lv_it!=l.end();lv_it++) {
		//For each vertex, I compute its clustering coeficient
		int m = 0;
		int n = 0;
		//Printf.printf "Processing vertex %d\n" v#getNumberInFile;
		std::vector<Vertex*>* nb = (*lv_it)->getNeighbours();
		std::vector<Vertex*>::iterator nb_it;
		for (nb_it=nb->begin();nb_it!=nb->end();nb_it++) {
			//If it belongs to the max core
			//Printf.printf "	Processing neighbour %d\n" nb#getNumberInFile;
			if (graph.kcores.getKCoresProperties(*nb_it)==maxShellIndex) {
				//It counts as a neighbour
				n += 1;
				//For each neighbour of nb, if it's v'neighbour too, then it's a connection among neighbours
				std::vector<Vertex*>* nb2 = (*nb_it)->getNeighbours();
				std::vector<Vertex*>::iterator nb_it2;
				for (nb_it2=nb2->begin();nb_it2!=nb2->end();nb_it2++) {
					//Printf.printf "	Processing neighbour of neighbour %d\n" nbnb#getNumber;
					//If it belongs to the max core and is neighbour of both
					if (((*nb_it2)->hasNeighbour(*lv_it))&&(graph.kcores.getKCoresProperties(*nb_it2)==maxShellIndex))
						m += 1;
				}
			}
		}

		//let cc = (float !m)/.(float (!n*(!n-1))) in
		//Not clustering coeficients, but amount of connections among neighbours
		float cc = m;
		
		clusteringCoefficients[*lv_it] = cc;
	}				
	
	//Sorting list according to clustering coeficients
	l.sort(CliqueSorterComp(this));
	//Printf.printf "List sorted by clustering coeficients.\n";
	
	while (l.size()!=0) {
		logCore << "Starting clique." << "\n";
		
		//std::string logging("Clique");
		
		Vertex* v = l.front();
		
		logCore << "\tNode " << v->getNumber() << "\n";
		//logging := Printf.sprintf "%s %d" !logging v#getNumberInFile;
		
		std::vector<Vertex*>* nb = v->getNeighbours();
		
		std::list<Vertex*> clique;
		std::list<Vertex*>::iterator clique_it;

		clique.push_back(v);
		
		std::list<Vertex*> nbToClique;
		std::vector<Vertex*>::iterator it;
		for (it=nb->begin();it!=nb->end();it++)
			nbToClique.push_back(*it);

		//Removing neighbours that are not in l
		nbToClique.remove_if(not_exist_l);
	
		nbToClique.sort(CliqueSorterComp(this));
		
		//std::cout << "NBToClique tiene" << nbToClique.size() << "\n";
		while (nbToClique.size()!=0) {
		
			Vertex* v_hd = nbToClique.front();
			nbToClique.pop_front();

			//std::cout << "Tomo: " << v_hd->getNumber() << "\n";
			bool areAll = true;
			for (clique_it=clique.begin();clique_it!=clique.end();clique_it++) {
				//std::cout << "Considero el " << (*clique_it)->getNumber() << std::endl;
				if ((*clique_it!=v_hd)&&(std::find((*clique_it)->getNeighbours()->begin(),(*clique_it)->getNeighbours()->end(),v_hd)==(*clique_it)->getNeighbours()->end())) {
					areAll = false;
					break;
				}
			}
		
			if (areAll==true) {
				//std::cout << "push_back" << std::endl;
				logCore << "\tNode " << v_hd->getNumber() << "\n";
				clique.push_back(v_hd);
				//Printf.printf "Added\n";
				//logging := Printf.sprintf "%s %d" !logging !front#getNumberInFile;
			}
		}

		//logging := Printf.sprintf "%s\n" !logging;
		//log#print !logging "cores";
		std::list<Vertex*>::iterator it2;
		for (clique_it=clique.begin();clique_it!=clique.end();clique_it++) {
			it2 = std::find(l.begin(), l.end(), *clique_it);
			if (it2!=l.end())
				l.erase(it2);
		}
		
		it2 = std::find(l.begin(), l.end(), v);
		if (it2!=l.end())
			l.erase(it2);
				
		//Printf.printf "\t\t(tam desp %d) " (List.length !l);
		
		clique.sort(CliqueSorterCompareHosts(this));
/*		Uniform u(0.0, 1.0);
		if (u.getValue()>0.5)
			cliques.push_back(clique);
		else
			cliques.push_front(clique);
*/
		cliques.push_back(clique);
		
		//Printf.printf "\n";
	}	
}

