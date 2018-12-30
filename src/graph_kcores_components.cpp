#include "graph_kcores_components.h"
#include "graph_kcores.h"
#include "vertex.h"
#include <algorithm>
#include "uniform.h"
#include "network.h"

std::vector<int> kcoresvalues;
std::vector<int> mark;

Graph_KCores_Components::Graph_KCores_Components(Graph* m_g, Graph_KCores* m_kcores, Graph_Visual* m_visual) : mainComponent(0, NULL, m_g, m_kcores, m_visual) {
	g = m_g;
	kcores = m_kcores;
	visual = m_visual;
	kcoresvalues.resize(20000000);
	mark.resize(20000000);
}

void Graph_KCores_Components::findComponents() {
	std::list<Vertex*>::iterator vertex_it;
	std::list<Vertex*> vertices = g->getVertices();

	for (vertex_it=vertices.begin(); vertex_it!=vertices.end(); vertex_it++) {
		//std::cout << (*vertex_it)->getNumber() << " " << kcores->getKCoresProperties(*vertex_it).shellIndex << std::endl;
		kcoresvalues[(*vertex_it)->getNumber()]=kcores->getKCoresProperties((*vertex_it));
	}

	mainComponent.computeComponents(&g->getVertices(), kcores->getMaxShellIndex(), kcores->getMinShellIndex(), g->getMaxDegree());
}

void Graph_KCores_Components::printComponents() {
	mainComponent.printComponent();
}

//computeComponents must have been invoked before
void Graph_KCores_Components::findCoordinates() {
	//Start recursive invocation
	double phiIni = uniform2Pi.getValue();
	mainComponent.findCoordinates(kcores->getMaxShellIndex(), kcores->getMinShellIndex(), g->getMaxDegree(), phiIni);
}

//findCoordinates must have been invoked before
void Graph_KCores_Components::logCoordinates() {
	mainComponent.logCoordinates();
}

void Graph_KCores_Components::buildClustersMap() {
	mainComponent.buildClustersMap(clustersMap);

	/*std::map<int, std::list<std::list<Vertex*>* > >::iterator map_it;
	for (map_it=clustersMap.begin();map_it!=clustersMap.end();map_it++) {
		std::cout << "k = " << map_it->first << std::endl;
		std::list<std::list<Vertex*>* > clusters = map_it->second;
		std::list<std::list<Vertex*>* >::iterator clusters_it;
		for (clusters_it=clusters.begin();clusters_it!=clusters.end();clusters_it++) {
			std::list<Vertex*>* cluster = *clusters_it;
			std::list<Vertex*>::iterator cluster_it;
			std::cout << "    ";
			for (cluster_it=cluster->begin();cluster_it!=cluster->end();cluster_it++) {
				std::cout << (*cluster_it)->getNumber() << " ";
			}
			std::cout << std::endl;
		}
	}*/
}

