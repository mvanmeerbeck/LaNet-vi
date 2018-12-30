/*****		FORD-FULKERSON ALGORITHM IMPLEMENTATION  *****/
/** Module Name :	ff.cpp								**/
/** Date : 10 / 26 / 2005								**/
/** Description : Finding maximum flow on directed graph
					using FORD-FULKERSON algorithm		**/
/** Author :	Chinh Trung VU							**/
/** Purpose : Completing algorithm class assignment		**/

#include <fstream>
#include <iostream>
#include <ostream>
#include <ctime>
#include "ff_routine.h"
#include <map>
#include "vertex.h"

#ifndef INFINITY
	#define INFINITY 0x7fff
#endif

#ifndef cMAX_WEIGHT
	#define cMAX_WEIGHT 10000 //Maximum edge weight value
#endif

struct Queue_Node {
 int v;			// node's number
 Queue_Node *next;	// point to next element in Queue
};

// Queue structure with basic operators
/* Creating singleton set consisting only vertex x
	- input:  node number
	- output: singleton set under the form of matrix's element
*/
class Queue {
public:
	// Queue's Head and Tail
	Queue_Node *first, *last;
	Queue() {first = last = NULL;} //Class construction
	~Queue() { ClearAll();} //Class destruction
 
	// Add new node into Queue
	void Add_Queue(int k){
		//std::cout << "a" << k << std::endl;
		Queue_Node* tmpp = new Queue_Node;
		//std::cout << "a1" << std::endl;
		tmpp->v = k;
		//std::cout << "b" << std::endl;
		tmpp->next = NULL;
		//if Queue is empty, point all to new value
		//std::cout << "c" << std::endl;
		if (first == NULL) {first=last = tmpp;} // Queue initilizing step
		else {last = last->next = tmpp;}
	}

	// Remove a node from Queue and return its node's number
	int Del_Queue(){
		if (first == NULL) return NULL;
		Queue_Node *tmp = first;
		int tmp_val = tmp->v;
		first = first->next;
		delete tmp;
		return tmp_val;
	}
 
	bool Queue_Empty(){
		if (first==NULL) return true;
		else return false;
	}
	
	void ClearAll(){
		while (first != NULL) Del_Queue();
	}
	
	// for debugging: Print the queue
	int Print_Queue(){
		if (first == NULL) return 0;
		Queue_Node *tmp = first;
		std::cout << "\nPrint Queue: ";
		while (tmp!=NULL){
			std::cout << "\t" << tmp->v;
			tmp =tmp ->next;
		}
		std::cout << "\n";
	}
};


