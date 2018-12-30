#include "svg_renderer.h"
#include "parameters.h"
#include "stdlib.h"
#include "stdio.h"

int SvgRenderer::render(std::string svgFile, std::string pngFile) {
	std::string null;

	std::string sys_type(getenv("HOME"));
	if (sys_type.find("/")!=0)
		null = "nul";
	else
		null = "/dev/null";
	
	int n = 0;

	char cadena[200];
	if (parameters.getIsLogFile()==true) {
                if (sys_type.find("/")==0) {
			sprintf(cadena, "rsvg-convert -w %d -h %d %s > %s 2> log/povray.log", parameters.getWidthResolution(), parameters.getHeightResolution(), svgFile.c_str(), pngFile.c_str());
                        n = system(cadena);
		}
        } else {
                if (sys_type.find("/")==0) {
			sprintf(cadena, "rsvg-convert -w %d -h %d %s > %s 2> %s", parameters.getWidthResolution(), parameters.getHeightResolution(), svgFile.c_str(), pngFile.c_str(), null.c_str());
                        n = system(cadena);
		}
        }

        if (n!=0) {
		std::cout << "ERROR: Error rendering image. Verify rsvg-convert is installed in your system.\n";
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
}

SvgRenderer svg_renderer;