void Graph_KCores_Components::logKConn() {

	Vertex* v;

	std::map<int, std::map<int, Vertex*> >::iterator list_it;
	std::list<Vertex*>::iterator list_vertex_it;
	std::map<int, Vertex*>::iterator map_vertex_it;
	std::vector<Vertex*>::iterator vertex_it, vertex_it2;

	std::map<std::pair<int, int>, int> histo; //Acumula nodos no StrictConnected por shellindex y phi value
	std::map<std::pair<int, int>, int> histo2; //Acumula nodos no WideConnected por shellindex y phi value

	logKConnectivity << "#Node number - Shell Index - KConnectivity\n";

	Vertex *d, *a;
	int minShellIndexPair;
	int n = 0;
	mapa_si_kconn.resize(kcores->getMaxShellIndex()+1);
	mapa_si_nokconn.resize(kcores->getMaxShellIndex()+1);
	for (int i=0;i<=kcores->getMaxShellIndex();i++) {
		mapa_si_kconn[i] = 0;
		mapa_si_nokconn[i] = 0;
	}

	std::vector<Vertex*> vv;
	for (list_vertex_it=g->getVertices().begin();list_vertex_it!=g->getVertices().end();list_vertex_it++) {
		vv.push_back(*list_vertex_it);
	}

/* Comentado el 07/12/2015 porque generaba problemas con los logs de la red grande biggestC_grafo_compact_mpls.txt
	if (g->tVertices()<300000) {
		for (int i=0;i<vv.size();i++) {
			for (int j=0;j<vv.size();j++) {
				if (i!=j) {
					d = vv[i];
					a = vv[j];
					minShellIndexPair = kcores->getKCoresProperties(d);
					if (kcores->getKCoresProperties(a)<minShellIndexPair) minShellIndexPair = kcores->getKCoresProperties(a);

					if (getIsInC(d)&&getIsInC(a))
						mapa_si_kconn[minShellIndexPair]++;
					else		
						mapa_si_nokconn[minShellIndexPair]++;
				}
			}
			n++;
			if (n%1000==0)
				std::cout << n << std::endl;
		}

		std::map<int, int>::iterator it2;
		int no_kconn, si_kconn;
		for (int i=kcores->getMinShellIndex();i<=kcores->getMaxShellIndex();i++) {
			if ((mapa_si_kconn[i]!=0)||(mapa_si_nokconn[i]!=0)) {

				logGomoryHu3 << i << " " << mapa_si_nokconn[i]/2 << " " << mapa_si_kconn[i]/2 << "\n";
			}
		}
	}
*/

	//For each list
	for (list_it=kcores->coresList->begin();list_it!=kcores->coresList->end(); list_it++) {
		//For each vertex
		for (map_vertex_it=list_it->second.begin();map_vertex_it!=list_it->second.end(); map_vertex_it++) {
			v=map_vertex_it->second;
			logKConnectivity << v->getNumber() << " " << list_it->first << " " << getKConnectivity(v) << "\n";

			if (getKConnectivity(v)!=list_it->first) {
				std::pair<int, int> par = std::make_pair<int, int>(int(list_it->first), getKConnectivity(v));
				if (histo.find(par)==histo.end())
					histo[par] = 1;
				else
					histo[par]++;	

				par = std::make_pair<int, int>(list_it->first-getKConnectivity(v), getKConnectivity(v));
				if (histo2.find(par)==histo2.end())
					histo2[par] = 1;
				else
					histo2[par]++;	
			}
		}
	}

//	logKConn2 << "#Shell Index - Phi value - Not KConnected amount of Pairs\n";

//	logKConn3 << "#Shell Index minus Phi value - Phi value - Not KConnected Amount of Pairs\n";

/*	for (int i=0;i<=maxShellIndex;i++) {
		for (int j=0;j<=maxShellIndex;j++) {
			std::pair<int, int> par = std::make_pair<int, int>(i, j);
			if (histo.find(par)==histo.end())
				histo[par]=0;
			if (histo2.find(par)==histo2.end())
				histo2[par]=0;
			logKConn2 << i << " " << j << " " << histo[par] << "\n";
			logKConn3 << i << " " << j << " " << histo2[par] << "\n";
		}
	}
*/
}

int Graph_KCores_Components::intersection(std::vector<Vertex*>* l1, std::vector<Vertex*>* l2) {
	std::vector<Vertex*>::iterator it1 = l1->begin();
	int i = 0;
	it1 = std::find_first_of(it1, l1->end(), l2->begin(), l2->end());
	while (it1 != l1->end()) {
		i++;
		it1++;
		it1 = std::find_first_of(it1, l1->end(), l2->begin(), l2->end());
	}
	
	return i;
}

//Computes Phi for a vertex in a cluster
int Graph_KCores_Components::evaluateVertexPhi(int cutWithNotFrontier2, int cutWithC) {
	int firstMax = 1;
	if (cutWithNotFrontier2 > 1) firstMax = cutWithNotFrontier2;
	int secondMin = firstMax;
	if (cutWithC < secondMin) secondMin = cutWithC;
	return secondMin;
}

