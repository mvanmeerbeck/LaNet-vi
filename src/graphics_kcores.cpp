#include "graphics_kcores.h"
#include "parameters.h"
#include "kcores_component.h"

Graphics_KCores::Graphics_KCores(Engine* e): uniform(0,1) {
	engine = e;
	xMeshSize = 1;
	yMeshSize = 1;
}

tColor Graphics_KCores::interpolate(tColor c1, double alfa1, tColor c2) {
	tColor c;
	c.r = c1.r * (1.0 - alfa1) + c2.r * (alfa1);
	c.g = c1.g * (1.0 - alfa1) + c2.g * (alfa1);
	c.b = c1.b * (1.0 - alfa1) + c2.b * (alfa1);
	return c;
}	

bool Graphics_KCores::par(int n) {
	if (n%2==0)
		return true;
	return false;
}

tColor Graphics_KCores::computeHostColorByShellIndex(Network* n, int shellIndex) {

	std::vector<std::pair<tColor, double> > colorList;

	if (parameters.getColor()=="col")
		colorList = rainbow;
	else
		colorList = blackwhite;

	int colorsNumber = colorList.size();

	int maxValue;
	if (parameters.getColorScaleMaxValue() == -1) {
		maxValue = n->kcores.getMaxShellIndex();
	}
	else {
		maxValue = parameters.getColorScaleMaxValue();
	}

	//maxValue=6;

	double position;
	if (parameters.getColor()=="col")
		position = (double)(min(shellIndex, maxValue) - 1) / (maxValue - 1);
	else if (parameters.getColor()=="bw")
		position = (double)(min(shellIndex, maxValue) - 1) / (maxValue - 1);
	else if ((parameters.getColor()=="bwi")&&(par(min(shellIndex, maxValue) + maxValue)))
		position = ((double)(maxValue - 1) + (double)(min(shellIndex, maxValue) - 1)) / (2.00 * (maxValue - 1));
	else 
		position = (double)(min(shellIndex, maxValue) - 1) / (2.00 * (maxValue - 1));
	
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
	
	//End of color calculation
	if (parameters.getColor()=="col") {
		double lumin = 0.7 + 0.5 * (double)(min(shellIndex, maxValue)%2);
		tColor color2;
		color2.r = lumin * hostColor.r;
		color2.g = lumin * hostColor.g;
		color2.b = lumin * hostColor.b;
		return color2;
	}
	return hostColor;
}

tColor Graphics_KCores::computeHostColor(Network* n, Vertex* h) {
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
		int kp_h = n->kcores.getKCoresProperties(h);
		return computeHostColorByShellIndex(n, kp_h);
	}

}

tColor Graphics_KCores::computeHostBorderColor(Network* n, Vertex* h) {
	tkConnectivityProperties kp_h = n->kcores_components.getkConnectivityProperties(h);
	return computeHostColorByShellIndex(n, kp_h.kConnectivity);

}
		
double Graphics_KCores::computeHostRatio(Network* n, double strength, int degree, bool weighted) {
	if ((parameters.getCoordDistributionAlgorithm()=="log")||(parameters.getCoordDistributionAlgorithm()=="pow")) {
		if (weighted==false) {
			return (0.007 * parameters.getRatioConstant()* pow((log(1 + degree)),1.5)); //FIXME: Potencia agregada al logaritmo para achicar los radios de los nodos de mayor grado
		}
		return (0.007 * parameters.getRatioConstant() * (log(1.0 + strength) / log(n->getMaxStrength())));
	}
	else {
		if (weighted==false)
			return (0.01 * 40 * pow((log(1 + degree) / log(n->getMaxDegree())),0.7));
		return (0.01 * 40 * (log(1.0 + strength) / log(n->getMaxStrength())));
	}
}

