#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <iostream>

class Parameters {
  private:
	std::string bckGnd;
	std::string color;
	std::string decomp;
	float percentOfEdges;
	int minEdges;
	int widthResolution;
	int heightResolution;
	float fontZoom;
	float epsilon;
	float delta;
	float gamma;
	float hstart;
	float hend;
	float vstart;
	float vend;
	float u;
	//Options: "lanet" or "nwb"
	std::string format;
	//Options: "" or "java"
	std::string show;
	std::string outputFile;
	std::string outputRendFile;
	std::string outputPngFile;
	std::string inputFile;
	std::string inputNodeNamesFile;
	std::string coresFile;
	std::string colorsFile;
	std::string coordDistributionAlgorithm;
	bool multigraph;
	bool weighted;
	int granularity;
	std::string renderer;
	std::string strengthsIntervals;
	double maximumStrength;
	bool circles;
	bool noCliques;
	bool kConn;
	bool connectivity;
	bool innerConnectivity;
	std::string kConnType;
	bool onlyGraphic;
	bool noGraphic;
	float opacity;
	bool isLogFile;
	int fromLayer;

	double alpha; //Parametro para graficar las componentes de los k-denses
	double beta; //Parametro para graficar las componentes de los k-denses
	int seed; //Random seed
	double minHostRatio;
	double ratioConstant;
	bool ratioConstantDefined;

	int colorScaleMaxValue;
	bool showDegreeScale;
	std::string measure;

  public:
	Parameters();
	std::string getBckGnd() { return bckGnd; }
	std::string getColor() { return color; }
	float getPercentOfEdges() { return percentOfEdges; }
	int getMinEdges() { return minEdges; }
	int getWidthResolution() { return widthResolution; }
	int getHeightResolution() { return heightResolution; }
	int getFontZoom() { return fontZoom; }
	void setAlpha(double a) { alpha = a; }
	double getAlpha() { return alpha; }
	void setBeta(double b) { beta = b; }
	double getBeta() { return beta; }
	void setSeed(int s) { seed = s; }
	int getSeed() { return seed; }
	float drawCircles() { return circles; }
	float getEpsilon() { return epsilon; }
	float getDelta() { return delta; }
	float getGamma() { return gamma; }
	float getHStart() { return hstart; }
	float getHEnd() { return hend; }
	float getVStart() { return vstart; }
	float getVEnd() { return vend; }
	float getU() { return u; }
	std::string getFormat() { return format; }
	std::string getShow() { return show; }
	std::string getOutputFile() { return outputRendFile; }
	std::string getOutputRendFile() { return outputRendFile; }
	std::string getOutputPngFile() { return outputPngFile; }
	std::string getInputFile() { return inputFile; }
	std::string getInputNodeNamesFile() { return inputNodeNamesFile; }
	std::string getCoresFile() { return coresFile; }
	std::string getColorsFile() { return colorsFile; }
	std::string getCoordDistributionAlgorithm() { return coordDistributionAlgorithm; }
	bool getMultigraph() { return multigraph; }
	bool getWeighted() { return weighted; }
	int getGranularity() { return granularity; }
	std::string getRenderer() { return renderer; }
	std::string getStrengthsIntervals() { return strengthsIntervals; }
	double getMaximumStrength() { return maximumStrength; }
	bool getNoCliques() { return noCliques; }
	bool getKConn() { return kConn; }
	bool getConnectivity() { return connectivity; }
	bool getInnerConnectivity() { return innerConnectivity; }
	std::string getKConnType() { return kConnType; }
	bool getOnlyGraphic() { return onlyGraphic; }
	bool getNoGraphic() { return noGraphic; }
	float getOpacity() { return opacity; }
	bool getIsLogFile() { return isLogFile; }
	inline int getFromLayer() { return fromLayer; }

	double getRatioConstant() { return ratioConstant; }
	double setRatioConstant(double m) { ratioConstant = m; }

	bool getRatioConstantDefined() { return ratioConstantDefined; }

	int getColorScaleMaxValue() { return colorScaleMaxValue; }
	void setColorScaleMaxValue(int c) { colorScaleMaxValue = c; }
	bool getShowDegreeScale () { return showDegreeScale; }
	void setShowDegreeScale(bool s) { showDegreeScale = s; }
	std::string getMeasure() { return measure; }
	void setMeasure(std::string m) { measure = m; }

	std::string getDecomp() { return decomp; }
	void setDecomp(std::string d) { decomp = d; }

	void printHeader();
	void printHelp();
	void validateParameters(int argc, char* argv[]);

};

//The parameters object
extern Parameters parameters;

#endif

