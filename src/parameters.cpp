#include "parameters.h"
#include <sstream>
#include "stdlib.h"
#include "string.h"
#include <fstream>

Parameters parameters;

Parameters::Parameters() {
	ratioConstant = 1;
	ratioConstantDefined = false;
	bckGnd = "black";
	color = "col";
	decomp = "kcores";
	percentOfEdges = 0.0;
	minEdges = 1000;
	widthResolution = 800;
	heightResolution = 600;
	fontZoom = 1.0;
	epsilon = 0.0;
	delta = 1.0;
	gamma = 1.3;

epsilon = 0.18;
delta = 1.3;
gamma = 1.5;

	hstart = 0.0;
	hend = 1.0;
	vstart = 0.0;
	vend = 1.0;
	u = 1.0;
	format = "lanet";
	//Options: "" or "java"
	show = "";
	outputFile = "";
	outputRendFile = "";
	outputPngFile = "";
	inputFile = "";
	inputNodeNamesFile = "";
	coresFile = "";
	colorsFile = "";
	coordDistributionAlgorithm = "classic";
	multigraph = false;
	weighted = false;
	granularity = -1;
	renderer = "povray";
	strengthsIntervals = "equalIntervalSize";
	maximumStrength = -1;
	noCliques = false;
	circles = false;
	alpha = 0.3;
	beta = 1.0;
	seed = 0;
	kConn = false;
	kConnType = "wide";
	connectivity = false;
	innerConnectivity = false;
	onlyGraphic = false;
	noGraphic = false;
	opacity = 0.2;
	isLogFile = false;
	fromLayer = 0;

	colorScaleMaxValue = -1;
	showDegreeScale = true;
	measure = "mcore";

}

void Parameters::printHeader() {
	std::cout << "\nLaNet-vi 3.0.1n";
	std::cout << "--------------\n\n";
	std::cout << "Large scale networks visualization tool using k-core and k-dense (m-core) decomposition.\nSoftware license: Academic Free License (AFL)\nImages license: Creative Commons License";
	std::cout << "\nhttp://xavier.informatics.indiana.edu/lanet-vi/\n\n";
}

