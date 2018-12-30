#ifndef _CONNECTIVITY_H_
#define _CONNECTIVITY_H_

#include "ff_routine.h"

class Network;

typedef struct {
	std::list<Vert*> lista_num;
	std::list<void*> hijos;
	std::list<int> flujos;
	void* padre;
	int flujo_padre;
	int numero_de_hijo;
} Nodo;

typedef struct {
	Nodo* raiz;
} Arbol;

class Connectivity {

  protected:
	Arbol gomory_hu_tree;
	Network* n;

  public:
	Connectivity(Network* n1);

	void build_gomory_hu_tree(int No_of_Node);

	int find_connectivity(int v1, int v2);

	void find_connectivities();

	void find_inner_connectivities(int layer);

	int find_minimum_cut();

	void find_path(int v1, std::list<std::pair<int, int> >& camino1);

	void print_gomory_hu_tree(Vert* vertices, bool archivo=true);

};

#endif