/* Read graph input from file (for testing the corresctness of code
	- input:  file name
	- output: Adjacency list
*/
Vert* read_input_file(std::list<Vertex*> &lista_vertices, Vert* vertices, bool pesado, bool dirigido, int& No_of_Node, std::map<std::pair<int, int>,int>& capacity){

	//fstream in,out;
	int i,j;

	
//	std::cout << "______________________________________________________\n\n";
//	std::cout << "Number of nodes: \t" << No_of_Node << '\n';
//	std::cout << "Number of edges: \t" << No_of_Edge << '\n';
//	std::cout << "______________________________________________________\n";
//	std::cout << '\n';


	// try to locate memory space for vertices adjacency list
	vertices = new Vert [No_of_Node * 3];
	if(!vertices) { std::cout << std::endl << "memory problem!"; return FALSE; }

	//reset all value to INFINITY
	for(i=0;i<No_of_Node * 3;i++){
/*		for(j=0;j<No_of_Node * 3;j++){
			capacity[i][j] =INFINITY;
		}
*/		vertices[i].node_name = -1;
	}

	i=0;
	int a,b;
	int ia, ib;
	int cap;

	std::map<int, int> mapa_nombre_i;

	std::list<Vertex*>::iterator it1 = lista_vertices.begin();
	std::vector<Vertex*>::iterator it2;

	//First: Push data to adjacency matrix
	while (it1!=lista_vertices.end()){
		it2 = (*it1)->getNeighbours()->begin();
		while (it2!=(*it1)->getNeighbours()->end()) {
			a = (*it1)->getNumber();
			b = (*it2)->getNumber();
			//in >> a;
			//in >> b;
			if (mapa_nombre_i.find(a) == mapa_nombre_i.end()) {
				mapa_nombre_i[a] = i;
				vertices[i].node_name = a;
				vertices[i].node_i = i;
				i++;
			}
			ia = mapa_nombre_i[a];
			if (mapa_nombre_i.find(b) == mapa_nombre_i.end()) {
				mapa_nombre_i[b] = i;
				vertices[i].node_name = b;
				vertices[i].node_i = i;
				i++;
			}
			ib = mapa_nombre_i[b];
			std::pair<int, int> par(ia, ib);
			std::pair<int, int> par_op(ib, ia);
			//TODO: Ver para grafos pesados
			/*if (pesado) {
				in >> cap;
				capacity[par] = cap;
				if (!dirigido)
					capacity[par_op] = capacity[par];
				else {
					if (capacity.find(par_op) == capacity.end())
						capacity[par_op] = 0;
				}
			}
			else*/ {
				if (capacity.find(par)==capacity.end()) {
					capacity[par] = 1;
					if (dirigido) {
						if (capacity.find(par_op) == capacity.end())
							capacity[par_op] = 0;
					}
				}
				else {
					capacity[par] += 1;
				}
			}
			it2++;
		}
		it1++;
	}

	for(i=0;i<No_of_Node * 3;i++){
		vertices[i].bfs_status=WHITE;
		vertices[i].prev=-1;
		vertices[i].adj=NULL;
	}

	struct link *e_temp, *next_e;

	std::map<std::pair<int, int>, int>::iterator it;
	for (it=capacity.begin();it!=capacity.end();it++) {
		//creating new edge
		e_temp = new struct link;
		e_temp->node=&vertices[it->first.second];
		e_temp->capacity=it->second;
		e_temp->flow=0;
		e_temp->adj=NULL;
		//link it to it adjacency list
		if (vertices[it->first.first].adj==NULL){
			vertices[it->first.first].adj=e_temp;
			next_e=vertices[it->first.first].adj;
		}
		else
		{
			next_e->adj=e_temp;
			next_e = next_e->adj;
		}
	};

	// clean temp variable
	//delete capacity;

	return vertices;
}


void limpiar(Vert* vertices, int& No_of_Node) {

	struct link* e_temp;
	for(int i=0;i<No_of_Node * 3;i++){
		e_temp = vertices[i].adj;
		//begin to find in the link list the edge [from,to]
		while (e_temp!=NULL) {
			e_temp->flow = 0;
			e_temp = e_temp->adj;
		}
	}
}

/*/FUNCTION PROTOTYPE/*/
struct link * find_edge(int from, int to,Vert *vertices);
int bfs(int start, int target, Vert *vertices, int No_of_Node);
void declare_adj_list(Vert *vertices);

/*/FUNCTION IMPLEMENTATION/*/

/* Find edge [from,to] on adjacency list.
	- input:  from, to node
	- output: edge in the form pointer of link structure
*/
struct link * find_edge(int from, int to, Vert *vertices){
	struct link *e_temp;
	e_temp = vertices[from].adj;
	//begin to find in the link list the edge [from,to]
	while ((((Vert*)e_temp->node)->node_i !=to) && (e_temp!=NULL))
		e_temp = e_temp->adj;
	return e_temp;
}

