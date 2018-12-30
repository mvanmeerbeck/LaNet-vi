#include "povray_renderer.h"
#include "parameters.h"
#include "stdlib.h"
#include "stdio.h"

int PovrayRenderer::render(std::string povFile, std::string pngFile) {
	std::string null;

	std::string sys_type(getenv("HOME"));
	if (sys_type.find("/")!=0)
		null = "nul";
	else
		null = "/dev/null";
	
	int n = 0;

	char cadena[200];

	if (parameters.getIsLogFile()==true) {
                if (sys_type.find("/")!=0) {
			sprintf(cadena, "pvengine +I%s +O%s +W%d +H%d +Q9 +SC%f +EC%f +SR%f +ER%f -D /EXIT +FN 2> log/povray.log", povFile.c_str(), pngFile.c_str(), parameters.getWidthResolution(), parameters.getHeightResolution(), parameters.getVStart(), parameters.getVEnd(), parameters.getHStart(), parameters.getHEnd());
                        n = system(cadena);
                } else {
			sprintf(cadena, "povray +I%s +O%s +W%d +H%d +Q9 +SC%f +EC%f +SR%f +ER%f -D +FN 2> log/povray.log", povFile.c_str(), pngFile.c_str(), parameters.getWidthResolution(), parameters.getHeightResolution(), parameters.getVStart(), parameters.getVEnd(), parameters.getHStart(), parameters.getHEnd());
                        n = system(cadena);
                }

	} else {
	        if (sys_type.find("/")!=0) {
			sprintf(cadena, "pvengine +I%s +O%s +W%d +H%d +Q9 +SC%f +EC%f +SR%f +ER%f -D +FN /EXIT 2> %s",povFile.c_str(), pngFile.c_str(), parameters.getWidthResolution(), parameters.getHeightResolution(), parameters.getVStart(), parameters.getVEnd(), parameters.getHStart(), parameters.getHEnd(), null.c_str());
                	n = system(cadena);
                } else {
			sprintf(cadena, "povray +I%s +O%s +W%d +H%d +Q9 +SC%f +EC%f +SR%f +ER%f -D +FN 2> %s", povFile.c_str(), pngFile.c_str(), parameters.getWidthResolution(), parameters.getHeightResolution(), parameters.getVStart(), parameters.getVEnd(), parameters.getHStart(), parameters.getHEnd(), null.c_str());
                        n = system(cadena);
                }
        }

        if (n!=0) {
		std::cout << "ERROR: Error rendering image. Verify povray is installed in your system and the bin directory of the povray installation path is in the PATH environment variable.\n";
		exit(1);
	}
	
	if (parameters.getShow()=="java") {
		sprintf(cadena, "java -jar visualizer.jar %s 2> %s", pngFile.c_str(), null.c_str());
		n = system(cadena);
		if (n!=0) {
			std::cout << "ERROR: Error displaying image. Verify java is installed in your system.\n";
			exit(1);
		}
	}
	return n;
}

PovrayRenderer povray_renderer;

