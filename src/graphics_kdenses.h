#ifndef _GRAPHICS_KDENSES_H_
#define _GRAPHICS_KDENSES_H_

#include "types.h"
#include <vector>
#include "network.h"
#include "engine.h"
#include "uniform.h"

class Graphics_KDenses {

  private:
	std::vector<std::vector<int> > namesMesh;
	Uniform uniform;

	int xMeshSize;
	int yMeshSize;

	Engine* engine;

	tColor interpolate(tColor c1, double alfa1, tColor c2);
	double computeHostRatio(Network* n, double strength, int degree, bool weighted);
	tColor computeHostColorByDenseIndex(Network* n, int shellIndex);
	tColor computeHostColor(Network* n, Vertex* v);
	tColor computeHostBorderColor(Network* n, Vertex* v);
	void addCluster(std::list<Vertex*> cluster, int order, Network* n, double u, tColor textColor, int ppcV, int ppcH, double ratio, double opacity);
	void addComponents(KDenses_Component* c, int order, Network* n, double u, tColor textColor, int ppcV, int ppcH, double ratio);

	bool par(int n);

  public:
	Graphics_KDenses(Engine* engine);
	void generateNetworkFile(Network* n);
			
};

#endif

