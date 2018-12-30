#ifndef _TYPES_H_
#define _TYPES_H_

#include "log.h"
#include "math.h"
#include <vector>
#include <map>

class Community;

extern double pi;

//A struct for accesing the best neighbour of a community in terms of submodularity
class bestNeighbour {
  public:
	Community* C1;
	Community* C2;
	double num, den;
};

int max(int a, int b);

typedef bestNeighbour* bestNeighbourPtr;
extern bool bestNeighbourOrder(const bestNeighbourPtr& bn1, const bestNeighbourPtr& bn2);

class Compare {
  public:
	bool operator()(const bestNeighbourPtr& bn1, const bestNeighbourPtr& bn2) { return bestNeighbourOrder(bn1, bn2);};
};

typedef struct {
	double xi, yi;
	int dirxi, diryi;
} tLetra;

typedef struct {
	double r, g, b;
} tColor;

//Rainbow colors
extern tColor magenta;
extern tColor blue;
extern tColor cyan;
extern tColor green;
extern tColor yellow;
extern tColor red;

//B&W scale
extern tColor white;
extern tColor gray;
extern tColor black;

typedef struct {
	double xi, yi, dirxi, diryi;
} tImagePosition;

//Colors list for color images
extern std::vector<std::pair<tColor, double> > rainbow;

//Colors list for B&W images
extern std::vector<std::pair<tColor,double> > blackwhite;

extern Log logCore;
extern Log logKConnectivity;
extern Log logKConn2;
extern Log logKConn3;
extern Log logGomoryHu;
extern Log logGomoryHu2;
extern Log logGomoryHu3;
extern Log logGomoryHu4;
extern Log logGomoryHu5;
extern Log logQ;
extern Log logCommunities;
extern Log logCoords;
extern Log result_communities;
extern Log logPixels;

extern std::map<std::pair<int, int>, int> mapa_si_conn;
extern std::map<std::pair<int, double>, int> mapa_si_conn_norm; //Conectividad normalizada por el producto de los tamaños de los clusters
extern std::vector<double> mapa_si_kconn; //Cantidad de pares kconectados por minShellIndex
extern std::vector<double> mapa_si_nokconn; //Cantidad de pares no kconectados por minShellIndex
extern std::map<std::pair<void*, void*>, double> mapa_cluster_cluster_conn; //Cluster-cluster-conn norm;

double min(double a, double b);
double max(double a, double b);

#endif

