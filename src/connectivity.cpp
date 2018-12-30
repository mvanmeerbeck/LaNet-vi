#include "ff_routine.h"
#include <list>
#include <iostream>
#include <fstream>
#include <map>
#include "connectivity.h"
#include "network.h"
#include <queue>

typedef struct {
	int desde;
	int flujo;
	int hijo;
	int layer;
} tPilaBFS_conect;

Connectivity::Connectivity(Network* n1) {
	n = n1;
}

void imprimir_arbol_recursivo(Nodo* n, int nivel, Vert* vertices, int tam, int salto, int flujo, bool archivo) {

	for (int i=0;i<nivel;i++) logGomoryHu << " ";
	logGomoryHu << n->lista_num.front()->node_name;

	std::list<int>::iterator it = n->flujos.begin();
	while (it!=n->flujos.end()) {
		if (it!=n->flujos.begin())
			logGomoryHu << " ";
		else
			logGomoryHu << " (";
		logGomoryHu << *it;
		it++;
		if (it==n->flujos.end())
			logGomoryHu << ")";
	}
	logGomoryHu << "\n";
	std::list<void*>::iterator it2 = n->hijos.begin();
	it = n->flujos.begin();
	int ni = 0;
	while (it2!=n->hijos.end()) {
		ni++;
		imprimir_arbol_recursivo((Nodo*)*it2, nivel+1, vertices, tam, salto, *it, archivo);
		it2++;
		it++;
	}
}

void Connectivity::print_gomory_hu_tree(Vert* vert_list, bool archivo) {
	imprimir_arbol_recursivo(gomory_hu_tree.raiz, 0, vert_list, 10, 30, -1000000, archivo);
}