int Graph_KCores_Components::evaluateCutC(Vertex* v) {
	int cutC = 0;
	std::vector<Vertex*>::iterator nb_it;
	for (nb_it=v->getNeighbours()->begin();nb_it!=v->getNeighbours()->end();nb_it++) {
		if (getIsKConnected(*nb_it)==true)

			cutC += 1;
	}
	return cutC;
}

int k_conditions = 0;

//Evaluates k-connectivity conditions in a cluster
bool Graph_KCores_Components::conditions(std::list<Vertex*>* Q) {
	std::list<Vertex*>::iterator Q_it;
	Graph g;
	int intVirtualVertex;

	//int k = Q->front()->getShellIndex();
	int k = k_conditions;
/*	if ((k_conditions == 1)&&(Q->front()->getShellIndex()==2))
		std::cout << "Verificando conectividad " << k << " para cluster " << Q->front()->getNumber() << " core " << Q->front()->getShellIndex() << std::endl;
*/
/*	if (Q->front()->getNumber()==8511)
		std::cout << "Verificando conectividad " << k << " para cluster " << Q->front()->getNumber() << " core " << Q->front()->getShellIndex() << std::endl;
*/	//First condition: Contracted diameter < 2
	for (Q_it=Q->begin();Q_it!=Q->end();Q_it++) {
		g.addVertex(*Q_it);
	}
	intVirtualVertex = g.getMaxVertex() + 1;

	for (Q_it=Q->begin();Q_it!=Q->end();Q_it++) {
		std::vector<Vertex*>::iterator nb_it = (*Q_it)->getNeighbours()->begin();
		bool enc = false;
		while ((nb_it != (*Q_it)->getNeighbours()->end())&&(!enc)) {
			if (IsKConnected(*nb_it))
				enc=true;
			else
				nb_it++;
		}
		if (nb_it != (*Q_it)->getNeighbours()->end())
			g.addEdge((*Q_it)->getNumber(), intVirtualVertex);
	}
	bool diameter2Condition = g.computeDiameter2();
/*	if (Q->front()->getNumber()==8511)
		std::cout << "\tCond de diametro: " << diameter2Condition << std::endl;
	//Condicion de diametro, excepto para k=1, que no hace falta
*/	if ((diameter2Condition==false)&&(k!=1)) {
		//if (k == 28)
		//	std::cout << "Capa 28. No cumple condicion de diametro. Nodos: " << Q->size() << std::endl;
		//else
			return false;
	}
	//Second condition: Phi
	int phi = 0;
	std::list<Vertex*>::iterator it;
	//Compute frontier
	std::vector<Vertex*> B; //Frontier
	for (it=Q->begin();it!=Q->end();it++) {
		int cutC = evaluateCutC(*it);
		if (cutC >= 1)
			B.push_back(*it);
	}

	//Compute not frontier2
	std::vector<Vertex*> NotB2; //Frontier2
	for (it=Q->begin();it!=Q->end();it++) {
		int cutC = evaluateCutC(*it);
		if (cutC < 2) {
			NotB2.push_back(*it);
		}
	}

	if ((B.size()>=k)||(B.size()==Q->size()))
		return true;

	//if (k == 28)
	//	std::cout << "Capa 28. No cumple condiciones fáciles" << std::endl;

	for (it=Q->begin();it!=Q->end();it++) {
		int cutNotFrontier2 = intersection((*it)->getNeighbours(), &NotB2);
		//if (Q->front()->getNumber()==27)
		//	std::cout << "Nodo " << (*it)->getNumber() << " " << cutNotFrontier2 << std::endl;
		int cutC = evaluateCutC(*it);
		//if (Q->front()->getNumber()==27)
		//	std::cout << "cutC " << (*it)->getNumber() << " " << cutC << std::endl;
		phi += evaluateVertexPhi(cutNotFrontier2, cutC);
	}
	//if (Q->front()->getNumber()==27)
	//	std::cout << "\tPhi: " << phi << std::endl;
	return (phi>=k);
}

