#include "kdenses_component.h"
#include <iostream>
#include "parameters.h"
#include "uniform.h"
#include "normal.h"
#include "circular_average.h"
#include "math.h"
#include "clique.h"
#include "network.h"
#include "graph_kdenses.h"
#include "graph_visual.h"
#include "distribute_components.h"
#include "utils.h"

float degree_squares_denses = 0;
float central_core_ratio_denses = 0;

KDenses_Component::KDenses_Component(int m_denseIndex, KDenses_Component* m_parentComponent, Graph* net, Graph_KDenses* m_kdenses, Graph_Visual* m_visual) {
	denseIndex = m_denseIndex;
	size = 0;
	tLogDegree = 0;
	tLog2Degree = 0;
	tLogBetaDegree = 0;
	denseCardinal = 0;
	denseTLogDegree = 0;
	denseTLog2Degree = 0;
	denseTLogBetaDegree = 0;
	parentComponent = m_parentComponent;
	n = net;

	rho = -1.0;
	phi = 0.0;

	xCoord = 0.0;
	yCoord = 0.0;
	u = parameters.getU();

	ratio = 1.0;
	endRatio = 0.0;

	kdenses = m_kdenses;
	visual = m_visual;
}

void KDenses_Component::placeInCircularSector(float ratio, float alfa, int n, int total, float random, float& rho, float& phi, bool unique) {
	float pos = ((float)n / total) + random;
	if (pos > 1.0)
		pos = pos - 1.0;

	float divs;

	if (unique == false)
		divs = pos * (2.0 * ratio + ratio * alfa);
	else
		divs = ratio + pos * ratio * alfa;

	rho = 0.0;
	phi = 0.0;
	if (divs < ratio) {
		rho = divs;
		phi = 0.0;
	} else {
		if ((divs >= ratio)&&(divs < ratio + ratio * alfa)) {
			rho = ratio;
			phi = (divs - ratio) / ratio;
		} else {
			rho = ratio - (divs - ratio - (ratio * alfa));
			phi = alfa;
		}
	}
}

void KDenses_Component::neighboursComponentPropagation(Vertex* v, KDenses_Component* component, std::list<Vertex*>* comp_cluster) {
	std::vector<Vertex*>* neighb = v->getNeighbours();
	std::vector<Vertex*>::iterator neighb_it;
	std::list<int>::iterator edge_dense_it;
	edge_dense_it=v->getEdgeDenses()->begin();
	for (neighb_it=neighb->begin();neighb_it!=neighb->end();neighb_it++) {
		if ((*edge_dense_it > denseIndex)&&(mark[*neighb_it]!=denseIndex+1)) {
			//Assigning component to vertex
			//(*neighb_it)->setKDenses_Component(component);
			//Introducing vertex in component
			comp_cluster->push_back(*neighb_it);
			//Total size of this component
			component->size++;
			component->tLogDegree+=log((*neighb_it)->getDegree()+1);
			component->tLog2Degree+=pow(log((*neighb_it)->getDegree()+1),2.00);
			component->tLogBetaDegree+=pow(log((*neighb_it)->getDegree()+1),2.00/parameters.getBeta());
			component->all_vertices.push_back(*neighb_it);
			mark[*neighb_it] = denseIndex+1;
			neighboursComponentPropagation(*neighb_it, component, comp_cluster);
		}
		edge_dense_it++;
	}
}

void KDenses_Component::neighboursClusterPropagation(Vertex* v, std::list<Vertex*>* cluster) {
	std::vector<Vertex*>* neighb = v->getNeighbours();
	std::vector<Vertex*>::iterator neighb_it;
	std::list<int>::iterator edge_dense_it;
	edge_dense_it=v->getEdgeDenses()->begin();
	for (neighb_it=neighb->begin();neighb_it!=neighb->end();neighb_it++) {
		if (((*neighb_it)->getCluster()==NULL)&&(*edge_dense_it==denseIndex)&&(kdenses->getKDensesProperties(*neighb_it).denseIndex==denseIndex)) {
			//Assigning cluster to vertex
			(*neighb_it)->setCluster(cluster);
			//Introducing vertex in cluster
			cluster->push_back(*neighb_it);
			neighboursClusterPropagation(*neighb_it, cluster);
		}
		edge_dense_it++;
	}
}

