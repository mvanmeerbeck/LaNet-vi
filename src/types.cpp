#include "log.h"
#include "types.h"
#include "parameters.h"

//Rainbow colors
tColor magenta = { 1.0, 0.2, 1.00};
tColor blue = { 0.2, 0.2, 1.00};
tColor cyan = { 0.2, 1.0, 1.00};
tColor green = { 0.2, 1.0, 0.2};
tColor yellow = { 1.0, 1.0, 0.00};
tColor red = { 1.0, 0.2, 0.2};

//Colors list for color images
std::vector<std::pair<tColor, double> > rainbow;

//B&W scale
tColor white = { 2.0, 2.0, 2.0};
tColor gray = { 0.7, 0.7, 0.7};
tColor black = { 0.0, 0.0, 0.0}; 

//Colors list for B&W images
std::vector<std::pair<tColor,double> > blackwhite;

int max(int a, int b) {
	if (a>b)
		return a;
	return b;
}

double pi = 4.0 * atan(1.0);

class Init {
  public:
	Init() {
		rainbow.push_back(std::make_pair<tColor, double>(tColor(magenta), 0.01));
		rainbow.push_back(std::make_pair<tColor, double>(tColor(blue), 0.20));
		rainbow.push_back(std::make_pair<tColor, double>(tColor(cyan), 0.35));
		rainbow.push_back(std::make_pair<tColor, double>(tColor(green), 0.50));
		rainbow.push_back(std::make_pair<tColor, double>(tColor(yellow), 0.65));
		rainbow.push_back(std::make_pair<tColor, double>(tColor(red), 0.97));

		blackwhite.push_back(std::make_pair<tColor, double>(tColor(white), -0.75));
		blackwhite.push_back(std::make_pair<tColor, double>(tColor(gray), 0.41));
		blackwhite.push_back(std::make_pair<tColor, double>(tColor(black), 1.23));
	}
};

Init init;

Log logCore("log/cores.log");
Log logKConnectivity("log/kconn.log");
//Log logKConn2("log/kconn2.log");
//Log logKConn3("log/kconn3.log");
Log logGomoryHu("log/gomory_hu.log");
Log logGomoryHu2("log/gomory_hu2.log");
Log logGomoryHu3("log/gomory_hu3.log");
Log logGomoryHu4("log/gomory_hu4.log");
Log logGomoryHu5("log/gomory_hu5.log");
Log logCoords("log/coordinates.log");
Log logPixels("log/pixels.log");

