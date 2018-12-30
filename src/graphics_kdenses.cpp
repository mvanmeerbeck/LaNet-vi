#include "graphics_kdenses.h"
#include "parameters.h"
#include "kdenses_component.h"

Graphics_KDenses::Graphics_KDenses(Engine* e): uniform(0,1) {
	engine = e;
	xMeshSize = 1;
	yMeshSize = 1;
}

tColor Graphics_KDenses::interpolate(tColor c1, double alfa1, tColor c2) {
	tColor c;
	c.r = c1.r * (1.0 - alfa1) + c2.r * (alfa1);
	c.g = c1.g * (1.0 - alfa1) + c2.g * (alfa1);
	c.b = c1.b * (1.0 - alfa1) + c2.b * (alfa1);
	return c;
}	

bool Graphics_KDenses::par(int n) {
	if (n%2==0)
		return true;
	return false;
}

tColor Graphics_KDenses::computeHostColorByDenseIndex(Network* n, int denseIndex) {
	std::vector<std::pair<tColor, double> > colorList;

	if (parameters.getColor()=="col")
		colorList = rainbow;
	else
		colorList = blackwhite;

	int colorsNumber = colorList.size();

	int maxValue;
	if (parameters.getColorScaleMaxValue() == -1) {
		maxValue = n->kdenses.getMaxDenseIndex();
	}
	else {
		if (parameters.getMeasure()=="mcore")
			maxValue = parameters.getColorScaleMaxValue()+2;
		else
			maxValue = parameters.getColorScaleMaxValue();
	}

	double position;
	if (parameters.getColor()=="col")
		position = (double)(min(denseIndex, maxValue) - 1) / (maxValue - 1);
	else if (parameters.getColor()=="bw")
		position = (double)(min(denseIndex, maxValue) - 1) / (maxValue - 1);
	else if ((parameters.getColor()=="bwi")&&(par(min(denseIndex, maxValue) + maxValue)))
		position = ((double)(maxValue - 1) + (double)(min(denseIndex, maxValue) - 1)) / (2.00 * (maxValue - 1));
	else 
		position = (double)(min(denseIndex, maxValue) - 1) / (2.00 * (maxValue - 1));
	
	//Color calculation
	tColor hostColor;
	if (maxValue==1)
		hostColor = red;
	else {
		std::pair<tColor, double> p1 = colorList[0];
		std::pair<tColor, double> p2 = colorList[colorsNumber-1];
		if (position < p1.second)
			hostColor = p1.first;
		else if (position > p2.second)
			hostColor = p2.first;
		else {
			int index = 0;
			bool enc = false;
			
			while (enc==false) {
				p1 = colorList[index];
				p2 = colorList[index + 1];
				if ((p1.second<=position)&&(position<=p2.second))
					enc = true;
				else
					index += 1;
			}

			p1 = colorList[index];
			p2 = colorList[index + 1];
			double alfa1 = (double)(position - p1.second) / (p2.second - p1.second);
			hostColor = interpolate(p1.first, alfa1, p2.first);
		}
	}
	
	if (parameters.getBckGnd()=="black") {
		if (parameters.getColor()=="col") {
			double lumin = 0.7 + 0.5 * (double)(min(denseIndex, maxValue)%2);
			tColor color2;
			color2.r = lumin * hostColor.r;
			color2.g = lumin * hostColor.g;
			color2.b = lumin * hostColor.b;
			return color2;
		}
	}
	else
		if (parameters.getColor()=="col") {
			double lumin = 0.9;
			tColor color2;
			color2.r = lumin * hostColor.r;
			color2.g = lumin * hostColor.g;
			color2.b = lumin * hostColor.b;
			return color2;
		}
}

tColor Graphics_KDenses::computeHostColor(Network* n, Vertex* h) {
	tColor color;
	if (parameters.getColorsFile() != "") {
		try {
			std::vector<double> vc = n->colors.getColor(h);
			color.r = vc[0];
			color.g = vc[1];
			color.b = vc[2];
			return color;
		}
		catch (ExceptionVertexNotColored e) { 
			if (parameters.getBckGnd() == "black") {
				color.r = 1.0;
				color.g = 1.0;
				color.b = 1.0;
			}
			else {
				color.r = 0.0;
				color.g = 0.0;
				color.b = 0.0;
			}
			return color;
		}
	}
	else {
		tKDensesProperties kp_h = n->kdenses.getKDensesProperties(h);
		return computeHostColorByDenseIndex(n, kp_h.denseIndex);
	}
}

