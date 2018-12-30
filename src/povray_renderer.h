#ifndef _POVRAY_RENDERER_H_
#define _POVRAY_RENDERER_H_

#include <string>

//This class renders the .png image by calling povray
class PovrayRenderer {
  public:
	//Returns exit code (int)
	int render(std::string povFile, std::string pngFile);
};

extern PovrayRenderer povray_renderer;

#endif

