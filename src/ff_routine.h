#ifndef _FF_ROUTINE_
#define _FF_ROUTINE_

#include <map>
#include <list>

class Vertex;

/*/USEFUL FUNCTION/*/
#define clrscr() system("clear");

/*/ BASIC DEFINITIONS /*/
typedef int dat_type;	// data type for capacity, flow, ...
//for bfs
#define TRUE       1
#define FALSE      0
	#define WHITE 0		// node is not visited
	#define GRAY 1		// node is not visited but wait in line (on queue)
	#define BLACK 2		// node visited
//for graph input

	#ifndef INFINITY
		#define INFINITY 0x7fff
	#endif
//for estimating running time
//	#define NO_LOOP 50 //1000

/*/ ADJACENCY LIST DATA STRUCTURE/*/
struct link  // Structure for adjacency links (edges)
{	
	dat_type capacity;	// link capacity
	dat_type flow;		// flow after each time augmenting
	void* node;		// name of the end node of the edge
	struct link *adj;	// the next edge
};

struct node_detail		// Structure for nodes
{
	int node_name;		// name of the node
	int node_i;		// i of the node
	int bfs_status,prev;// for bfs use
	struct link *adj;	// next edge
};
typedef struct node_detail Vert;
// ~ End of ADJACENCY LIST DATA STRUCTURE\\

Vert* read_input_file(std::list<Vertex*> &lista_vertices, Vert *vertices, bool pesado, bool dirigido, int& No_of_Node, std::map<std::pair<int, int>, int> &capacity);
void limpiar(Vert *vertices, int& No_of_Node);
double Ford_Fulkerson (int source, int sink, Vert *vertices, int No_of_Node);
void remove_adj_list(Vert *vertices, int No_of_Node);
bool randomize_input(Vert* vertices, int No_of_Node);

#endif