void flujo_maximo_recursivo(Nodo** nodop, Vert* vertices, int No_of_Node, int& i, std::map<std::pair<int, int>, int> &capacity, Arbol* arbol) {

	Nodo* nodo = *nodop;
	//std::cout << "Nodo tiene " << nodo->lista_num.size() << std::endl;

	Nodo* n1 = nodo;
	Nodo* n2 = new Nodo();
	n2->padre = n1;
	Vert* origen = nodo->lista_num.front();
	nodo->lista_num.pop_front();
	Vert* destino = nodo->lista_num.front();
	nodo->lista_num.push_front(origen);
	//std::cout << "FF de " << origen->node_name << " a " << destino->node_name << std::endl;
	int maximum_flow = Ford_Fulkerson(origen->node_i,destino->node_i, vertices, No_of_Node);
	//std::cout << "\n\nMaximum flow de " << origen->node_name << " a " << destino->node_name << "\t\t:"<<maximum_flow<<"\n\n";

	std::list<Vert*> new_lista_num;
	std::list<Vert*>::iterator it = nodo->lista_num.begin();

	bool invertir = false;
	while (it != nodo->lista_num.end()) {
		if ((*it)->bfs_status != 0) {
			//std::cout << "Nodo " << (*it)->node_name << " queda en lista 1"<< std::endl;
			if ((*it)->node_name != -1) {
				new_lista_num.push_back(*it);
			}
		}
		else {
			//std::cout << "Nodo " << (*it)->node_name << " queda en lista 2"<< std::endl;
			if ((*it)->node_name == -1) {
				invertir = true;
			}
			else {
				n2->lista_num.push_back(*it);
			}
		}
		it++;
	}
	/*if (invertir) {
		std::cout << "ENTREEE" << std::endl;
		n1->lista_num = n2->lista_num;
		n2->lista_num = new_lista_num;
	}
	else*/
		n1->lista_num = new_lista_num;


	nodo->hijos.push_back(n2);
	nodo->flujos.push_back(maximum_flow);
	n2->flujo_padre = maximum_flow;
	n2->numero_de_hijo=nodo->hijos.size() - 1;
	i++;

	Vert* v_virtual = &vertices[i];
	v_virtual->node_i = i;

	i++;

	Vert* v_virtual_2 = &vertices[i];
	v_virtual_2->node_i = i;

	v_virtual->node_name = -1;
	v_virtual_2->node_name = -1;
	v_virtual->adj = NULL;
	v_virtual_2->adj = NULL;

	Vert* v;

	struct link *e_temp, *first_e;

	it = nodo->lista_num.begin();
	//Asigno el punto común a todos los que tocan el corte
	while (it != nodo->lista_num.end()) {

		int vnode=(*it)->node_i;
		e_temp = vertices[vnode].adj;
		while(e_temp!=NULL) {
			v=(Vert*)e_temp->node;
			e_temp->flow = 0;
			e_temp->capacity = 1;
			/*if (v->node_name!=-1) {
				if (v->bfs_status!=vertices[vnode].bfs_status){
					std::cout << "Reemplazo x el virtual" << std::endl;
					if (vertices[vnode].bfs_status == 0) {
						//Cambio puntero desde v_node, reemplazando al vertice por el virtual
						e_temp->node = v_virtual;
						std::cout << ((Vert*)e_temp->node)->node_i << std::endl;
						//Creo un eje desde el vertice virtual hacia v_node
						first_e = new struct link();
						*first_e = *e_temp;
						first_e->node = &vertices[vnode];
						first_e->adj = v_virtual->adj;			
						v_virtual->adj = first_e;
					}
					else {
						e_temp->node = v_virtual_2;
						std::cout << ((Vert*)e_temp->node)->node_i << std::endl;
						first_e = new struct link();
						*first_e = *e_temp;
						first_e->node = &vertices[vnode];
						first_e->adj = v_virtual_2->adj;
						v_virtual_2->adj = first_e;
					}
				}
			}*/
			e_temp = e_temp->adj;
		}
		it++;
	}

	it = n2->lista_num.begin();
	//Asigno el punto común a todos los que tocan el corte
	while (it != n2->lista_num.end()) {
		int vnode=(*it)->node_i;
		e_temp = vertices[vnode].adj;
		while(e_temp!=NULL) {
			v=(Vert*)e_temp->node;
			e_temp->flow = 0;
			e_temp->capacity = 1;
			/*if (v->node_name!=-1) {
				if (v->bfs_status!=vertices[vnode].bfs_status){
					std::cout << "Reemplazo " << ((Vert*)e_temp->node)->node_i << " x el virtual " << v_virtual->node_i << std::endl;
					if (vertices[vnode].bfs_status == 0) {
						e_temp->node = v_virtual;
						std::cout << ((Vert*)e_temp->node)->node_i << std::endl;
						first_e = new struct link();
						*first_e = *e_temp;
						first_e->node = &vertices[vnode];
						first_e->adj = v_virtual->adj;			
						v_virtual->adj = first_e;
					}
					else {
						e_temp->node = v_virtual_2;
						std::cout << ((Vert*)e_temp->node)->node_i << std::endl;
						first_e = new struct link();
						*first_e = *e_temp;
						first_e->node = &vertices[vnode];
						first_e->adj = v_virtual_2->adj;
						v_virtual_2->adj = first_e;
					}
				}
			}*/
			e_temp = e_temp->adj;
		}
		it++;
	}

	limpiar(vertices, No_of_Node);

	//std::ofstream ff;
	//imprimir_arbol(*arbol, vertices, false);
	
	if (n1->lista_num.size() > 1) {
		//std::cout << "Hijo1 " << n1->lista_num.size() << std::endl;
		nodo->lista_num.push_back(v_virtual);
		flujo_maximo_recursivo(&n1,vertices, No_of_Node, i, capacity, arbol);
	}

	if (n2->lista_num.size() > 1) {
		//std::cout << "Hijo2 " << n2->lista_num.size() << std::endl;
		n2->lista_num.push_back(v_virtual_2);
		flujo_maximo_recursivo(&n2,vertices, No_of_Node, i, capacity, arbol);
	}

}

void Connectivity::build_gomory_hu_tree(int No_of_Node) {
	Vert* vert_list;
	std::map<std::pair<int, int>, int> capacity;
	vert_list=read_input_file(n->getVertices(), vert_list, false, false, No_of_Node, capacity);
	limpiar(vert_list, No_of_Node);

	//Inicialmente tomo a la raiz;
	gomory_hu_tree.raiz = new Nodo();
	Nodo* nodo_inicial = gomory_hu_tree.raiz;

	for(int i=0;i<No_of_Node;i++){
		if (vert_list[i].node_name!=-1)
			nodo_inicial->lista_num.push_back(&vert_list[i]);
	}
	
	gomory_hu_tree.raiz->padre = NULL;

	int i = No_of_Node;

	flujo_maximo_recursivo(&nodo_inicial, vert_list, No_of_Node, i, capacity, &gomory_hu_tree);

	print_gomory_hu_tree(vert_list);
	
}