void Graphics_KCores::addCluster(std::list<Vertex*> cluster, int order, Network* n, double u, tColor textColor, int ppcV, int ppcH, double ratio, double opacity) {
	Vertex* host;
	int i, ni;
	std::list<Vertex*>::iterator cluster_it;
	for (cluster_it=cluster.begin();cluster_it!=cluster.end();cluster_it++) {
		host = *cluster_it;
		tVisualProperties vp = n->visual.getVisualProperties(host);
		int kp = n->kcores.getKCoresProperties(host);

		//Computing host ratio
		double hostRatio = computeHostRatio(n, (host->getStrength()), (host->getDegree()), (parameters.getWeighted() && (!parameters.getMultigraph())));
		
		if (order==2) {

			//Adding sphere or block
//			if ((host->getIsKConnected()==true)&&(host->getKConnectivity()!=host->getShellIndex())) {
//				std::cout << "Distinto, shell " << host->getShellIndex() << " kconn " << host->getKConnectivity() << std::endl;
//			}
			tVisualProperties vp = n->visual.getVisualProperties(host);
			logPixels <<host->getNumber() << " " << engine->getXPixel(vp.x) << " " << engine->getYPixel(vp.y) << "\n";
			
			if ((parameters.getKConn()==true) && (n->kcores_components.getIsKConnected(host)==false) && (parameters.getColor() != "col")) {
				engine->addBlock(vp.x, vp.y, 0, hostRatio, vp.color);
			}
			else {
				engine->addHost(vp.x, vp.y, 0, hostRatio, vp.color, vp.bordercolor);

			}
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
				double largeY = parameters.getU() * 1.2 * ratio;
				double largeX = parameters.getU() * 1.6 * ratio;
				double size = 0.05 * largeX * 800.0 * parameters.getFontZoom() / parameters.getWidthResolution();
				double sizeY = 0.05 * largeY * 600.0 * parameters.getFontZoom() / parameters.getHeightResolution();
				//engine#addText host#getName textColor size ((host#getX +. 0.08) /. size) (host#getY /. size);
			
				double xCoordRight = (vp.x + 1.2 * hostRatio) / size;
				double yCoordRight = vp.y / size;
				tLetra ip1 = { xCoordRight, yCoordRight, 1, 0};
										
				double xCoordLeft = (vp.x - 1.2 * hostRatio) / size;
				double yCoordLeft = vp.y / size;
				tLetra ip2 = { xCoordLeft, yCoordLeft, -1, 0};

				double xCoordUp = vp.x / size;
				double yCoordUp = (vp.y + 1.2 * hostRatio) / size;
				tLetra ip3 = { xCoordUp, yCoordUp, 1, 1};
				tLetra ip4 = { xCoordUp, yCoordUp, -1, 1};
			
				double xCoordDown = vp.x / size;
				double yCoordDown = (vp.y - 1.2 * hostRatio) / size;
				tLetra ip5 = { xCoordDown, yCoordDown, 1, -1};
				tLetra ip6 = { xCoordDown, yCoordDown, -1, -1};
			
				std::vector<tLetra> meshPosVector;
				meshPosVector.push_back(ip1);
				meshPosVector.push_back(ip2);
				meshPosVector.push_back(ip3);
				meshPosVector.push_back(ip4);
				meshPosVector.push_back(ip5);
				meshPosVector.push_back(ip6);
			
				//let nodeName = (Printf.sprintf "%d" host#getNumber) in
				int chars = nodeName.length();
				std::cout << "-----------------------" << std::endl;
				std::cout << nodeName << std::endl;
			
				int overlaps[6] =  {0, 0, 0, 0, 0, 0};
				int overlapsi;
			
				//For each position
				for (ni=0;ni<=meshPosVector.size();ni++) {
					int xMesh = round(((double)xMeshSize / 2) + (meshPosVector[ni].xi * size / (2.0 * largeX)) * xMeshSize);
					int yMesh = round(((double)yMeshSize / 2) + (meshPosVector[ni].yi * size / (2.0 * largeY)) * yMeshSize);
				
					//Printf.printf "n=%d xMesh: %d yMesh: %d\n" n xMesh yMesh;
					std::cout << "xi: " << meshPosVector[ni].xi * size  << std::endl;
					std::cout << "yi: " << meshPosVector[ni].yi * size  << std::endl;
					std::cout << "large: " << 2*largeX  << std::endl;
					std::cout << "xMesh " << xMesh << " yMesh " << yMesh << "; MeshSize: " << xMeshSize << "x" << yMeshSize << std::endl;
					double dir = meshPosVector[ni].dirxi;
					double dir2 = meshPosVector[ni].diryi;
					//For each character
					for (i=0;i<=chars;i++) {

						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh+dir2 < yMeshSize)&&(yMesh>0)) {
						
							if (namesMesh[xMesh+dir*i][yMesh+dir2] != 0) {
								overlapsi = namesMesh[xMesh+dir*i][yMesh+dir2];
								overlaps[ni] += overlapsi;
							}
						} else
							overlaps[ni] += 1;
					
						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh+dir2 + 1 < yMeshSize)&&(yMesh+dir2 + 1 >0)) {
							if (namesMesh[xMesh+dir*i][yMesh+dir2 + 1] != 0) {
								overlapsi = namesMesh[xMesh+dir*i][yMesh+dir2 + 1];
								overlaps[ni] += overlapsi;
							}
						} else
							overlaps[ni] += 1;
					
						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh+dir2 - 1 < yMeshSize)&&(yMesh+dir2 - 1 >0)) {
						
							if (namesMesh[xMesh+dir*i][yMesh+dir2 - 1] != 0) {
								overlapsi = namesMesh[xMesh+dir*i][yMesh+dir2 - 1];
								overlaps[ni] += overlapsi;
							}
						} else
							overlaps[ni] += 1;
					}
				}
			
				//Printf.printf " [%d %d %d %d %d %d] \n" !overlaps.(0) !overlaps.(1) !overlaps.(2) !overlaps.(3) !overlaps.(4) !overlaps.(5);
			
				int minPos = 0;
				for (ni=1;ni<=5;ni++) {
					std::cout << "over " << overlaps[ni] << std::endl;
					if (overlaps[ni] < overlaps[minPos])
						minPos = ni;
				}
			
				int xMesh = round(((double)xMeshSize / 2) + (meshPosVector[minPos].xi * size / (2.0 * largeX)) * xMeshSize);
				int yMesh = round(((double)yMeshSize / 2) + (meshPosVector[minPos].yi * size / (2.0 * largeY)) * yMeshSize);

				int dir =  meshPosVector[minPos].dirxi;
				int dir2 =  meshPosVector[minPos].diryi;
				//Printf.printf "pos %d\n" !minPos;
			
				//If adding this text doesn't result in a great superposition
				if ((1)/*(overlaps[minPos] <= 1)&&((double)overlaps[minPos]/chars <= 0.4)*/) {
					if (dir==1) {
						std::cout << "chars " << chars+1 << std::endl;
						engine->addText(nodeName, textColor, size, meshPosVector[minPos].xi, meshPosVector[minPos].yi + 0.5, "start");
					} else {
						std::cout << "chars " << chars+1 << std::endl;
						engine->addText(nodeName, textColor, size, meshPosVector[minPos].xi, meshPosVector[minPos].yi + 0.5, "end");
					}
					
					//Marking mesh
					for (i=0;i<=chars;i++) {
						if (((xMesh+dir*i) < xMeshSize)&&((xMesh+dir*i)>0)&&(yMesh+dir2 < yMeshSize)&&(yMesh+dir2>0))
							namesMesh[xMesh+dir*i][yMesh+dir2] += 1;
					}
				}
			}

		}

		if ((order==1)||(order==4)) {
			//Adding edge
			Vertex* ngb;
			std::vector<Vertex*>::iterator ngb_it;
			std::vector<double>::iterator weights_it;
			weights_it = host->getWeights().begin();

			double meanWeight = sqrt(n->getMinWeight()*n->getMaxWeight());

			for (ngb_it=host->getNeighbours()->begin();ngb_it!=host->getNeighbours()->end();ngb_it++) {
				ngb = *ngb_it;
				tVisualProperties vp_ngb = n->visual.getVisualProperties(ngb);
				int kp_ngb = n->kcores.getKCoresProperties(ngb);

				//TODO
				double p;
				if (n->isWeighted()) {
					if (*weights_it>=meanWeight)
						p = (log10(*weights_it/meanWeight)+1)/10;
					else
						p = 0;
//				std::cout << *weights_it << " " << p << std::endl;
				}
				else
					p = 1;
				weights_it++;

				//Probability factor
				double max2 = parameters.getPercentOfEdges();
				if ((double)parameters.getMinEdges() / n->tEdges()>max2)
					max2 = (double)parameters.getMinEdges() / n->tEdges();
				double theValue = uniform.getValue();
				if (theValue < max2)  {
					//numberOfEdges += 1;
					//To avoid selecting the same edge when host n is analyzed
					if (
(((host->getCluster()!=ngb->getCluster())&&(order==1))
||
((host->getCluster()==ngb->getCluster())&&(order==4)))
&&(host->getNumber() > ngb->getNumber())
) {
						double midPointX = (vp.x + vp_ngb.x) / 2;
						double midPointY = (vp.y + vp_ngb.y) / 2;
						double z = 2 * (n->kcores.getMaxShellIndex() - min(kp_ngb, kp));
						tColor ngbColor = vp_ngb.color;
						if (parameters.getColor()=="col") {
							ngbColor.r=0.75*ngbColor.r;
							ngbColor.g=0.75*ngbColor.g;
							ngbColor.b=0.75*ngbColor.b;
						}
						else {
							ngbColor.r=1.2*ngbColor.r;
							ngbColor.g=1.2*ngbColor.g;
							ngbColor.b=1.2*ngbColor.b;
						}

						float ratioEdge;
						if (order==1)
							ratioEdge = 0.10*(computeHostRatio(n, 0, min(host->getDegree(),ngb->getDegree()), false));
						else
							ratioEdge = 0.10*(computeHostRatio(n, 0, min(host->getDegree(),ngb->getDegree()), false));


						if (parameters.getCoordDistributionAlgorithm()=="classic")
							engine->addCylinder(vp.x, vp.y, z, midPointX, midPointY, z, ngbColor, opacity, ratioEdge);
						else
							engine->addCylinder(vp.x, vp.y, z, midPointX, midPointY, z, ngbColor, opacity, ratioEdge);
					
						tColor hostColor = vp.color;
						if (parameters.getColor()=="col") {
							hostColor.r = 0.75*hostColor.r;
							hostColor.g = 0.75*hostColor.g;
							hostColor.b = 0.75*hostColor.b;
						}
						else {
							hostColor.r = 1.2*hostColor.r;
							hostColor.g = 1.2*hostColor.g;
							hostColor.b = 1.2*hostColor.b;
						}

						if (order==1)
							ratioEdge = 0.10*(computeHostRatio(n, 0, min(host->getDegree(),ngb->getDegree()), false));
						else
							ratioEdge = 0.10*(computeHostRatio(n, 0, min(host->getDegree(),ngb->getDegree()), false));

						if (parameters.getCoordDistributionAlgorithm()=="classic")
							engine->addCylinder(midPointX, midPointY, z, vp_ngb.x, vp_ngb.y, z, hostColor, opacity, ratioEdge);
						else
							engine->addCylinder(midPointX, midPointY, z, vp_ngb.x, vp_ngb.y, z, hostColor, opacity, ratioEdge);
					}
				}
			}
		}
	}
}

