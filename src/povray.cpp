#include "povray.h"
#include "parameters.h"
#include <iomanip>
#include <sstream>

Povray::Povray(std::string filename) {
	out_file.open(filename.c_str(), std::fstream::out);
	r_saved = 0.0;
	u_saved = 0.0;
}
	
void Povray::addHeaders(int nc, float u, float r) {
	out_file << "#include \"colors.inc\"\n";
	out_file << "#include \"stones.inc\"\n";
	out_file << "#include \"textures.inc\"\n";
	out_file << "#include \"shapes.inc\"\n";
	out_file << "#include \"glass.inc\"\n";
	out_file << "#include \"metals.inc\"\n";
	out_file << "#include \"woods.inc\"\n";
	out_file << "global_settings { max_trace_level 10 }\n";
	out_file << "global_settings { assumed_gamma 1.8 }\n";
	out_file << "camera { orthographic location <0, 0, " << (-u * 2.0 * parameters.getGamma() * r) << "> look_at  <0, 0, 0> }\n";
	r_saved = r;
	u_saved = u;
	out_file << "light_source { < " << r << ", " << r << ", " << (-r/0.3) << "> color rgb <1.3, 1.3, 1.3>}\n";
	out_file << "global_settings { ambient_light rgb<0.8, 0.8, 0.8> }\n";
	//White or black background
	if (parameters.getBckGnd()=="white")
		out_file << "background { color rgb <5, 5, 5> }\n";
}


int Povray::getXPixel(float x) {
	return x;
}

int Povray::getYPixel(float y) {
	return y;
}

void Povray::addHost(float x, float y, float z, float ratio, tColor color, tColor borderColor, float opacity) {
	out_file << "sphere { <" << x << ", " << y << ", "<< z << ">, " << ratio << " pigment { rgb <" << color.r << ", " << color.g << ", " << color.b << "> }  }\n";
	float x1 = x;
	float x2 = x;
	float y1 = y;
	float y2 = y;
	float diameter = ratio * 1.5;
	float z1 = z + ratio + 0.1;
	float z2 = z + ratio + 0.2;

	if ((color.r != borderColor.r)&&(color.g != borderColor.g)&&(color.b != borderColor.b)) {
		out_file << std::fixed << std::setprecision(5) << "cylinder { <" << round(x1*100000)/100000 << ", " << round(y1*100000)/100000 << ", " << round(z1*100000)/100000 << ">, <" << round(x2*100000)/100000 << ", " << round(y2*100000)/100000 << ", " << round(z2*100000)/100000 << "> " << diameter << " texture { pigment { color rgbf <" << borderColor.r << ", " << borderColor.g << ", " << borderColor.b << "> } } }\n";
	}
}

void Povray::addBlock(float x, float y, float z, float ratio, tColor color) {
	out_file << "box { <" << (x-ratio) << ", " << (y-ratio) << ", " << z << ">, <" << (x+ratio) << ", " << (y+ratio) << ", 0 > pigment { rgb <" << color.r << ", " << color.g << ", " << color.b << "> }  }\n";
}

void Povray::addCircle(float x, float y, float ratio, tColor color) {
	out_file << "torus { " << ratio << " 0.002 rotate -90*x translate <" << x << "," << y << ",0> pigment { rgb <" << color.r << ", " << color.g << ", " << color.b << "> } }\n";
}
	
void Povray::addCylinder(float x1, float y1, float z1, float x2, float y2, float z2, tColor color, float opacity, float diameter) {
	//Verify that base point != apex point, for that would generate an error in povray

	bool dist1 = ((int)round(x1*100000) != (int)round(x2*100000));
	bool dist2 = ((int)round(y1*100000) != (int)round(y2*100000));
	bool dist3 = ((int)round(z1*100000) != (int)round(z2*100000));
	if (dist1 || dist2 || dist3) {
		out_file << std::fixed << std::setprecision(5) << "cylinder { <" << round(x1*100000)/100000 << ", " << round(y1*100000)/100000 << ", " << round(z1*100000)/100000 << ">, <" << round(x2*100000)/100000 << ", " << round(y2*100000)/100000 << ", " << round(z2*100000)/100000 << "> " << diameter << " open texture { pigment { color rgbt <" << color.r << ", " << color.g << ", " << color.b << ", " << (1.00-opacity) << "> } } }\n";
	}
}

void Povray::addText(std::string text, tColor color, float size, float x, float y, std::string align) {
	out_file << "text { ttf \"cyrvetic.ttf\" \"" << text.c_str() << "\" 0.37, 0 pigment { color rgbf < " << color.r << " , " << color.g << " , " << color.b << "> } translate < " << x << " , " << y - 0.3 << " , " << 0 << " > scale < " << size << " , " << size << ", " << size << " >  }\n";
}

void Povray::close() {
	//Close file
	out_file.close();
}