void KDenses_Component::computeComponents(float &central_core_parent, float maxDenseIndex, float minDenseIndex, float maxDegree) {
	mark.clear();
	std::list<Vertex*>* vertices = clusters.front();

	std::list<Vertex*>::iterator vertex_it;
	Vertex* v;

	for (vertex_it=vertices->begin(); vertex_it!=vertices->end(); vertex_it++) {
		v = *vertex_it;
		mark[v] = -1;
	}

	std::list<Vertex*>* cluster = new std::list<Vertex*>();
	for (vertex_it=vertices->begin(); vertex_it!=vertices->end(); vertex_it++) {
		v = *vertex_it;
		//Vertexes which should be in an internal dense are marked with a number equal to this dense + 1		
		//If this vertex must be in an internal dense
		if (mark[v]!=denseIndex+1) {
			if (kdenses->getKDensesProperties(v).denseIndex > denseIndex) {
				KDenses_Component* component = new KDenses_Component(denseIndex+1, this, n, kdenses, visual);
				components.push_back(component);

				//Cluster for this new component
				std::list<Vertex*>* comp_cluster = new std::list<Vertex*>();
				component->clusters.push_back(comp_cluster);
				//Assigning component to vertex
				//v->setKDenses_Component(component);
				//Introducing vertex in component
				comp_cluster->push_back(v);
				//Total size of this component
				component->size++;
				component->tLogDegree+=log(v->getDegree()+1);
				component->tLog2Degree+=pow(log(v->getDegree()+1),2.00);
				component->tLogBetaDegree+=pow(log(v->getDegree()+1),2.00/parameters.getBeta());
				component->all_vertices.push_back(v);
				mark[v] = denseIndex+1;

				neighboursComponentPropagation(v, component, comp_cluster);
			}					
			else {
				denseCardinal++;
				denseTLogDegree += log(v->getDegree()+1);
				denseTLog2Degree += pow(log(v->getDegree()+1),2.00);
				denseTLogBetaDegree += pow(log(v->getDegree()+1),2.00/parameters.getBeta());
				//If this vertex belongs to a cluster in this shell
				cluster->push_back(v);
			}
		}
	}
	clusters.push_back(cluster);	
	//Deleting cluster that contained all this core
	clusters.pop_front();

	//Clusters computation
	for (vertex_it=clusters.front()->begin(); vertex_it!=clusters.front()->end(); vertex_it++) {
		v = *vertex_it;

		if (v->getCluster()==NULL) {
			std::list<Vertex*>* n_cluster = new std::list<Vertex*>();
			//Assigning cluster to vertex
			v->setCluster(n_cluster);
			//Introducing vertex in cluster
			n_cluster->push_back(v);
			neighboursClusterPropagation(v, n_cluster);
			
			clusters.push_back(n_cluster);
		}
	
	}
	//Deleting cluster that contained all the clusters from this shell
	clusters.pop_front();

	std::list<std::list<Vertex*>*> new_clusters;
	std::list<std::list<Vertex*>*>::iterator new_clusters_it;
	Uniform uniform(0.00, 1.00);
	for (new_clusters_it=clusters.begin(); new_clusters_it!=clusters.end(); new_clusters_it++) {
		if (uniform.getValue()>0.5) {
			new_clusters.push_back(*new_clusters_it);
		}
		else {
			new_clusters.push_front(*new_clusters_it);
		}
	}
	clusters = new_clusters;

	std::list<KDenses_Component*>::iterator components_it;

/*	else {
		float c2 = 0.0;
		float c3;
		for (components_it=components.begin();components_it!=components.end();components_it++) {
			(*components_it)->computeComponents(c3, maxShellIndex, minShellIndex, maxDegree);
			c2 = c3;
		}
		central_core_ratio = c2;
		central_core_parent = c2;
	}
*/	for (components_it=components.begin();components_it!=components.end();components_it++) {
		(*components_it)->computeComponents(central_core_parent, maxDenseIndex, minDenseIndex, maxDegree);
	}
}

void KDenses_Component::computeComponents(std::list<Vertex*>* vertices, float maxDenseIndex, float minDenseIndex, float maxDegree) {
	clusters.push_front(vertices);
	all_vertices = *vertices;
	size += vertices->size();
	std::list<Vertex*>::iterator v_it;
	for (v_it=vertices->begin();v_it!=vertices->end();v_it++) {
		tLogDegree += log((*v_it)->getDegree()+1);
		tLog2Degree += pow(log((*v_it)->getDegree()+1),2.00);
		tLogBetaDegree += pow(log((*v_it)->getDegree()+1),2.00/parameters.getBeta());
	}
	float c;
	this->computeComponents(c, maxDenseIndex, minDenseIndex, maxDegree);
}