void hallar_camino_recursivo(int v1, std::list<std::pair<int, int> >& camino1, Nodo* n, bool& por_aca) {
	std::list<int>::iterator it = n->flujos.begin();
	std::list<void*>::iterator it2 = n->hijos.begin();
	it = n->flujos.begin();
	int ni = 0;
	while (it2!=n->hijos.end()) {
		if (((Nodo*)(*it2))->lista_num.front()->node_name == v1) {
			camino1.push_front(std::make_pair<int, int>(int(*it), int(((Nodo*)(*it2))->lista_num.front()->node_name)));
			por_aca = true; //Por acá era el camino
			return;
		}
		hallar_camino_recursivo(v1, camino1, (Nodo*)*it2, por_aca);
		if (por_aca==true) {
			camino1.push_front(std::make_pair<int, int>(int(*it), int(((Nodo*)(*it2))->lista_num.front()->node_name)));
			por_aca = true; //Por acá era el camino
			return;
		}
		it2++;
		it++;
	}
	por_aca = false;
}

void Connectivity::find_path(int v1, std::list<std::pair<int, int> >& camino1) {
	if (gomory_hu_tree.raiz->lista_num.front()->node_name==v1)
		return;
	bool por_aca = false;
	hallar_camino_recursivo(v1, camino1, gomory_hu_tree.raiz, por_aca);
}

int Connectivity::find_connectivity(int v1, int v2) {

	std::list<std::pair<int, int> > camino1; //Pares (peso, nodo_destino) para llegar a v1
	find_path(v1, camino1);

	std::list<std::pair<int, int> > camino2; //Pares (peso, nodo_destino) para llegar a v1
	find_path(v2, camino2);

	std::list<std::pair<int, int> >::iterator it, it2;
/*	for (it=camino1.begin();it!=camino1.end();it++) {
		std::cout << (*it).first << " " << (*it).second << std::endl;
	}
	for (it2=camino2.begin();it2!=camino2.end();it2++) {
		std::cout << (*it2).first << " " << (*it2).second << std::endl;
	}
*/
	it=camino1.begin();
	it2=camino2.begin();
	int min_camino = -1; //Infinito

	while ((it!=camino1.end())&&(it2!=camino2.end())&&((*it).second==(*it2).second)) {
		it++;
		it2++;

	}

	for (1;it!=camino1.end();it++) {
		if (((*it).first<min_camino)||(min_camino==-1))
			min_camino = (*it).first;
		//std::cout << (*it).first << " " << (*it).second << std::endl;
	}

	for (1;it2!=camino2.end();it2++) {
		if (((*it2).first<min_camino)||(min_camino==-1))
			min_camino = (*it2).first;
		//std::cout << (*it2).first << " " << (*it2).second << std::endl;
	}

	return min_camino;
}

int min(int a, int b) {
	if (a<b)
		return a;
	return b;
}

int contador=0;
std::map<std::pair<int, int>, int> mapa_si_conn;
//std::map<std::pair<int, double>, int> mapa_si_conn_norm; //Conectividad normalizada por el producto de los tamaños de los clusters
std::vector<double> mapa_si_kconn; //Cantidad de pares kconectados por minShellIndex
std::vector<double> mapa_si_nokconn; //Cantidad de pares no kconectados por minShellIndex
std::map<std::pair<void*, void*>, double> mapa_cluster_cluster_conn; //Cluster-cluster-conn norm;

