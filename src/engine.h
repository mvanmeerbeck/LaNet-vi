#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <fstream>
#include "types.h"

class Engine {
  public:
	virtual void addHeaders(int nc, float u, float r) = 0;
	virtual void addHost(float x, float y, float z, float ratio, tColor color, tColor borderColor, float opacity = 1.0) = 0;
	virtual void addBlock(float x, float y, float z, float ratio, tColor color) = 0;
	virtual void addCylinder(float x1, float y1, float z1, float x2, float y2, float z2, tColor color, float opacity, float diameter) = 0;
	virtual void addText(std::string text, tColor color, float size, float x, float y, std::string align) = 0;
	virtual void addCircle(float x, float y, float ratio, tColor color) = 0;
	virtual void close() = 0;

	virtual int getXPixel(float x) = 0; //Get the pixel that corresponds to an (x,y) point in the viewBox
	virtual int getYPixel(float y) = 0; //Get the pixel that corresponds to an (x,y) point in the viewBox
};

#endif