int comm_kdense_number = 0;

void KDenses_Component::printCommunities() {
	if (clusters.size()!=0) {
//		for (int i=0;i<denseIndex;i++) std::cout << " ";			
//		std::cout << denseIndex << "-Dense" << std::endl;
	}

	std::list<KDenses_Component*>::iterator components_it;
	std::list<std::list<Vertex*>*>::iterator clusters_it;
	std::list<Vertex*>::iterator cluster_it;

	for (clusters_it=clusters.begin();clusters_it!=clusters.end();clusters_it++) {
//		for (int i=0;i<denseIndex;i++) std::cout << " ";			
		for (cluster_it=(*clusters_it)->begin();cluster_it!=(*clusters_it)->end();cluster_it++) {
			std::cout << (*cluster_it)->getNumber() << " " << comm_kdense_number << std::endl;
		}
		comm_kdense_number++;
//		std::cout << std::endl;
	}
	for (components_it=components.begin();components_it!=components.end();components_it++) {
		(*components_it)->printCommunities();
	}
}

void KDenses_Component::printComponent() {
	if (clusters.size()!=0) {
		for (int i=0;i<denseIndex;i++) std::cout << " ";			
		std::cout << denseIndex << "-Dense" << std::endl;
	}

	std::list<KDenses_Component*>::iterator components_it;
	std::list<std::list<Vertex*>*>::iterator clusters_it;
	std::list<Vertex*>::iterator cluster_it;
	for (clusters_it=clusters.begin();clusters_it!=clusters.end();clusters_it++) {
		for (int i=0;i<denseIndex;i++) std::cout << " ";			
		for (cluster_it=(*clusters_it)->begin();cluster_it!=(*clusters_it)->end();cluster_it++) {
			std::cout << (*cluster_it)->getNumber() << " ";
		}
		std::cout << std::endl;
	}
	for (components_it=components.begin();components_it!=components.end();components_it++) {
		(*components_it)->printComponent();
	}
}

void KDenses_Component::logCoordinates() {
	int i;
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords << "KDenses_Component of core " << denseIndex << "\n";
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords << "X: " << getX() << "\n";
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords << "Y: " << getY() << "\n";
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords << "R: " << getRatio() << "\n";
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords << "U: " << getU() << "\n";
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords << "Rho: " << getRho() << "\n";
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords << "Phi: " << getPhi() << "\n";
	for (i=1;i<denseIndex;i++)
		logCoords << " ";
	logCoords <<  "KDenses_Component size: " << getSize() << "\n";

	std::list<std::list<Vertex*>* >::iterator it_clusters;
	for (it_clusters=clusters.begin();it_clusters!=clusters.end();it_clusters++) {
		std::list<Vertex*>::iterator it_cluster;
		for (it_cluster=(*it_clusters)->begin();it_cluster!=(*it_clusters)->end();it_cluster++) {
			Vertex* h = *it_cluster;
			for(i=1;i<=denseIndex;i++)
				logCoords << " ";
			tVisualProperties vp = visual->getVisualProperties(h);
			logCoords << "Host " << h->getNumber() << ", x=" << vp.x << " y=" << vp.y << " rho=" << vp.rho << " phi=" << vp.phi << "\n";
		}
	}

	std::list<KDenses_Component*>::iterator it_comp;
	for (it_comp=components.begin();it_comp!=components.end();it_comp++) {
		(*it_comp)->logCoordinates();
	}
}