//Entrada representa por donde entré al nodo actual. 0,1,...,(n-1) para los n hijos. -1 para el padre.
void extender_guardado_conectividades(Network &graph, Nodo* desde, Nodo* actual, int flujo, int entrada, int layer) {
	//logGomoryHu << desde->lista_num.front()->node_name << " " << actual->lista_num.front()->node_name << " " << flujo << "\n";

	Vertex* d;
	Vertex* a;
	d = graph.getVertex(desde->lista_num.front()->node_name);
	a = graph.getVertex(actual->lista_num.front()->node_name);

	int minShellIndexPair = graph.kcores.getKCoresProperties(d);
	int minKConnectivityPair = graph.kcores_components.getKConnectivity(d);

	if (graph.kcores.getKCoresProperties(a)<minShellIndexPair) minShellIndexPair = graph.kcores.getKCoresProperties(a);
//	if (graph.getVertex(actual->lista_num.front()->node_name)->getKConnectivity()<minKConnectivityPair) minKConnectivityPair = graph.getVertex(actual->lista_num.front()->node_name)->getKConnectivity();


	//std::cout << d->getNumber() << " " << a->getNumber() << " " << d->getDegree() << " " << a->getDegree() << " " << graph.kcores.getKCoresProperties(d).shellIndex << " " << graph.getKCoresProperties(a).shellIndex << " " << d->getCluster() << " " << a->getCluster() << " " << flujo << " " << graph.kcores.getIsInC(d) << " " << graph.kcores.getIsInC(a) << "\n";


	//If not inner, or inner but the vertex is in the border
	if ((layer==-1)||(minShellIndexPair==layer)) {

		std::pair<int, int> p(minShellIndexPair, flujo);
		std::pair<void*, void*> p3(a->getCluster(), d->getCluster());
		std::pair<void*, void*> p4(d->getCluster(), a->getCluster());
		if (mapa_si_conn.find(p)!=mapa_si_conn.end()) {
			mapa_si_conn[p]++;
		}
		else {
			mapa_si_conn[p] = 1;
		}
/*
		if (a->getCluster()!=d->getCluster()) {
			if (mapa_cluster_cluster_conn.find(p3)!=mapa_cluster_cluster_conn.end()) {
				mapa_cluster_cluster_conn[p3] += (double)flujo/(2*d->getCluster()->size()*a->getCluster()->size());
				mapa_cluster_cluster_conn[p4] += (double)flujo/(2*d->getCluster()->size()*a->getCluster()->size());
			}
			else {
				mapa_cluster_cluster_conn[p3] = (double)flujo/(2*d->getCluster()->size()*a->getCluster()->size());
				mapa_cluster_cluster_conn[p4] = (double)flujo/(2*d->getCluster()->size()*a->getCluster()->size());
			}
		}
		else {
			if (mapa_cluster_cluster_conn.find(p3)!=mapa_cluster_cluster_conn.end()) {
				mapa_cluster_cluster_conn[p3] += (double)flujo/(d->getCluster()->size()*(a->getCluster()->size()-1));
			}
			else {
				mapa_cluster_cluster_conn[p3] = (double)flujo/(d->getCluster()->size()*(a->getCluster()->size()-1));
			}
		}

*/
		//Conectividad normalizada
	/*	std::pair<int, double> p2(minShellIndexPair, (double)flujo/(d->getCluster()->size()*a->getCluster()->size()));
		if (mapa_si_conn_norm.find(p2)!=mapa_si_conn_norm.end()) {
			mapa_si_conn_norm[p2]++;
		}
		else {
			mapa_si_conn_norm[p2] = 1;
		}
	*/
	}
	
	contador++;
	if ((contador%1000000)==0)
		std::cout << contador << std::endl;
	if ((actual->padre!=NULL)&&(entrada!=-1))
		extender_guardado_conectividades(graph, desde, (Nodo*)actual->padre, min(flujo, actual->flujo_padre), actual->numero_de_hijo, layer);

	std::list<int>::iterator it = actual->flujos.begin();
	std::list<void*>::iterator it2 = actual->hijos.begin();
	while (it2!=actual->hijos.end()) {
		if (((Nodo*)*it2)->numero_de_hijo!=entrada)
			extender_guardado_conectividades(graph, desde, (Nodo*)*it2, min(flujo, *it), -1, layer);
		it2++;
		it++;
	}
}