void Graphics_KCores::addComponents(KCores_Component* c, int order, Network* n, double u, tColor textColor, int ppcV, int ppcH, double ratio) {

	std::list<KCores_Component*>::iterator comp_it;
	std::list<std::list<Vertex*>* >::iterator clustersIt;

	for (clustersIt=c->getClusters().begin();clustersIt!=c->getClusters().end();clustersIt++) {
		if (order == 4) {
			if (c->getComponents().size()==0)
				addCluster(*(*clustersIt), order, n, u, textColor, ppcV, ppcH, ratio, parameters.getOpacity());
			else
				addCluster(*(*clustersIt), order, n, u, textColor, ppcV, ppcH, ratio, parameters.getOpacity());
		}
		else if (order==5)
			engine->addCircle(c->getX(), c->getY(), c->getRatio() * c->getU() * parameters.getGamma(), computeHostColorByShellIndex(n, c->getShellIndex()));
		else if (order==2) {
			addCluster(*(*clustersIt), order, n, u, textColor, ppcV, ppcH, ratio, parameters.getOpacity());
		}
		else {
			addCluster(*(*clustersIt), order, n, u, textColor, ppcV, ppcH, ratio, parameters.getOpacity());
		}
	}

	for (comp_it=c->getComponents().begin();comp_it!=c->getComponents().end();comp_it++) {
		addComponents(*comp_it, order, n, u, textColor, ppcV, ppcH, ratio);
	}



}