//Computes a cluster hosts' coordinates
void KDenses_Component::findClusterCoordinates(float maxDenseIndex, float minDenseIndex, std::list<Vertex*>& cluster, float partialClusterSum) {
	float mean = pi * cluster.size() / denseCardinal;
	float dev = pi * cluster.size() / denseCardinal;
	Normal norm(mean, dev);
	Uniform uniform(0.00, 1.00);
	float eps = parameters.getEpsilon();
	float delta = parameters.getDelta();
	float gamma = parameters.getGamma();

	std::list<Vertex*>::iterator it_cluster;
	Vertex* h;
	for (it_cluster=cluster.begin();it_cluster!=cluster.end();it_cluster++) {
		h = *it_cluster;
		int dense_h = kdenses->getKDensesProperties(h).denseIndex;
		//Calculate rho
	
		//For this formula, see [NIPS2005], formula (1)
		int largerDenseIndexNeighbours = 0;
		int sumatory = 0;
		Uniform uni(0.0, 1.0);
		std::vector<Vertex*>* nb = h->getNeighbours();
		//Mixing
		std::list<Vertex*> new_nb;
		std::vector<Vertex*>::iterator it_nb2;
		std::list<Vertex*>::iterator it_nb;
		for (it_nb2=nb->begin();it_nb2!=nb->end();it_nb2++) {
			if (kdenses->getKDensesProperties(*it_nb2).denseIndex >= dense_h) {
				largerDenseIndexNeighbours += 1;
				sumatory += maxDenseIndex - kdenses->getKDensesProperties(*it_nb2).denseIndex + 1;
			}
			if (uni.getValue()<0.5)
				new_nb.push_back(*it_nb2);
			else
				new_nb.push_front(*it_nb2);
		}
		float average;
		if (largerDenseIndexNeighbours != 0) {
			average = (float)sumatory / (largerDenseIndexNeighbours * (maxDenseIndex - dense_h));
		}
		else
			average = ((float)rand() / RAND_MAX);


		bool isTopCore = (endRatio==0);
		if ((parameters.getNoCliques()==true)&&(isTopCore)) {
			visual->setVertexRho(h, ratio * uniform.getValue()); //This will be multiplied by u when formulas (6) and (7) are applied
			visual->setVertexPhi(h, uniform.getValue() * 2 * pi);
		} else {
			float tau = (ratio-endRatio)/ratio;
			float rhoHost = ratio * (1.0-eps*tau) + eps * tau * ratio * average;
			visual->setVertexRho(h, rhoHost);

			//Calculate phi
			float phiHost = 0;

			float x_sum = 0;
			float y_sum = 0;
			if ((parameters.getNoCliques()==false)||(!isTopCore)) {
				int amount = 0;
				int new_amount;
				Circular_average ca;
				for (it_nb=new_nb.begin();it_nb!=new_nb.end();it_nb++) {
					if ((kdenses->getKDensesProperties(*it_nb).denseIndex > dense_h)||((kdenses->getKDensesProperties(*it_nb).denseIndex==dense_h)&&(visual->getCoordinatesComputed()==true))) {
						new_amount = kdenses->getKDensesProperties(*it_nb).denseIndex + 1 - dense_h;

						phiHost = ca.average(phiHost, amount, atan2(visual->getVisualProperties(*it_nb).y-yCoord, visual->getVisualProperties(*it_nb).x-xCoord), new_amount);

						amount += new_amount;
					}
				}
				//phiHost = atan2(y_sum, x_sum);
				//phiHost -= ang_init;

				if (amount==0) {
					phiHost = 2.0 * pi * uniform.getValue();
				}

			} else {
				//See [NIPS2005], formula (2)
				phiHost = 2.0 * pi * ((float)partialClusterSum / denseCardinal) + norm.getValue();
			}
			visual->setVertexPhi(h, phiHost);

		}
						
		//See [NIPS2005], formula (6)
		//Calculate x
		tVisualProperties vp = visual->getVisualProperties(h);

		float xHost = xCoord + (gamma * vp.rho * cos(vp.phi));
		visual->setVertexX(h, xHost);
		//End of x computation
	
		//Calculate y
		float yHost = yCoord + (gamma * vp.rho * sin(vp.phi));
		visual->setVertexY(h, yHost);
		//End of y computation

	}	
}			

//Sumatory of previous components cardinals
int KDenses_Component::getBrotherComponentsCardinal() {
	int sumatory = 0;
	std::list<KDenses_Component*> brothers = getParentComponent()->getComponents();
	std::list<KDenses_Component*>::iterator it;
	for (it=brothers.begin();it!=brothers.end();it++)
		sumatory += (*it)->getSize();
	return sumatory;
}
	
