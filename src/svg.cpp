#include "svg.h"
#include "parameters.h"

Svg::Svg(std::string filename) {
	out_file.open(filename.c_str(), std::fstream::out);
	r_saved = 0.0;
	u_saved = 0.0;
}
	
void Svg::addHeaders(int nc, float u, float r) {
	out_file << "<?xml version=\"1.0\" standalone=\"no\"?>\n";
	out_file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n";
  	out_file << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";

	u_saved = u;
	r_saved = u * 2.0 * parameters.getGamma() * r;
	float ratio = r_saved;
	float width = 1.6 * ratio;
	float height = 1.2 * ratio;
	out_file << "<svg width=\"" << parameters.getWidthResolution() << "\" height=\"" << parameters.getHeightResolution() << "\" viewBox=\"" << (-0.8*r_saved + parameters.getHStart() * width) << " " << (-0.6*r_saved + parameters.getVStart() * height) << " " << (width * (parameters.getHEnd() - parameters.getHStart())) << " " << (height*(parameters.getVEnd() - parameters.getVStart())) << "\" version=\"1.1\"\n";
	out_file << "xmlns=\"http://www.w3.org/2000/svg\">\n";
	if (parameters.getBckGnd() != "white")
		out_file << "<rect x=\"-" << (0.8*r_saved) << "\" y=\"-" << (0.6*r_saved) << "\" width=\"" << (1.6*r_saved) << "\" height=\"" << (1.2*r_saved) << "\" fill=\"rgb(0,0,0)\"/>\n";
	else
		out_file << "<rect x=\"-" << (0.8*r_saved) << "\" y=\"-" << (0.6*r_saved) << "\" width=\"" << (1.6*r_saved) << "\" height=\"" << (1.2*r_saved) << "\" fill=\"rgb(255,255,255)\"/>\n";
}

int Svg::getXPixel(float x) {
	float width = 1.6 * r_saved;
	float height = 1.2 * r_saved;
	return (x - (-0.8*r_saved + parameters.getHStart() * width))/(1.6*r_saved*(parameters.getHEnd() - parameters.getHStart()))*parameters.getWidthResolution();
}

int Svg::getYPixel(float y) {
	float width = 1.6 * r_saved;
	float height = 1.2 * r_saved;
	return (-y - (-0.6*r_saved + parameters.getVStart() * height))/(1.2*r_saved*(parameters.getVEnd() - parameters.getVStart()))*parameters.getHeightResolution();
}

void Svg::addHost(float x, float y, float z, float ratio, tColor color, tColor borderColor, float opacity) {

	float x1 = x;
	float x2 = x;
	float y1 = y;
	float y2 = y;
	float diameter = ratio * 1.3;
	float z1 = z + ratio + 0.1;
	float z2 = z + ratio + 0.2;

	if ((color.r != borderColor.r)||(color.g != borderColor.g)||(color.b != borderColor.b)) {
		out_file << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" fill=\"rgb(" << round(borderColor.r * 255) << "," << round(borderColor.g * 255) << "," << round(borderColor.b * 255) << ")\" r=\"" << diameter << "\" opacity=\"" << opacity << "\"/>\n";
	}

	out_file << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" fill=\"rgb(" << round(color.r * 255) << "," << round(color.g * 255) << "," << round(color.b * 255) << ")\" r=\"" << ratio << "\" opacity=\"" << opacity << "\"/>\n";

}

void Svg::addBlock(float x, float y, float z, float ratio, tColor color) {
	out_file << "<rect x=\"" << (x - ratio) << "\" y=\"" << (-y + ratio) << "\" fill=\"rgb(" << round(color.r * 255) << "," << round(color.g * 255) << "," << round(color.b * 255) << ")\" width=\"" << (2 * ratio) << "\" height=\"" << (2 * ratio) << "\"/>\n";
}

void Svg::addCylinder(float x1, float y1, float z1, float x2, float y2, float z2, tColor color, float opacity, float diameter) {
	//Verify that base point != apex point, for that would generate an error in povray
	if ((floor((x1+0.0005)*1000) != floor((x2+0.0005)*1000)) || (floor((y1+0.0005)*1000) != floor((y2+0.0005)*1000)))
		out_file << "<line x1=\"" << x1 << "\" x2=\"" << x2 << "\" y1=\"" << (-y1) << "\" y2=\"" << (-y2) << "\" stroke=\"rgb(" << round(color.r * 255) << "," << round(color.g * 255) << "," << round(color.b * 255) << ")\" stroke-width=\"" << (diameter*2) << "\" opacity=\"" << opacity << "\"/>\n";
}

void Svg::addText(std::string text, tColor color, float size, float x, float y, std::string align) {
	out_file << "<text x=\"" << (x*size) << "\" y=\"" << ((-y)*size) << "\" font-family=\"Verdana\" font-size=\"" << size << "\" text-anchor=\"" << align << "\" fill=\"rgb(" << round(color.r * 255) << "," << round(color.g * 255) << "," << round(color.b * 255) << ")\">" << text.c_str() << "</text>\n";
}

void Svg::addCircle(float x, float y, float ratio, tColor color) {
//	out_file << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" stroke-width=\"0.006\" stroke=\"rgb(" << round(color.r*255) << "," << round(color.g*255) << "," << round(color.b*255) << ")\" fill=\"none\" r=\"" << ratio << "\"/>";
	out_file << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" stroke-width=\"0.006\" stroke=\"rgb(" << round(color.r*255) << "," << round(color.g*255) << "," << round(color.b*255) << ")\" fill=\"rgb(" << round(pow(color.r, 0.1)*255) << "," << round(pow(color.g, 0.1)*255) << "," << round(pow(color.b, 0.1)*255) << ")\" r=\"" << ratio << "\"/>";
}

void Svg::close() {
	out_file << "</svg>\n";
	//Close file*)
	out_file.close();
}