void Graphics_KCores::generateNetworkFile(Network* n) {
	double u = parameters.getU();
	//Uniform probability distribution for edges drawing
	
	//Color computation for all hosts
	std::list<Vertex*>::iterator vertex_it;
	Vertex* h;
	for (vertex_it=n->getVertices().begin();vertex_it!=n->getVertices().end();vertex_it++) {
		h = *vertex_it;
		n->visual.setVertexColor(h, computeHostColor(n, h));
		if (parameters.getKConn()==true) {
			n->visual.setVertexBorderColor(h, computeHostBorderColor(n, h));
			if (n->kcores_components.getIsKConnected(h)==false) {
				//Log.log#print (Printf.sprintf "Host %d is not k-connected. m=%d and k-core=%d\n" h#getNumberInFile h#getM h#getShellIndex) "cores";
				if (parameters.getBckGnd()=="white")
					n->visual.setVertexColor(h, black);
				else
					n->visual.setVertexColor(h, white);
				n->visual.setVertexBorderColor(h, n->visual.getVertexColor(h));
			}
		}
		else
			n->visual.setVertexBorderColor(h, n->visual.getVertexColor(h));
	}

	//Generating mesh for nodes names*)
//	int ppcV = 12; //Pixels per character vertically
//	int ppcH = 10; //Pixels per character horizontally
	int ppcV = 7; //Pixels per character vertically
	int ppcH = 6; //Pixels per character horizontally
	
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

	double R;
	if (parameters.getCoordDistributionAlgorithm()=="classic") {
		KCores_Component* cnotempty = n->kcores_components.getMainComponent();
		while (cnotempty->getShellIndex()!=n->kcores.getMinShellIndex()) {
			cnotempty = cnotempty->getComponents().front();
		}
		R = cnotempty->getRatio();
	}
	else
		R = 1;

//	if (central_core_ratio + 1.0 > ratio)
//		ratio = central_core_ratio + 1.0;

	engine->addHeaders(n->kcores.getMaxShellIndex() + 1 - n->kcores.getMinShellIndex(), u, R);
	
	tColor textColor;
	if (parameters.getBckGnd()=="black")
		textColor = white;
	else
		textColor = black;

	if (parameters.drawCircles())
		addComponents(n->kcores_components.getMainComponent(), 5, n, u, textColor, ppcV, ppcH, R);

	addComponents(n->kcores_components.getMainComponent(), 1, n, u, textColor, ppcV, ppcH, R);
	addComponents(n->kcores_components.getMainComponent(), 4, n, u, textColor, ppcV, ppcH, R);
	addComponents(n->kcores_components.getMainComponent(), 2, n, u, textColor, ppcV, ppcH, R);


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
		addComponents(n->kcores_components.getMainComponent(), 3, n, u, textColor, ppcV, ppcH, R);

	if (parameters.getColorsFile() == "") {
		//Cores references
		for (int i=1;i<=n->kcores.getMaxShellIndex();i++) {

			double maxSI;
			if (n->kcores.getMaxShellIndex() > 15)
				maxSI = n->kcores.getMaxShellIndex();
			else
				maxSI = 15;		

			int nc = n->kcores.getMaxShellIndex() + 1 - n->kcores.getMinShellIndex();
			double x = u * ((double)R * parameters.getGamma() * 27 / 24);
			double separationRatio = 0.8 * R * u * 2 / (maxSI * 5);
			double ratio = 1.5 * separationRatio;
			double y = -u * R * parameters.getGamma() * 0.9 + 5.0 * u * parameters.getGamma() * separationRatio * i;
			tColor color = computeHostColorByShellIndex(n, i);
		
			//Adding sphere
			engine->addHost(x, y, 0, ratio, color, color);
			std::stringstream strm;
			strm << i;
			std::string shellNumber = strm.str();
			double size = 4.0 * 0.8 * R * u * 2 / (15.0 * 5);

			if ((n->kcores.getMaxShellIndex() <= 15)||((n->kcores.getMaxShellIndex() - i) % ((int)(n->kcores.getMaxShellIndex() / 15) + 1) == 0)) {
				engine->addText(shellNumber, color, size, (x + 3.0 * separationRatio)/size, y/size + 0.5, "left");
			}
		}
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
			
		double nc = n->kcores.getMaxShellIndex() + 1.0 - n->kcores.getMinShellIndex();

		double x = -u * R * (parameters.getGamma() * 15.0 / 12.0);
	
		if ((parameters.getWeighted()==false)||(parameters.getMultigraph()==true)) {
			double separationRatio = 1.5 * u * R * (0.0007 + 0.029 * (log(1 + n->getMaxDegree()) / log(n->getMaxDegree())));
			for (int i=1;i<=5;i++) {
				int degree = (int)ceil((double)n->getMaxDegree()/pow(4.0, i-1));
				if (degree > 1) {
					double ratio = computeHostRatio(n, 0.0, degree, false);
					double y = -u * R * 0.5 + 3.0 * separationRatio * i;
			
					engine->addHost(x, y, 0, ratio, sphereColor, sphereColor);
		
					std::stringstream strm;
					strm << degree;
					std::string shellNumber = strm.str();
					double size = 2.0 * separationRatio;
					engine->addText(shellNumber, textColor, size, (x + separationRatio)/size, y/size + 0.5, "left");
				}
			}
		} else {
			double separationRatio = 1.5 * (0.0007 + 0.029 * (log(1 + n->getMaxStrength()) / log(n->getMaxStrength())));
			for(int i=1;i<=5;i++) {
				double strength = n->getMaxStrength()/pow(4.0,(i-1));
			
				double ratio = computeHostRatio(n, strength, 0, true);
				double y = -u * 0.5 + 3.0 * separationRatio * i;
		
				engine->addHost(x, y, 0, ratio, sphereColor, sphereColor);
	
				std::stringstream strm;
				strm << strength;
				std::string shellNumber = strm.str();

				double size = 2.0 * separationRatio;
				engine->addText(shellNumber, textColor, size, (x + separationRatio) / size, y/size + 0.5, "left");
	
			}
		}
	}
		
	engine->close();
}