void hallar_conectividades_desde(Network &graph, Nodo* n, int layer) {
//	std::queue<tPilaBFS_conect> pila;
//	pila.desde = (Nodo*)n->padre;
//	pila.flujo = ((Nodo*)n)->flujo_padre;
//	pila.hijo = n->numero_de_hijo;
//	pila.layer = layer;
	if (n->padre!=NULL)
		extender_guardado_conectividades(graph, n, (Nodo*)n->padre, ((Nodo*)n)->flujo_padre, n->numero_de_hijo, layer);

	std::list<int>::iterator it = n->flujos.begin();
	std::list<void*>::iterator it2 = n->hijos.begin();
	it = n->flujos.begin();
	while (it2!=n->hijos.end()) {
		extender_guardado_conectividades(graph, n, (Nodo*)*it2, *it, -1, layer);
		it2++;
		it++;
	}

	it = n->flujos.begin();
	it2 = n->hijos.begin();

	while (it2!=n->hijos.end()) {
		hallar_conectividades_desde(graph, (Nodo*)*it2, layer);
		it2++;
		it++;
	}
}

void Connectivity::find_connectivities() {
	hallar_conectividades_desde(*n, gomory_hu_tree.raiz, -1);

	logGomoryHu2 << "#Shell Index - Connectivity - Amount of pairs\n";
	logGomoryHu3 << "#Shell Index - Not KConnected amount of pairs - KConnected amount of pairs\n";
//	logGomoryHu4 << "#Shell Index - Inter-Cluster Connectivity Normalized\n";
//	logGomoryHu5 << "#Shell Index - Average Inter-Cluster Connectivity Normalized\n";

	//Logueo en gomory-hu2: (si, conectividad real, cantidad) (Por Gomory Hu)
	std::map<std::pair<int, int>, int>::iterator it;
	for (it=mapa_si_conn.begin();it!=mapa_si_conn.end();it++) {
		logGomoryHu2 << it->first.first << " " << it->first.second << " " << it->second/2 << "\n";
	}
/*	std::map<std::pair<int, double>, int>::iterator it_norm;
	for (it_norm=mapa_si_conn_norm.begin();it_norm!=mapa_si_conn_norm.end();it_norm++) {
		logGomoryHu4 << it_norm->first.first << " " << it_norm->first.second << " " << it_norm->second << "\n";
	}
*/
/*	std::map<std::pair<void*, void*>, double>::iterator it_norm;
	std::map<std::pair<int, double>, int> provisory_map;
	std::map<std::pair<int, double>, int>::iterator it_provisory_map; // ( (Shell Index, Cluster-connectivity), Amount)
	std::map<int, int> provisory_map2; // ( (Shell Index, Average Cluster-connectivity)
	std::map<int, int> provisory_map3; // ( (Shell Index, Cluster-connectivity Total Amounts)
	std::map<int, int>::iterator it_provisory_map2;
	std::map<int, int>::iterator it_provisory_map3;
//	std::cout << "Cantidad de clusters al cuadrado: " << mapa_cluster_cluster_conn.size() << std::endl;
	for (it_norm=mapa_cluster_cluster_conn.begin();it_norm!=mapa_cluster_cluster_conn.end();it_norm++) {
		int si = n->kcores.getShellIndex(((std::list<Vertex*>*)(it_norm->first.first))->front());
		if (n->kcores.getShellIndex(((std::list<Vertex*>*)(it_norm->first.second))->front())<si)
			si = n->kcores.getShellIndex(((std::list<Vertex*>*)(it_norm->first.second))->front());
//		std::cout << "Si " << ((std::list<Vertex*>*)(it_norm->first.first))->front()->getShellIndex() << ", conectividad " << it_norm->second << std::endl;
		std::pair<int, double> p(si, it_norm->second);
		if (provisory_map.find(p)==provisory_map.end())
			provisory_map[p] = 1;
		else
			provisory_map[p]++;
	}
	for (it_provisory_map=provisory_map.begin();it_provisory_map!=provisory_map.end();it_provisory_map++) {
		if (provisory_map2.find(it_provisory_map->first.first) == provisory_map2.end()) {
			provisory_map2[it_provisory_map->first.first] = it_provisory_map->first.second*it_provisory_map->second;
			provisory_map3[it_provisory_map->first.first] = it_provisory_map->second;
		}
		else {
			provisory_map2[it_provisory_map->first.first] += it_provisory_map->first.second*it_provisory_map->second;
			provisory_map3[it_provisory_map->first.first] += it_provisory_map->second;
		}
		logGomoryHu4 << it_provisory_map->first.first << " " << it_provisory_map->first.second << " " << it_provisory_map->second << "\n";
	}

	it_provisory_map3=provisory_map3.begin();
	for (it_provisory_map2=provisory_map2.begin();it_provisory_map2!=provisory_map2.end();it_provisory_map2++) {
		logGomoryHu5 << it_provisory_map2->first << " " << (double)it_provisory_map2->second/it_provisory_map3->second << "\n";
		it_provisory_map3++;
	}
*/
	/*std::map<int, int>::iterator it2;
	int no_kconn;
	//Logueo en gomory-hu3: (si, cantidad no conexos)
	for (it2=mapa_si_kconn.begin();it2!=mapa_si_kconn.end();it2++) {
		if (mapa_si_nokconn.find(it2->first)!=mapa_si_nokconn.end())
			no_kconn = mapa_si_nokconn[it2->first];
		else
			no_kconn = 0;
		logGomoryHu3 << it2->first << " " << no_kconn << " " << it2->second << "\n";

	}*/

}

