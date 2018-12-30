#ifndef _GRAPH_OF_TRIANGLES_H_
#define _GRAPH_OF_TRIANGLES_H_

//This is a special type of graph where edges come always three at a time, forming triangles.

#include "graph.h"
#include "graph_triangled_kcores.h"
#include <set>
#include <map>

using namespace std;

class Graph_of_triangles : public Graph {

  private:

    std::set<int> vertices_list;

    //Map of vertices of the same core
    std::map<int, std::map<int, Vertex*> > coresList;

  public:

    std::map<int, std::list<std::pair<int, int> > > neighbours;

    Graph_Triangled_KCores kcores;

    Graph_of_triangles(bool m_simple = true, bool m_weighted = false);

    void addTriangle(int v1, int v2, int v3);

    void findTriangledCores();

};

#endif

