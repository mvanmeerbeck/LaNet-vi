#ifndef _SVG_RENDERER_H_
#define _SVG_RENDERER_H_

#include <string>

//This class renders the .svg image by calling the svg renderer
class SvgRenderer {
  public:
	//Returns exit code (int)
	int render(std::string svgFile, std::string pngFile);
};

extern SvgRenderer svg_renderer;

#endif