/* Breapth First Search (BFS) Traversal on RESIDUAL graph.
	- input:  residual graph. source, sink node
	- output: + sortest (in term of "no of edge") path 
				saved on the adjacency list
			  + return true if reach target, and vice versa
*/
int bfs(int start, int target, Vert *vertices, int No_of_Node){
	int i=1,j=0;
	int vnode,v;
	struct link *e_temp;
	Queue qu; //creat queue class

	// Reset all node to not visited status
	for(i=0;i<No_of_Node * 3;i++)
		vertices[i].bfs_status=WHITE;

	//Start to traverse
//	std::cout << "BFS de " << vertices[start].node_name << " a " << vertices[target].node_name << ". Agrego el " << vertices[start].node_name << std::endl;
	qu.Add_Queue(start);
	vertices[start].bfs_status=GRAY;
//	std::cout << "********EMPIEZO BFS***********" << std::endl;
	while(!qu.Queue_Empty()) // If queue empty, we stop!
	{  	vnode = qu.Del_Queue();
//		std::cout << "Tomo el " << vertices[vnode].node_name << std::endl;
		vertices[vnode].bfs_status=BLACK; //node vnode is visited
		// Search all adjacent WHITE nodes v of vnode
		e_temp = vertices[vnode].adj;
		while(e_temp!=NULL) {
			v=((Vert*)(e_temp->node))->node_i;
//			std::cout << "Nodo " << vertices[v].node_name << " tiene estatus WHITE " << (((Vert*)e_temp->node)->bfs_status==WHITE) << " capacity " << e_temp->capacity << ", buscando flow " << e_temp->flow << std::endl;
			// We only care edges in the RESIDUAL graph having POSITIVE RESIDUAL CAPACITY
			if ( (((Vert*)e_temp->node)->bfs_status==WHITE) && (e_temp->capacity > e_temp->flow) ){
				qu.Add_Queue(v);
				((Vert*)e_temp->node)->bfs_status=GRAY; //show that node v is waiting in queue
				((Vert*)e_temp->node)->prev = vnode;

				//Small trick: If we reach to target, paint it BLACK and stop
				if (v==target){
					vertices[target].bfs_status=BLACK;
					qu.ClearAll(); //remove Queue from memory
					break;
				}
			}
			e_temp = e_temp->adj;
		}
	}
	// The color of the target node is black means it was reached.
	return (vertices[target].bfs_status==BLACK);
}

/* Main FORD-FULKERSON methods, EDMONDS-KARP algorithm [Cormen]
	- input:  residual graph. source, sink node
	- output: return Maximum flow
*/
double Ford_Fulkerson (int source, int sink, Vert *vertices, int No_of_Node) {

    int u;
	struct link *e_temp;
    double max_flow = 0;
	
//std::cout << "******************************************FF************************************" << std::endl;
	// idea: While there exists an augmenting path, increment the flow along this path.
    while (bfs(source,sink, vertices, No_of_Node)) { //1. Find shortest path
        // 2. Determine the amount by which we can increment the flow.
		int increment = (int)INFINITY;
		int minimum;
		u=sink;
//		std::cout << "Nodo " << u << std::endl;
		//std::cout << "Vecinos: " << ((Vert*)vertices[u].adj->node)->node_i;
		//std::cout << " " << ((Vert*)vertices[u].adj->adj->node)->node_i;
		//std::cout << std::endl;		
		while (u!=source){ //go along shortest path from sink to source
			e_temp = find_edge(vertices[u].prev,u, vertices);
			minimum = increment;
			if (minimum > e_temp->capacity - e_temp->flow)
				minimum = e_temp->capacity - e_temp->flow;
			increment = minimum;
			u = vertices[u].prev;
//			std::cout << "Nodo " << u << std::endl;
		}
		
		// 3. Increment the flow along the found shortest path.
		u=sink;
//		std::cout << "INCREMENTO " << increment << std::endl;
		while (u!=source){ //go along shortest path from sink to source
			//3.1: increment flow edge [prev_u,u] if [prev_u,u] on shortest path
			e_temp = find_edge(vertices[u].prev,u,vertices);
			e_temp->flow+= increment;
			//3.2: decrement flow edge [u,prev_u] if [prev_u,u] on shortest path
			e_temp = find_edge(u,vertices[u].prev,vertices);
			e_temp->flow-= increment;

			u = vertices[u].prev; //continue crawling to source
		}
		max_flow += double(increment);
//		std::cout << "Fin INCREMENTO. Flujo: " << max_flow << std::endl;
    }
	return max_flow;
}

/* Delete adjacency list vertices after each running time estimation
	- input:  vertices matrix
	- output: return true if successful and vice versa
*/
void remove_adj_list(Vert *vertices, int No_of_Node){
	struct link *e_temp;
	struct link *next_e;
	// go through all list elements
	for(int i=0;i<No_of_Node;i++){
		//delete all adjacent nodes of each element
		e_temp = vertices[i].adj;
		while (e_temp!=NULL){
			next_e=e_temp->adj;
			if (next_e!=NULL)
				delete e_temp;
			e_temp = next_e;
		}
	}
	delete e_temp;
	delete next_e;
	delete vertices;
}