void Parameters::printHelp() {
	std::cout << "Parameters:\n";
	std::cout << "-input <file>: Name of input file containing the network edges. Required.\n";
	std::cout << "-decomp kcores|kdenses: Type of decomposition to be applied. Optional. Possible values: 'kcores', 'kdenses' (see parameter '-measure'). Default: 'kcores'.\n";
	std::cout << "-names <file>: Name of input file containing the nodes' names. A name \"0\" indicates LaNet to show the nodes' numbers. Optional.\n";
	std::cout << "-font <value>: Zoom in font for nodes' names. Default: 1.0. Optional.\n";
	std::cout << "-output <file>: Name of output .pov and .png files. Optional. Default: <inputFileName>.pov and <inputFileName>.png\n";
	std::cout << "-coresfile <file>: Generates a k-core decomposition file with (node number, shell-index). Optional. Default: no file\n";
	std::cout << "-colorsFile <file>: Name of input file containing colors for painting each node. Format: <node> <red> <green> <blue>. RGB values must be in the range [0.00-1.00]. Optional. Default: no file\n";
	std::cout << "-coordDistributionAlgorithm: Algorithm for the distribution of components. Possible values: 'classic', 'pow', 'log'. Optional. Default: 'classic'.\n";
	std::cout << "-logfile: Generate log files in log directory. Optional.\n";
	std::cout << "-logstdout: Generate log information on stdout. Optional.\n";
	std::cout << "-multigraph: Allows repeated edges.  Combined with 'weighted' allows to specify repeated edges as weight. Optional.\n";
	std::cout << "-weighted: Allows weights on edges (for weighted graphs). Combined with 'multigraph' allows to specify repeated edges as weight (in this case weight must be integer). Optional.\n";
	std::cout << "-strengthsIntervals: How to build p-function intervals on weighted graphs. Possible values: 'equalNodesPerInterval' or 'equalIntervalSize' or 'equalLogIntervalSize'. Optional. Default: equalIntervalSize\n";
	std::cout << "-maximumStrength: Upper limit for the strength intervals. May be useful for normalizing pictures of different networks. Optional. Default: NONE\n";
	std::cout << "-granularity: Amount of groups in weighted graphs. Optional. Default: maximum degree\n";
	std::cout << "-bckgnd <value>: Background color. Possible values: 'white' and 'black'. Optional. Default: white\n";
	std::cout << "-color <value>: Graph color. Possible values: 'col', 'bw' (black and white, one scale) and 'bwi' (black and white, interlaced scale). Optional. Default: col\n";
	std::cout << "-eps <value>: Epsilon. Controls the possibility of rings overlapping. Used to tune image's rendering. Optional. Default: 0.18\n";
	std::cout << "-delta <value>: Delta. Controls distance between components in graphical visualization. Optional. Default: 1.3\n";
	std::cout << "-gamma <value>: Gamma. Controls the component's diameter. Optional. Default: 1.5\n";
	std::cout << "-fromlayer <value>: Considers the graph induced from this layer up to the center. Optional. Default: 0\n";
	std::cout << "-edges <value>: Percent of visible edges. Value: 0.0 - 1.0. Optional. Default: 0.0\n";
	std::cout << "-minedges <value>: Minimum number of visible edges. Value: Integer. Optional. Default: 1000\n";
	std::cout << "-W <value>: Image width in pixels. Optional. Default: 800\n";
	std::cout << "-H <value>: Image height in pixels. Optional. Default: 600\n";
	std::cout << "-window <hstart> <hend> <vstart> <vend>: Defines a window for rendering. hstart and hend are values between 0.0 and 1.0 defining a percentage of width. The same for vstart and vend respecting height. Default: 0.0 1.0 0.0 1.0 (whole picture)\n";
	std::cout << "-u <value>: Unit length. Optional. Default: 1.0\n";
	std::cout << "-net: Only for use with Network Workbench (http://nwb.slis.indiana.edu).\n";
	std::cout << "-java: Shows the visualization in a window (Requires java).\n";
	std::cout << "-render <renderer>: Selects the render engine. Possible values: 'povray' and 'svg'. Default: povray\n";
	std::cout << "-opacity <value>: Selects the edges opacity. Only for SVG graphics. Range: 0.0-1.0. Optional. Default: 0.2\n";
	std::cout << "-nocliques: Omits cliques in central core.\n";
	std::cout << "-drawCircles: Draws circles for the components' borders.\n";
	std::cout << "-alpha: Constant on the formula for the component ratio as a function of its weight.\n";
	std::cout << "-beta: Exponent on the formula for the component ratio as a function of its weight.\n";
	std::cout << "-seed: Seed for the components' distribution algorithm.\n";
	std::cout << "-ratioConstant: Constant adjusting the node size. Optional. Default: Auto-adjusted.\n";
	std::cout << "-kconn: Computes k-connectivity.\n";
	std::cout << "-kconntype: k-connectivity type. Possible values: 'strict' and 'wide'. Default: wide\n";
	std::cout << "-connectivity: Finds real connectivity between all pairs of nodes. Logs in log/gomory_hu.log to log/gomory_hu5.log. Optional.\n";
	std::cout << "-innerConnectivity: Finds inner connectivity between all pairs of nodes. Logs in log/gomory_hu.log to log/gomory_hu5.log. Optional. Will hide option -connectivity.\n";
	std::cout << "-onlygraphic: Generates the .png from an existing povray or svg file. In this case -input contains the .pov or .svg file. Default: false. Valid parameters in this case are: render, java\n";
	std::cout << "-nographic: Omits generating the .png file.\n";
	std::cout << "-colorScaleMaxValue: Sets the maximal value for the color scale. Layers with a value or equal to this value will be represented with the same color. By default, it takes the value of the highest layer.\n";
	std::cout << "-showDegreeScale: Defines if the degree scale will be shown in the picture. Boolean (0, 1). Default: 1 (true).\n";
	std::cout << "-measure: Defines the centrality measure to be used. Possible values: \"kdense\", \"mcore\". Default: \"mcore\".\n";
	std::cout << "These parameters can also be set in file lanet.cfg.\n";
	std::cout << "\n";
}

