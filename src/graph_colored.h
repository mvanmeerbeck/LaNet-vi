#ifndef _GRAPH_COLORED_H_
#define _GRAPH_COLORED_H_

#include "vertex.h"
#include <map>

class Vertex;

class ExceptionVertexNotColored: public std::exception {
  public:
	ExceptionVertexNotColored() { };
	~ExceptionVertexNotColored() throw() {};
};

class Graph_Colored {

  protected:
	std::map<Vertex*, std::vector<double> > color;

  public:
	
	void setColor(Vertex* v, double red, double green, double blue) { 
		std::vector<double> vc;
		vc.push_back(red);
		vc.push_back(green);
		vc.push_back(blue);
		color[v] = vc;
	};
	std::vector<double> getColor(Vertex* v) { 
		std::map<Vertex*, std::vector<double> >::iterator it = color.find(v);
		if (it != color.end())
			return it->second;
		else {
			throw ExceptionVertexNotColored();
		};
	};
};

#endif