void Connectivity::find_inner_connectivities(int layer) {
	hallar_conectividades_desde(*n, gomory_hu_tree.raiz, layer);

	//It is the last layer, in innerConnectivity
	if (layer==1) {

		logGomoryHu2 << "#Shell Index - Connectivity - Amount of pairs\n";
		logGomoryHu3 << "#Shell Index - Not KConnected amount of pairs - KConnected amount of pairs\n";
		logGomoryHu4 << "#Shell Index - Connectivity Normalized - Amount of pairs\n";

		//Logueo en gomory-hu2: (si, conectividad real, cantidad) (Por Gomory Hu)
		std::map<std::pair<int, int>, int>::iterator it;
		for (it=mapa_si_conn.begin();it!=mapa_si_conn.end();it++) {
			logGomoryHu2 << it->first.first << " " << it->first.second << " " << it->second/2 << "\n";
		}
/*		std::map<std::pair<int, double>, int>::iterator it_norm;
		for (it_norm=mapa_si_conn_norm.begin();it_norm!=mapa_si_conn_norm.end();it_norm++) {
			logGomoryHu4 << it_norm->first.first << " " << it_norm->first.second << " " << it_norm->second << "\n";
		}
*/
/*		std::map<std::pair<void*, void*>, double>::iterator it_norm;
		std::map<std::pair<int, double>, int> provisory_map;
		std::map<std::pair<int, double>, int>::iterator it_provisory_map;
		std::cout << "Cantidad de clusters al cuadrado: " << mapa_cluster_cluster_conn.size() << std::endl;
		for (it_norm=mapa_cluster_cluster_conn.begin();it_norm!=mapa_cluster_cluster_conn.end();it_norm++) {
			std::pair<int, double> p(n->kcores.getShellIndex(((std::list<Vertex*>*)(it_norm->first.first))->front()), it_norm->second);
			if (provisory_map.find(p)==provisory_map.end())
				provisory_map[p] = 1;
			else
				provisory_map[p]++;
		}
		for (it_provisory_map=provisory_map.begin();it_provisory_map!=provisory_map.end();it_provisory_map++) {
			logGomoryHu4 << it_provisory_map->first.first << " " << it_provisory_map->first.second << " " << it_provisory_map->second << "\n";
		}
*/	}
}

int hallar_corte_minimo_recursivo(Nodo* n) {
	std::list<int>::iterator it = n->flujos.begin();
	std::list<void*>::iterator it2 = n->hijos.begin();
	it = n->flujos.begin();
	int f;
	int minimo = -1;
	while (it2!=n->hijos.end()) {
		f = hallar_corte_minimo_recursivo((Nodo*)*it2);
		if (((f<minimo)||(minimo==-1))&&(f!=-1))
			minimo = f;
		if ((*it<minimo)||(minimo==-1))
			minimo = *it;
		it2++;
		it++;
	}
	return minimo;
}

int Connectivity::find_minimum_cut() {
	return hallar_corte_minimo_recursivo(gomory_hu_tree.raiz);
}