void Parameters::validateParameters(int argc, char* argv[]) {
	bool isInputFilePresent = false;
	bool isOutputFilePresent = false;
	int actualPar = 1;
	

	//Config file parameters
	std::ifstream f_input;
	f_input.open("lanet.cfg");
	
	//Read first line
	std::string line = "";

	while (getline(f_input, line)) {

		try {

			if (line.find("#") == std::string::npos) {

				int posEqual = line.find("=");
				std::string name = line.substr(0, posEqual);
				std::string rest = line.substr(posEqual+1, line.size() - posEqual - 1);
				std::string value = rest.substr(rest.find_first_not_of(" "), rest.find_last_not_of(" "));
				if (name.find("bckgnd") != std::string::npos) {
					std::string bckGndValue = value;
					if (bckGndValue == "white")
						bckGnd = "white";
					else {
						if (bckGndValue == "black")
							bckGnd = "black";
					}
				}
				else if (name.find("color") != std::string::npos) {
					std::string colorValue = value;
					if (colorValue == "col")
						color = "col";
					else {
						if (colorValue == "bw")
							color = "bw";
						else {
							if (colorValue == "bwi")
								color = "bwi";
						}
					}
				}
				else if (name.find("eps") != std::string::npos) {
					epsilon = atof(value.c_str());
				}
				else if (name.find("delta") != std::string::npos) {
					delta = atof(value.c_str());
				}
				else if (name.find("gamma") != std::string::npos) {
					gamma = atof(value.c_str());
				}
				else if (name.find("minedges") != std::string::npos) {
					minEdges = atoi(value.c_str());
				}
				else if (name.find("edges") != std::string::npos) {
					percentOfEdges = atof(value.c_str());
				}
				else if (name.find("granularity") != std::string::npos) {
					granularity = atoi(value.c_str());
				}
				else if (name.find("width") != std::string::npos) {
					widthResolution = atoi(value.c_str());
				}
				else if (name.find("height") != std::string::npos) {
					heightResolution = atoi(value.c_str());
				}
				/*if (name.find("font") != std::string::npos) {
					strmFont = atoi(value.c_str());
				}*/
				else if (name.find("u") != std::string::npos) {
					u = atof(value.c_str());
				}
				else if (name.find("render") != std::string::npos) {
					std::string rendererValue = value;
					if (rendererValue == "povray")
						renderer = "povray";
					else {
						if (rendererValue == "svg")
							renderer = "svg";
					}
				}
				else if (name.find("opacity") != std::string::npos) {
					opacity = atof(value.c_str());
				}
				else if (name.find("strengthsIntervals") != std::string::npos) {
					strengthsIntervals = value;
				}
			}
		}
		catch (...) {
		}		
		
		//Read next line
		line = "";
	}
					
	//Command line parameters
	while (actualPar < argc) {
		char* parameterName = argv[actualPar];
		if (strcmp(parameterName, "-input")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-input' parameter must be followed by input file name\n\n";
				printHelp();
				exit(1);
			} else {
				inputFile = argv[actualPar + 1];
				isInputFilePresent = true;
			}
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-decomp")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-decomp' parameter must be followed by \"kcores\" or \"kdenses\"\n\n";
				printHelp();
				exit(1);
			} else
				decomp = argv[actualPar + 1];
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-names")==0) {
			if (argc < actualPar + 2) {
				inputNodeNamesFile = "0";
				actualPar = actualPar + 1;
			} else {
				if (argv[actualPar + 1][0] != '-') {
					inputNodeNamesFile = argv[actualPar + 1];
					actualPar = actualPar + 2;
				} else {
					inputNodeNamesFile = "0";
					actualPar = actualPar + 1;
				}
			}
		}
		else if (strcmp(parameterName, "-output")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-output' parameter must be followed by output file name\n\n";
				printHelp();
				exit(1);
			} else {
				outputFile = argv[actualPar + 1];
				isOutputFilePresent = true;
			}
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-coresfile")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-coresfile' parameter must be followed by cores file name\n\n";
				printHelp();
				exit(1);
			} else {
				coresFile = argv[actualPar + 1];
			}
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-colorsFile")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-colorsFile' parameter must be followed by colors file name\n\n";
				printHelp();
				exit(1);
			} else {
				colorsFile = argv[actualPar + 1];
			}
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-coordDistributionAlgorithm")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-coordDistributionAlgorithm' parameter must be followed by 'classic', 'pow' or 'log'\n\n";
				printHelp();
				exit(1);
			} else {
				coordDistributionAlgorithm = argv[actualPar + 1];
			}
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-logfile")==0) {
			actualPar =  actualPar + 1;
			isLogFile = true;
		}
		else if (strcmp(parameterName, "-logstdout")==0) {
			actualPar = actualPar + 1;
			//log#setLogStdout;
		}
		else if (strcmp(parameterName, "-multigraph")==0) {
			actualPar = actualPar + 1;
			multigraph = true;
		}
		else if (strcmp(parameterName, "-weighted")==0) {
			actualPar = actualPar + 1;
			weighted = true;
		}
		else if (strcmp(parameterName, "-granularity")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-granularity' parameter must be followed by granularity (integer)\n\n";
				printHelp();
				exit(1);
			} else
				granularity = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-bckgnd")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-bckgnd' parameter must be followed by background color value\n\n";
				printHelp();
				exit(1);
			} else {
				std::string bckGndValue = argv[actualPar + 1];
				actualPar = actualPar + 2;
				if (bckGndValue=="white")
					bckGnd = "white";
				else {
					if (bckGndValue=="black")
						bckGnd = "black";
					else {
						std::cout << "ERROR: " << bckGndValue << " is not a possible value for bckgnd. Possible values are 'white' and 'black'.\n\n";
						printHelp();
						exit(1);
					}
				}
			}
		}
		else if (strcmp(parameterName, "-color")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-color' parameter must be followed by graph color value\n\n";
				printHelp();
				exit(1);
			} else {
				std::string colorValue = argv[actualPar + 1];
				actualPar = actualPar + 2;
				if (colorValue=="col")
					color = "col";
				else {
					if (colorValue=="bw")
						color = "bw";
					else {
						if (colorValue=="bwi")
							color = "bwi";
						else {
							std::cout << "ERROR: " << colorValue << " is not a possible value for color. Possible values are 'col', 'bw' and 'bwi'.\n\n";
							printHelp();
							exit(1);
						}
					}
				}
			}
		}
		else if (strcmp(parameterName, "-eps")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-eps' parameter must be followed by epsilon value\n\n";
				printHelp();
				exit(1);
			} else
				epsilon = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-delta")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-delta' parameter must be followed by delta value\n\n";
				printHelp();
				exit(1);
			} else
				delta = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-gamma")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-gamma' parameter must be followed by gamma value\n\n";
				printHelp();
				exit(1);
			} else
				gamma = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-fromlayer")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-fromlayer' parameter must be followed by layer number\n\n";
				printHelp();
				exit(1);
			} else
				fromLayer = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-edges")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-edges' parameter must be followed by percent of visible edges (0.0 - 1.0)\n\n";
				printHelp();
				exit(1);
			} else
				percentOfEdges = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-minedges")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-minedges' parameter must be followed by minimum number of visible edges (integer)\n\n";
				printHelp();
				exit(1);
			} else
				minEdges = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-W")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-W' parameter must be followed by width value\n\n";
				printHelp();
				exit(1);
			} else
				widthResolution = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-H")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-H' parameter must be followed by height value\n\n";
				printHelp();
				exit(1);
			} else
				heightResolution = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-font")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-font' parameter must be followed by font zoom value\n\n";
				printHelp();
				exit(1);
			} else
				fontZoom = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-window")==0) {
			if (argc < actualPar + 5) {
				std::cout << "ERROR: '-H' parameter must be followed by four values: hstart hend vstart vend\n\n";
				printHelp();
				exit(1);
			} else {
				hstart = atof(argv[actualPar + 1]);
				hend = atof(argv[actualPar + 2]);
				vstart = atof(argv[actualPar + 3]);
				vend = atof(argv[actualPar + 4]);
			}
			actualPar = actualPar + 5;
		}
		else if (strcmp(parameterName, "-u")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-u' parameter must be followed by unit length value\n\n";
				printHelp();
				exit(1);
			} else
				u = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-net")==0) {
			format = "net";
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-java")==0) {
			show = "java";
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-render")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-render' parameter must be followed by renderer name\n\n";
				printHelp();
				exit(1);
			} else {
				std::string rendererValue = argv[actualPar + 1];
				actualPar = actualPar + 2;
				if (rendererValue=="povray")
					renderer = "povray";
				else {
					if (rendererValue=="svg")
						renderer = "svg";
					else {
						std::cout << "ERROR: "<< rendererValue << " is not a possible value for render. Possible values are 'povray' and 'svg'.\n\n";
						printHelp();
						exit(1);
					}
				}
			}
		}
		else if (strcmp(parameterName, "-nocliques")==0) {
			noCliques = true;
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-drawCircles")==0) {
			circles = true;
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-alpha")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-alpha' parameter must be followed by value\n\n";
				printHelp();
				exit(1);
			} else
				alpha = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-beta")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-beta' parameter must be followed by value\n\n";
				printHelp();
				exit(1);
			} else
				beta = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-seed")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-seed' parameter must be followed by value\n\n";
				printHelp();
				exit(1);
			} else
				seed = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-ratioConstant")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-ratioConstant' parameter must be followed by value\n\n";
				printHelp();
				exit(1);
			} else {
				ratioConstant = atof(argv[actualPar + 1]);
				ratioConstantDefined = true;
			}
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-colorScaleMaxValue")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-colorScaleMaxValue' parameter must be followed by value\n\n";
				printHelp();
				exit(1);
			} else
				colorScaleMaxValue = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-showDegreeScale")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-showDegreeScale' parameter must be followed by value\n\n";
				printHelp();
				exit(1);
			} else
				showDegreeScale = atoi(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-measure")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-measure' parameter must be followed by \"kdense\" or \"mcore\"\n\n";
				printHelp();
				exit(1);
			} else
				measure = argv[actualPar + 1];
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-opacity")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-opacity' parameter must be followed by value.\n\n";
				printHelp();
				exit(1);
			} else
				opacity = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-kconn")==0) {
			kConn = true;
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-connectivity")==0) {
			connectivity = true;
			innerConnectivity = false;
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-innerConnectivity")==0) {
			innerConnectivity = true;
			connectivity = false;
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-kconntype")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-kconntype' parameter must be followed by connectivity type\n\n";
				printHelp();
				exit(1);
			} else {
				std::string connectivityTypeValue = argv[actualPar + 1];
				actualPar = actualPar + 2;
				if (connectivityTypeValue=="strict")
					kConnType = "strict";
				else {
					if (connectivityTypeValue=="wide")
						kConnType = "wide";
					else {
						std::cout << "ERROR: "<< connectivityTypeValue << " is not a possible value for kconntype. Possible values are 'strict' and 'wide'.\n\n";
						printHelp();
						exit(1);
					}
				}
			}
		}
		else if (strcmp(parameterName, "-onlygraphic")==0) {
			onlyGraphic = true;
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-nographic")==0) {
			noGraphic = true;
			actualPar = actualPar + 1;
		}
		else if (strcmp(parameterName, "-strengthsIntervals")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-strengthsIntervals' parameter must be followed by 'equalNodesPerInterval' or 'equalIntervalSize' or 'equalLogIntervalSize'.\n\n";
				printHelp();
				exit(1);
			} else
				strengthsIntervals = argv[actualPar + 1];
			actualPar = actualPar + 2;
		}
		else if (strcmp(parameterName, "-maximumStrength")==0) {
			if (argc < actualPar + 2) {
				std::cout << "ERROR: '-maximumStrength' parameter must be followed by a value.\n\n";
				printHelp();
				exit(1);
			} else
				maximumStrength = atof(argv[actualPar + 1]);
			actualPar = actualPar + 2;
		}



		else {
			std::cout << "ERROR: " << argv[actualPar] << " is not a valid parameter\n\n";
			printHelp();
			exit(1);
		}
	}

	if ((kConn==true)&&(multigraph==true)) {
		std::cout << "ERROR: k-connectivity analysis for multigraphs is not implemented yet.\n\n";
		printHelp();
		exit(1);
	}

	if ((kConn==true)&&(weighted==true)) {
		std::cout << "ERROR: k-connectivity analysis for weighted graphs is not implemented yet.\n\n";
		printHelp();
		exit(1);
	}

	if ((decomp=="kdenses")&&((kConn==true)||(connectivity==true)||(innerConnectivity==true))) {
		std::cout << "ERROR: k-connectivity analysis requires k-core decomposition.\n\n";
		printHelp();
		exit(1);
	}

	if (isInputFilePresent==false) {
		std::cout << "ERROR: '-input' parameter is required\n\n";
		printHelp();
		exit(1);
	}

	//Default output file name
	if (isOutputFilePresent==false) {
		//This code will convert, e.g. "samples/netSample/net1.dat" in "net1.pov" and "net1.png"
		outputFile = inputFile;
		outputFile = inputFile.substr(0, inputFile.rfind('.'));
		outputFile = outputFile.substr(outputFile.rfind('/') + 1, outputFile.size() - outputFile.find('/') - 1);
		outputFile = outputFile.substr(outputFile.rfind('\\') + 1, outputFile.size() - outputFile.find('\\') - 1);
	}
	
	std::string bck;
	if (bckGnd=="black")
		bck = "b";
	else
		bck="w";
	
	std::stringstream strmWidthResolution, strmHeightResolution;
	strmWidthResolution << widthResolution; 
	strmHeightResolution << heightResolution; 
	outputRendFile = outputFile + "_" + color + "_" + bck + "_" + strmWidthResolution.str() + "x" + strmHeightResolution.str();

	if (renderer=="povray") {
		if (kConn==true)
			outputRendFile = outputRendFile + "_kconn";
		if (inputNodeNamesFile!="")
			outputRendFile = outputRendFile + "_names";
		outputRendFile = outputRendFile + ".pov";
	}
	else {
		if (kConn==true)
			outputRendFile = outputRendFile + "_kconn";
		if (inputNodeNamesFile!="")
			outputRendFile = outputRendFile + "_names";
		outputRendFile = outputRendFile + ".svg";
	}

	if (onlyGraphic==false) {
		if (renderer=="povray") {
			outputPngFile = outputFile + "_" + color + "_" + bck + "_" + strmWidthResolution.str() + "x" + strmHeightResolution.str();
			if (kConn==true)
				outputPngFile = outputPngFile + "_kconn";
			if (inputNodeNamesFile!="")
				outputPngFile = outputPngFile + "_names";
			outputPngFile = outputPngFile + "POV" + ".png";
		}
		else {
			outputPngFile = outputFile + "_" + color + "_" + bck + "_" + strmWidthResolution.str() + "x" + strmHeightResolution.str();
			if (kConn==true)
				outputPngFile = outputPngFile + "_kconn";
			if (inputNodeNamesFile!="")
				outputPngFile = outputPngFile + "_names";
			outputPngFile = outputPngFile + "SVG" + ".png";
		}
	}
	else {
		if (renderer=="povray")
			outputPngFile = outputFile + "POV" + ".png";
		else
			outputPngFile = outputFile + "SVG" + ".png";
	}
	
	//if (isLogFile) {
		//log#setLogFile inputFile;
}

