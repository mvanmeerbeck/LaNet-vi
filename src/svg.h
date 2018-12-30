#ifndef _SVG_H_
#define _SVG_H_

#include <fstream>
#include "types.h"
#include "engine.h"

class Svg : public Engine {
  private:
	std::ofstream out_file;
	float r_saved;
	float u_saved;

  public:
	Svg(std::string filename);
	void addHeaders(int nc, float u, float r);
	void addHost(float x, float y, float z, float ratio, tColor color, tColor borderColor, float opacity = 1.0);
	void addBlock(float x, float y, float z, float ratio, tColor color);
	void addCylinder(float x1, float y1, float z1, float x2, float y2, float z2, tColor color, float opacity, float diameter);
	void addText(std::string text, tColor color, float size, float x, float y, std::string align);
	void addCircle(float x, float y, float ratio, tColor color);
	void close();

	int getXPixel(float x); //Get the pixel that corresponds to an (x,y) point in the viewBox
	int getYPixel(float y); //Get the pixel that corresponds to an (x,y) point in the viewBox
};

#endif