double Graphics_KDenses::computeHostRatio(Network* n, double strength, int degree, bool weighted) {
	if (weighted==false) {
		return (parameters.getRatioConstant() * pow((log(1 + degree)),0.5)); //FIXME: Potencia agregada al logaritmo para achicar los radios de los nodos de mayor grado
//		return parameters.getRatioConstant() * parameters.getMinHostRatio();
	}
	return (parameters.getRatioConstant() * (log(1.0 + strength) / log(n->getMaxStrength())));
}

void Graphics_KDenses::addCluster(std::list<Vertex*> cluster, int order, Network* n, double u, tColor textColor, int ppcV, int ppcH, double ratio, double opacity) {
	Vertex* host;
	int i, ni;
	std::list<Vertex*>::iterator cluster_it;
	for (cluster_it=cluster.begin();cluster_it!=cluster.end();cluster_it++) {
		host = *cluster_it;
		tVisualProperties vp = n->visual.getVisualProperties(host);
		tKDensesProperties kp = n->kdenses.getKDensesProperties(host);

		//Computing host ratio
		double hostRatio = computeHostRatio(n, (host->getStrength()), (host->getDegree()), (parameters.getWeighted() && (!parameters.getMultigraph())));
		
		if (order==2) {

			//Adding sphere or block
//			if ((host->getIsKConnected()==true)&&(host->getKConnectivity()!=host->getDenseIndex())) {
//				std::cout << "Distinto, dense " << host->getDenseIndex() << " kconn " << host->getKConnectivity() << std::endl;
//			}
			tVisualProperties vp = n->visual.getVisualProperties(host);
			logPixels <<host->getNumber() << " " << engine->getXPixel(vp.x) << " " << engine->getYPixel(vp.y) << "\n";

//			engine->addHost(vp.x, vp.y, 0, hostRatio, vp.color, blackwhite[2].first); //BLACK BORDER FOR THE NODE (NICE FOR WHITE BCKGND PICTURES)
			engine->addHost(vp.x, vp.y, 0, hostRatio, vp.color, vp.color);
		}

		if (order==3) {
			//Adding node's name
		
			bool hasName = true;
			std::string nodeName;
			std::stringstream strm;

			if (n->names.getName(host) != "") {
				nodeName = n->names.getName(host);
			}
			else {
				if (parameters.getInputNodeNamesFile()=="0") {
					strm << host->getNumber();
					nodeName = strm.str();
				}
				else {
					hasName = false;
					nodeName = "";
				}
			}

			if (hasName==true) {
				//if host#getName <> "" then
				//Printf.printf "Nodo %s\n" host#getName;
				double large = parameters.getU() * ratio;
				double size = 0.05 * large * 800.0 * parameters.getFontZoom() / parameters.getWidthResolution();
				//engine#addText host#getName textColor size ((host#getX +. 0.08) /. size) (host#getY /. size);
			
				double xCoordRight = (vp.x + 1.2 * hostRatio) / size;
				double yCoordRight = vp.y / size;
				tLetra ip1 = { xCoordRight, yCoordRight, 1, 0};
										
				double xCoordLeft = (vp.x - 1.2 * hostRatio) / size;
				double yCoordLeft = vp.y / size;
				tLetra ip2 = { xCoordLeft, yCoordLeft, -1, 0};

				double xCoordUp = vp.x / size;
				double yCoordUp = (vp.y + 1.2 * hostRatio) / size;
				tLetra ip3 = { xCoordUp, yCoordUp, 1, 0};
				tLetra ip4 = { xCoordUp, yCoordUp, -1, 0};
			
				double xCoordDown = vp.x / size;
				double yCoordDown = (vp.y - 1.2 * hostRatio) / size;
				tLetra ip5 = { xCoordDown, yCoordDown, 1, 0};
				tLetra ip6 = { xCoordDown, yCoordDown, -1, 0};
			
				std::vector<tLetra> meshPosVector;
				meshPosVector.push_back(ip1);
				meshPosVector.push_back(ip2);
				meshPosVector.push_back(ip3);
				meshPosVector.push_back(ip4);
				meshPosVector.push_back(ip5);
				meshPosVector.push_back(ip6);
			
				//let nodeName = (Printf.sprintf "%d" host#getNumber) in
				int chars = nodeName.length();
			
				int overlaps[6] =  {0, 0, 0, 0, 0, 0};
				int overlapsi;
			
				//For each position
				for (ni=0;ni<=meshPosVector.size();ni++) {
					int xMesh = round(((double)xMeshSize / 2) + (meshPosVector[ni].xi * size / (2.0 * large)) * xMeshSize);
					int yMesh = round(((double)yMeshSize / 2) + (meshPosVector[ni].yi * size / (2.0 * large)) * yMeshSize);
				
					//Printf.printf "n=%d xMesh: %d yMesh: %d\n" n xMesh yMesh;

					double dir = meshPosVector[ni].dirxi;
					//For each character
					for (i=0;i<=chars;i++) {
					
						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh < yMeshSize)&&(yMesh>0)) {
						
							if (namesMesh[xMesh+dir*i][yMesh] != 0) {
								overlapsi = namesMesh[xMesh+dir*i][yMesh];
								overlaps[ni] += overlapsi;
							}
						} else
							overlaps[ni] += 1;
					
						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh + 1 < yMeshSize)&&(yMesh + 1 >0)) {
							if (namesMesh[xMesh+dir*i][yMesh + 1] != 0) {
								overlapsi = namesMesh[xMesh+dir*i][yMesh + 1];
								overlaps[ni] += overlapsi;
							}
						} else
							overlaps[ni] += 1;
					
						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh - 1 < yMeshSize)&&(yMesh - 1 >0)) {
						
							if (namesMesh[xMesh+dir*i][yMesh - 1] != 0) {
								overlapsi = namesMesh[xMesh+dir*i][yMesh - 1];
								overlaps[ni] += overlapsi;
							}
						} else
							overlaps[ni] += 1;
					}
				}
			
				//Printf.printf " [%d %d %d %d %d %d] \n" !overlaps.(0) !overlaps.(1) !overlaps.(2) !overlaps.(3) !overlaps.(4) !overlaps.(5);
			
				int minPos = 0;
				for (ni=1;ni<=5;ni++) {
					if (overlaps[ni] < overlaps[minPos])
						minPos = ni;
				}
			
				int xMesh = round(((double)xMeshSize / 2) + (meshPosVector[minPos].xi * size / (2.0 * large)) * xMeshSize);
				int yMesh = round(((double)yMeshSize / 2) + (meshPosVector[minPos].yi * size / (2.0 * large)) * yMeshSize);

				int dir =  meshPosVector[minPos].dirxi;
				//Printf.printf "pos %d\n" !minPos;
			
				//If adding this text doesn't result in a great superposition
				if ((overlaps[minPos] <= 1)&&((double)overlaps[minPos]/chars <= 0.4)) {
					if (dir==1) {
						engine->addText(nodeName, textColor, size, meshPosVector[minPos].xi, meshPosVector[minPos].yi + 0.5, "left");
					} else {
						engine->addText(nodeName, textColor, size, (meshPosVector[minPos].xi - (chars + 1) * ((double)ppcH / parameters.getWidthResolution()) * 2.0 * large / size), meshPosVector[minPos].yi + 0.5, "right");
					}
					
					//Marking mesh
					for (i=0;i<=chars;i++) {
						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh < yMeshSize)&&(yMesh>0))
							namesMesh[xMesh+dir*i][yMesh] += 1;
					}
				}
			}

		}

		if ((order==1)||(order==4)) {
			//Adding edge
			Vertex* ngb;
			std::vector<Vertex*>::iterator ngb_it;
			for (ngb_it=host->getNeighbours()->begin();ngb_it!=host->getNeighbours()->end();ngb_it++) {
				ngb = *ngb_it;
				tVisualProperties vp_ngb = n->visual.getVisualProperties(ngb);
				tKDensesProperties kp_ngb = n->kdenses.getKDensesProperties(ngb);

				double p = 1;
				//Probability factor
				double max2 = parameters.getPercentOfEdges();
				if ((double)parameters.getMinEdges() / n->tEdges()>max2)
					max2 = (double)parameters.getMinEdges() / n->tEdges();
				double theValue = uniform.getValue();
				if (theValue < max2)  {
					//numberOfEdges += 1;
					//To avoid selecting the same edge when host n is analyzed
					if (((host->getCluster()!=ngb->getCluster())&&(order==1))||((host->getCluster()==ngb->getCluster())&&(order==4))&&(host->getNumber() > ngb->getNumber())) {

						int edge_id;
						if (n->kdenses.map_vertices_pair_to_edge.find(std::make_pair(ngb->getNumber(), host->getNumber()))!=n->kdenses.map_vertices_pair_to_edge.end()) 
							edge_id = n->kdenses.map_vertices_pair_to_edge[std::make_pair(ngb->getNumber(), host->getNumber())];
						else
							edge_id = -1;
						//std::cout << host->getNumber() << " " << ngb->getNumber() << " edge_id: " << edge_id << std::endl;
						int edge_k_dense;
						if (edge_id!=-1)
							edge_k_dense = n->kdenses.dual_graph.kcores.getShellIndex(n->kdenses.dual_graph.getVertex(edge_id))/2 + 2;
						else
							edge_k_dense = 2;
						//std::cout << " --> " << edge_k_dense << std::endl;


						double z = 2 * (n->kdenses.getMaxDenseIndex() - edge_k_dense);
						tColor ngbColor = computeHostColorByDenseIndex(n, edge_k_dense);

						if (parameters.getColor()=="col") {
							ngbColor.r=0.5*ngbColor.r;
							ngbColor.g=0.5*ngbColor.g;
							ngbColor.b=0.5*ngbColor.b;
						}
						else {
							ngbColor.r=1.2*ngbColor.r;
							ngbColor.g=1.2*ngbColor.g;
							ngbColor.b=1.2*ngbColor.b;
						}

						if (order==4)
							engine->addCylinder(vp.x, vp.y, z, vp_ngb.x, vp_ngb.y, z, ngbColor, 1.0, 0.2*(computeHostRatio(n, 0, 1, false)));
						else if (order==1) {
							ngbColor.r=0.90;
							ngbColor.g=0.90;
							ngbColor.b=0.90;
							engine->addCylinder(vp.x, vp.y, z, vp_ngb.x, vp_ngb.y, z, ngbColor, 1.0, 0.2*(computeHostRatio(n, 0, 1, false)));
						}
					}
				}
			}
		}
	}
}