//Sumatory of previous components cardinals
int KDenses_Component::getPreviousComponentsCardinal() {
	int sumatory = 0;
	std::list<KDenses_Component*> brothers = getParentComponent()->getComponents();
	std::list<KDenses_Component*>::iterator it;
	for (it=brothers.begin();*it!=this;it++)
		sumatory += (*it)->getSize();
	sumatory += (*it)->getSize() / 2;
	return sumatory;
}

void KDenses_Component::computeCliqueHostCoordinates(std::list<Vertex*>& clique, int hostsSum, int hostsTotal, int cliqueHosts, float random) {
	float initialAngle = ((float)hostsSum / hostsTotal) * 2 * pi;
	float angle = ((float)cliqueHosts / hostsTotal) * 2 * pi;
	std::list<Vertex*>::iterator it, it_cluster;
	Vertex* ch;
	int i = 0;

	for (it=clique.begin();it!=clique.end();it++) {
		ch = *it;

		it_cluster = clusters.front()->begin();
		while ((*it_cluster)->getNumber()!=ch->getNumber())
			it_cluster++;
		Vertex* h = *it_cluster;

		float rho, phi;
		placeInCircularSector((0.90 * ratio), angle, i, cliqueHosts, random, rho, phi, (cliqueHosts==hostsTotal));
		//std::cout << "rho: " << rho << std::endl;
		/*Printf.printf "Node %d\n" i;
		Printf.printf "Angle %f\n" !angle;
		Printf.printf "v.r: %f\n" v.r;
		Printf.printf "v.alfa: %f\n" v.alfa;*/

		float x = rho * (cos(phi + initialAngle)) + 0 * ratio * cos(initialAngle + angle / 2.0);
	
		float y = rho * (sin(phi + initialAngle)) + 0 * ratio * sin(initialAngle + angle / 2.0);
		visual->setVertexRho(h, sqrt(x * x + y * y));
//		std::cout << h->getRho() << std::endl;		
		/*Printf.printf "RHO: %f\n" h#getRho;
		Printf.printf "\n";*/
	
		float a = atan(y/x);
		if (x > 0.0)
			visual->setVertexPhi(h, a);
		else 
			visual->setVertexPhi(h, a + 3.141592);

		tVisualProperties vp = visual->getVisualProperties(h);
		//Calculate x
		float xHost = xCoord + (parameters.getGamma() * vp.rho * cos(vp.phi));
		visual->setVertexX(h, xHost);

		//Calculate y
		float yHost = yCoord + (parameters.getGamma() * vp.rho * sin(vp.phi));
		visual->setVertexY(h, yHost);

		i++;
	}
}

