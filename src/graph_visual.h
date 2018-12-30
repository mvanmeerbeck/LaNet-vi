#ifndef _GRAPH_VISUAL_H_
#define _GRAPH_VISUAL_H_

#include "vertex.h"
#include <map>

class Vertex;

typedef struct {
	float rho, phi, x, y, ratio;
	tColor color, bordercolor;
} tVisualProperties;

class Graph_Visual {

  protected:
	std::map<Vertex*, tVisualProperties> visualProperties;
	bool coordinatesComputed;

  public:
	
	void setVisualProperties(Vertex* v, tVisualProperties vp) { visualProperties[v] = vp; };
	tVisualProperties getVisualProperties(Vertex* v) { return visualProperties[v]; };
	void setVertexRho(Vertex* v, float r) { visualProperties[v].rho = r; };
	void setVertexPhi(Vertex* v, float p) { visualProperties[v].phi = p; };
	void setVertexX(Vertex* v, float xi) { visualProperties[v].x = xi; };
	float getVertexX(Vertex* v) { return visualProperties[v].x; };
	void setVertexY(Vertex* v, float yi) { visualProperties[v].y = yi; };
	float getVertexY(Vertex* v) { return visualProperties[v].y; };
	void setVertexRatio(Vertex* v, float r) { visualProperties[v].ratio = r; };
	float getVertexRatio(Vertex* v) { return visualProperties[v].ratio; };
	void setVertexColor(Vertex* v, tColor c) { visualProperties[v].color = c; };
	void setVertexBorderColor(Vertex* v, tColor c) { visualProperties[v].bordercolor = c; };
	tColor getVertexColor(Vertex* v) { return visualProperties[v].color; };
	bool getCoordinatesComputed() { return coordinatesComputed; };
	bool setCoordinatesComputed(bool b = true) { coordinatesComputed = b; };

};

#endif