void Graph_KCores_Components::setClusterConnectivity(std::list<Vertex*>* Q, int shellIndex) {
	std::list<Vertex*>::iterator Q_it;
	Vertex* v;
	for (Q_it=Q->begin();Q_it!=Q->end();Q_it++) {
		v = *Q_it;
//		std::cout << "Nodo " << v->getNumber() << " conectividad " << shellIndex << std::endl;
		setKConnectivity(v, shellIndex);
	}
}

bool Graph_KCores_Components::IsKConnected(Vertex* v) {
	return getIsKConnected(v);
}

void Graph_KCores_Components::computeKConnectivityStrict() {

	//Looking for a cluster with maxShellIndex
	std::list<Vertex*>* Q;

	std::list<std::list<Vertex*>* >::iterator it_Q = clustersMap[kcores->getMaxShellIndex()].begin();

	bool found = false;
	int shell = kcores->getMaxShellIndex();

	std::list<std::list<Vertex*>* >::iterator Q_it, Q_it2;

	while ((!found)&&((it_Q!=clustersMap[shell].end())||(shell!=kcores->getMinShellIndex()))) {

		Network g;
		g.addVertices(*(*it_Q));
		//std::cout << shell << " " << g.computeDiameter2() << " " << g.getMinDegree() << std::endl;
		//FIXME
		if (((g.computeDiameter2()==true)||(shell==77))&&(g.getMinDegree()>=shell)) {
			found=true;
			Q=*it_Q;
			setClusterConnectivity(Q, shell); //Like joining Q to C
			break;
		}
		/*Connectivity g_conn(&g);
		g_conn.build_gomory_hu_tree(g.tVertices());
		if (g_conn.find_minimum_cut() >= shell) {
			found=true;
			Q=*it_Q;
			setClusterConnectivity(Q, shell); //Like joining Q to C
			break;
		}*/
		it_Q++;
		if ((it_Q==clustersMap[shell].end())&&(shell!=kcores->getMinShellIndex())) {
			clustersMap.erase(shell);
			shell--;
//			while (clustersMap.find(shell)==clustersMap.end());
//				shell--;
			it_Q=clustersMap[shell].begin();
		}
	}
	clustersMap.erase(shell);

	std::cout << "Part B" << std::endl;

	std::map<int, std::list<std::list<Vertex*>* > >::iterator map_it;

	while (clustersMap.empty()==false) {
		map_it = clustersMap.end();
		map_it--;
		int thisShellIndex = map_it->first;
		std::cout << "shell index: " << thisShellIndex << std::endl;
		k_conditions = thisShellIndex;

		std::list<std::list<Vertex*>* > lQ = map_it->second;

		std::list<std::list<Vertex*>* >::iterator Q_it = lQ.begin();
		bool enc = false;
		while ((Q_it != lQ.end())&&(!enc)) {
			if (conditions(*Q_it))
				enc=true;
			else
				Q_it++;
		}

		while (Q_it != lQ.end()) {
//			std::cout << "  Adding a cluster Q to C" << std::endl;
			setClusterConnectivity(*Q_it, thisShellIndex); //Like joining Q to C
			while (lQ.front() != *Q_it)
				lQ.pop_front();
			lQ.pop_front();
			//lQ.remove(*Q_it);

			Q_it = lQ.begin();
			enc = false;
			while ((Q_it != lQ.end())&&(!enc)) {
				if (conditions(*Q_it))
					enc=true;
				else
					Q_it++;
			}
		}
		clustersMap.erase(thisShellIndex);
	}

}