//This method calculates coordinates for all hosts in this connected component and, recursively, in its children
void KDenses_Component::findCoordinates(int maxDenseIndex, int minDenseIndex, int maxDegree, int phiIni) {
	//Calculate this component's center
	float eps = parameters.getEpsilon();
	float delta = parameters.getDelta();

	float phiIn = 3.0;
	std::list<KDenses_Component*>::iterator it_component;

	double R;
	if (components.size()!=0) {
		if (denseTLog2Degree!=0) {
			if (parameters.getCoordDistributionAlgorithm()=="log") {
				if (denseIndex>1) {
//					R = (float)(sqrt(size-denseCardinal))/sqrt(size) * ratio;
					R = 0.92 * ratio;
	//				R = sqrt((float)(sqrt(tLog2Degree-denseTLog2Degree))/sqrt(tLog2Degree)) * ratio;
					endRatio = R;
				}
				else {
					R = 0.97;
					endRatio = R;
				}
			}
			else {
				if (denseIndex>1) {
//					R = min(sqrt((float)(sqrt(tLog2Degree-denseTLog2Degree))/sqrt(tLog2Degree)) * ratio, 0.96 * ratio);
//					R = (float)(sqrt(size-denseCardinal))/sqrt(size) * ratio;
					R = 0.92 * ratio;
					endRatio = R;
	//				R = 0.96 * ratio;
				}
				else {
					R = 0.97;
					endRatio = R;
				}
			}
		}
		else
			R = ratio;
//		double R = ((float)(sqrt(tLog2Degree-denseTLog2Degree))/sqrt(tLog2Degree)) * ratio;

		if (components.size()>1) {
			double myW[components.size()];

			double myX[components.size()];
			double myY[components.size()];
			double myR[components.size()];
			int i=0;
//			std::cout << "Hay " << components.size() << std::endl;
//			std::cout << "tLog2Degree " << tLog2Degree << std::endl;
//			std::cout << "denseTLog2Degree " << denseTLog2Degree << std::endl;

			for (it_component=components.begin();it_component!=components.end();it_component++) {
//				std::cout << "it " << (float)(*it_component)->tDegree << std::endl;

				if (parameters.getCoordDistributionAlgorithm()=="log")
					myW[i] = (double)(*it_component)->size;
				else
					myW[i] = (double)(*it_component)->tLogBetaDegree;

//				std::cout << "W[" << i << "]: " << myW[i] << std::endl;
				i++;
			}
//			std::cout << "Mi centro es (" << xCoord << ", " << yCoord << ") y mi radio es " << R << ", size " << components.size() << std::endl;
			int res = distribute_components(xCoord, yCoord, R, components.size(), myW, myX, myY, myR);

			i=0;
			for (it_component=components.begin();it_component!=components.end();it_component++) {
				(*it_component)->setX(myX[i]);
				(*it_component)->setY(myY[i]);
				(*it_component)->setRatio(myR[i]);
				i++;
			}
//			std::cout << "----- FIN --------" << std::endl;
		}
		else {
			(*components.begin())->setX(xCoord);
			(*components.begin())->setY(yCoord);
			(*components.begin())->setRatio(R);

		}
	}
	else {
		if ((parameters.getRatioConstantDefined()==false)&&((0.5*ratio/(sqrt(tLog2Degree)) < parameters.getRatioConstant())||(parameters.getRatioConstant()==-1))) {
			parameters.setRatioConstant(0.5*ratio/(sqrt(tLog2Degree)));
		}
}

	//Recursive callings
	for (it_component=components.begin();it_component!=components.end();it_component++) {
		(*it_component)->findCoordinates(maxDenseIndex, minDenseIndex, maxDegree, phiIn);
	}
	
	int partialClusterSum = 0;
	std::list<std::list<Vertex*>* >::iterator it_clusters;
	//Finding cluster coordinates
	for (it_clusters=clusters.begin();it_clusters!=clusters.end();it_clusters++) {
		
		if ((*it_clusters)->size() != 0) {
			//If this component is a top core
			if (components.size()==0) {
				if (parameters.getNoCliques()==false) {
					//In this case, cliques are built
					Network g1;
					int cantHostsClique = 0;
					
					std::list<Vertex*>::iterator it_cluster;
					for (it_cluster=(*it_clusters)->begin();it_cluster!=(*it_clusters)->end();it_cluster++) {
						Vertex* hg = *it_cluster;
						degree_squares_denses += log(1 + hg->getDegree()) * log (1 + hg->getDegree());
						g1.addVertex(hg);
						cantHostsClique += 1;
					}							
	
					Clique clique;
					clique.buildCliques(g1, maxDenseIndex);
					//log#print (Printf.sprintf "Central core size: %d\n" !cantHostsClique) "cores";
					std::list<std::list<Vertex*> > listCliques = clique.getCliques();
					std::list<std::list<Vertex*> >::iterator list_cliques_it;

					int partialSum = 0;
					for (list_cliques_it=listCliques.begin();list_cliques_it!=listCliques.end();list_cliques_it++) {

						Uniform uniform(0.0,1.0);
						float random = uniform.getValue();
						computeCliqueHostCoordinates(*list_cliques_it, partialSum, cantHostsClique, list_cliques_it->size(), random);

						partialSum += list_cliques_it->size();
					}
				} else //No cliques
					findClusterCoordinates(maxDenseIndex, minDenseIndex, *(*it_clusters), partialClusterSum);
	
			}
			else
				findClusterCoordinates(maxDenseIndex, minDenseIndex, *(*it_clusters), partialClusterSum);
		}
		partialClusterSum += (*it_clusters)->size();
	}		

	visual->setCoordinatesComputed(true);
}

void KDenses_Component::buildClustersMap(std::map<int, std::list<std::list<Vertex*>* > >& clustersMap) {
	std::list<std::list<Vertex*>* >::iterator it_clusters;
	for (it_clusters=clusters.begin();it_clusters!=clusters.end();it_clusters++) {
		clustersMap[denseIndex].push_back(*it_clusters);
	}

	std::list<KDenses_Component*>::iterator it_comp;
	for (it_comp=components.begin();it_comp!=components.end();it_comp++) {
		(*it_comp)->buildClustersMap(clustersMap);
	}
}