void Graphics_KDenses::addComponents(KDenses_Component* c, int order, Network* n, double u, tColor textColor, int ppcV, int ppcH, double ratio) {

	double rhoDense;

	std::list<KDenses_Component*>::iterator comp_it;
	std::list<std::list<Vertex*>* >::iterator clustersIt;

	for (clustersIt=c->getClusters().begin();clustersIt!=c->getClusters().end();clustersIt++) {
		if (order==4)
			addCluster(*(*clustersIt), order, n, u, textColor, ppcV, ppcH, ratio, 0.9);
		else if (order==5)
			engine->addCircle(c->getX(), c->getY(), c->getRatio() * parameters.getGamma(), computeHostColorByDenseIndex(n, c->getDenseIndex()));
		else
			addCluster(*(*clustersIt), order, n, u, textColor, ppcV, ppcH, ratio, parameters.getOpacity());
	}

	for (comp_it=c->getComponents().begin();comp_it!=c->getComponents().end();comp_it++) {
		addComponents(*comp_it, order, n, u, textColor, ppcV, ppcH, ratio);
	}



}

void Graphics_KDenses::generateNetworkFile(Network* n) {
	double u = parameters.getU();
	//Uniform probability distribution for edges drawing
	
	//Color computation for all hosts
	std::list<Vertex*>::iterator vertex_it;
	Vertex* h;
	for (vertex_it=n->getVertices().begin();vertex_it!=n->getVertices().end();vertex_it++) {
		h = *vertex_it;
		n->visual.setVertexColor(h, computeHostColor(n, h));
	}

	//Generating mesh for nodes names*)
	int ppcV = 12; //Pixels per character vertically
	int ppcH = 10; //Pixels per character horizontally
	
	xMeshSize = round((double)parameters.getWidthResolution()/(ppcH*parameters.getFontZoom()));
	yMeshSize = round((double)parameters.getHeightResolution()/(ppcV*parameters.getFontZoom()));
	
	namesMesh.resize(xMeshSize);
	for (int i=0;i<xMeshSize;i++) {
		namesMesh[i].resize(yMeshSize);
		for (int j=0;j<yMeshSize;j++) {
			namesMesh[i][j] = 0;
		}
	}
	
	//int numberOfEdges = 0;
	double R = 1;
//	if (central_core_ratio + 1.0 > ratio)
//		ratio = central_core_ratio + 1.0;
	engine->addHeaders(n->kdenses.getMaxDenseIndex() + 1 - n->kdenses.getMinDenseIndex(), u, R);
	
	tColor textColor;
	if (parameters.getBckGnd()=="black")
		textColor = white;
	else
		textColor = black;

	if (parameters.drawCircles()) {
		addComponents(n->kdenses_components.getMainComponent(), 5, n, u, textColor, ppcV, ppcH, R);
	}
	addComponents(n->kdenses_components.getMainComponent(), 1, n, u, textColor, ppcV, ppcH, R);
	addComponents(n->kdenses_components.getMainComponent(), 4, n, u, textColor, ppcV, ppcH, R);
	addComponents(n->kdenses_components.getMainComponent(), 2, n, u, textColor, ppcV, ppcH, R);

/*
	//GRID
	for (int i=1;i<=24;i++) {
		engine->addCylinder((double)-1.2 + (double)i/10, -1.2, 0, (double)-1.2 + (double)i/10, 1.2, 0, white, 1.0,  0.002);
	}

	for (int i=1;i<=24;i++) {
		engine->addCylinder( -1.2,(double)-1.2 + (double)i/10, 0, 1.2, (double)-1.2 + (double)i/10, 0, white, 1.0,  0.002);
	}
*/

	if (parameters.getInputNodeNamesFile() != "")
		addComponents(n->kdenses_components.getMainComponent(), 3, n, u, textColor, ppcV, ppcH, R);

	//Denses references
	for (int i=2;i<=n->kdenses.getMaxDenseIndex();i++) {

		double maxSI;
		if (n->kdenses.getMaxDenseIndex() > 15)
			maxSI = n->kdenses.getMaxDenseIndex();
		else
			maxSI = 15;		

		int nc = n->kdenses.getMaxDenseIndex() + 1 - n->kdenses.getMinDenseIndex();
		double x = u * ((double)R * parameters.getGamma() * 27 / 24);
		double separationRatio = 0.8 * u * 2 / (maxSI * 5);
		double ratio = 1.5 * separationRatio;
		double y = -u  * parameters.getGamma() * 0.9 + 5.0 * parameters.getGamma() * separationRatio * i;
		tColor color = computeHostColorByDenseIndex(n, i);
	
		//Adding sphere
		engine->addHost(x, y, 0, ratio, color, blackwhite[2].first);
		std::stringstream strm;
		strm << i - 2 *(parameters.getMeasure()=="mcore");
		std::string kDenseOrMCoreNumber = strm.str();
		double size = 4.0 * 0.8 * u * 2 / (15.0 * 5);

		if ((n->kdenses.getMaxDenseIndex() <= 15)||((n->kdenses.getMaxDenseIndex() - i) % ((int)(n->kdenses.getMaxDenseIndex() / 15) + 1) == 0))
			engine->addText(kDenseOrMCoreNumber, color, size, ((x + 3.0 * separationRatio) / size), (y / size) + 0.5, "left");
					
	}
	
	if (parameters.getShowDegreeScale()==true) {
		//Degree references, 5 at most
		if (parameters.getBckGnd()=="black")
			textColor=white;
		else
			textColor=black;

		tColor sphereColor;
		if (parameters.getBckGnd()=="black")
			sphereColor=white;
		else
			sphereColor=gray;
			
		double nc = n->kdenses.getMaxDenseIndex() + 1.0 - n->kdenses.getMinDenseIndex();

		double x = -u * (R * parameters.getGamma() * 15.0 / 12.0);
	
		if ((parameters.getWeighted()==false)||(parameters.getMultigraph()==true)) {
			double separationRatio = 1.5 * (0.0007 + 0.029 * (log(1 + n->getMaxDegree()) / log(n->getMaxDegree())));
			for (int i=1;i<=5;i++) {
				int degree = (int)ceil((double)n->getMaxDegree()/pow(4.0, i-1));
				if (degree > 1) {
					double ratio = computeHostRatio(n, 0.0, degree, false);
					double y = -u * 0.5 + 3.0 * separationRatio * i;
			
					engine->addHost(x, y, 0, ratio, sphereColor, blackwhite[2].first);
		
					std::stringstream strm;
					strm << degree;
					std::string denseNumber = strm.str();
					double size = 2.0 * separationRatio;
					engine->addText(denseNumber, textColor, size, ((x + separationRatio) / size), y / size + 0.5, "left");
				}
			}
		} else {
			double separationRatio = 1.5 * (0.0007 + 0.029 * (log(1 + n->getMaxStrength()) / log(n->getMaxStrength())));
			for(int i=1;i<=5;i++) {
				double strength = n->getMaxStrength()/pow(4.0,(i-1));
			
				double ratio = computeHostRatio(n, strength, 0, true);
				double y = -u * 0.5 + 3.0 * separationRatio * i;
		
				engine->addHost(x, y, 0, ratio, sphereColor, blackwhite[2].first);
	
				std::stringstream strm;
				strm << strength;
				std::string denseNumber = strm.str();

				double size = 2.0 * separationRatio;
				engine->addText(denseNumber, textColor, size, ((x + separationRatio) / size), (y / size) + 0.5, "left");
	
			}
		}
	}
		
	engine->close();
}