void Graph_KCores_Components::computeKConnectivityWide() {

	//Looking for a cluster with maxShellIndex
	std::list<Vertex*>* Q;
	std::map<int, std::list<std::list<Vertex*>* > > clustersMapPrime;
	std::list<std::list<Vertex*>* >::iterator it_Q = clustersMap[kcores->getMaxShellIndex()].begin();
	bool found = false;
	int shell = kcores->getMaxShellIndex();

	while ((!found)&&((it_Q!=clustersMap[shell].end())||(shell!=kcores->getMinShellIndex()))) {
		Network g;
		g.addVertices(*(*it_Q));
		if (g.computeDiameter2()==true) {
			found=true;
			Q=*it_Q;
			clustersMap[shell].erase(it_Q);
			setClusterConnectivity(Q, shell); //Like joining Q to C
			break;
		}
		Connectivity g_conn(&g);
		g_conn.build_gomory_hu_tree(g.tVertices());
		if (g_conn.find_minimum_cut() >= shell) {
			found=true;
			Q=*it_Q;
			clustersMap[shell].erase(it_Q);
			setClusterConnectivity(Q, shell); //Like joining Q to C
			break;
		}
		it_Q++;
		if ((it_Q==clustersMap[shell].end())&&(shell!=kcores->getMinShellIndex())) {
			clustersMapPrime[shell]=clustersMap[shell];
			clustersMap.erase(shell);
			shell--;
			it_Q=clustersMap[shell].begin();
		}
	}
	clustersMapPrime[shell]=clustersMap[shell];
	clustersMap.erase(shell);

	std::map<int, std::list<std::list<Vertex*>* > >::iterator map_it, map_it_prime;


	while (clustersMap.empty()==false) {
		map_it = clustersMap.end();
		map_it--;
		int thisShellIndex = map_it->first;
		k_conditions = thisShellIndex;
		
		map_it_prime = clustersMapPrime.end();

		std::list<std::list<Vertex*>* >::iterator Q_it;

		while (map_it_prime != clustersMapPrime.begin()) {
			//std::cout << "Entrando a un nivel de clustersMapPrime" << std::endl;
			map_it_prime--;
			int thisShellIndexPrime = map_it_prime->first;
			if (thisShellIndexPrime < thisShellIndex)
				break;

			std::list<std::list<Vertex*>* > lQ = map_it_prime->second;

			Q_it = lQ.begin();
			bool enc = false;
			while ((Q_it != lQ.end())&&(!enc)) {
				if (conditions(*Q_it))
					enc=true;
				else
					Q_it++;
			}

			//std::cout << "    Antes del while" << std::endl;
			while (Q_it != lQ.end()) {
				//std::cout << "Un cluster que cumple D" << std::endl;
				setClusterConnectivity(*Q_it, thisShellIndex);
				while (lQ.front() != *Q_it)
					lQ.pop_front();
				lQ.pop_front();
				map_it_prime->second.remove(*Q_it);

				Q_it = lQ.begin();
				bool enc = false;
				while ((Q_it != lQ.end())&&(!enc)) {
					if (conditions(*Q_it))
						enc=true;
					else
						Q_it++;
				}
			}
		}

		std::list<std::list<Vertex*>* > lQ = map_it->second;

		Q_it = lQ.begin();
		bool enc = false;
		while ((Q_it != lQ.end())&&(!enc)) {
			if (conditions(*Q_it))
				enc=true;
			else
				Q_it++;
		}

		while (Q_it != lQ.end()) {
			setClusterConnectivity(*Q_it, thisShellIndex); //Like joining Q to C
			while (lQ.front() != *Q_it) {
				lQ.pop_front();
			}
			lQ.pop_front();
			map_it->second.remove(*Q_it);

			Q_it = lQ.begin();
			bool enc = false;
			while ((Q_it != lQ.end())&&(!enc)) {
				if (conditions(*Q_it))
					enc=true;
				else
					Q_it++;
			}
		}
		clustersMapPrime[thisShellIndex].merge(clustersMap[thisShellIndex]);
		clustersMap.erase(thisShellIndex);
	}

}

void Graph_KCores_Components::computeKConnectivity(std::string type) {
	if (type == "strict")
		computeKConnectivityStrict();
	else
		computeKConnectivityWide();
}

