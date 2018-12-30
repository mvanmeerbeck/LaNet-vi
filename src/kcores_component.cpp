#include "kcores_component.h"
#include <iostream>
#include "parameters.h"
#include "uniform.h"
#include "normal.h"
#include "circular_average.h"
#include "math.h"
#include "clique.h"
#include "network.h"
#include "graph_kcores.h"
#include "graph_visual.h"
#include "distribute_components.h"
#include "utils.h"

float degree_squares = 0;
extern std::vector<int> kcoresvalues;
extern std::vector<int> mark;

KCores_Component::KCores_Component(int m_shellIndex, KCores_Component* m_parentComponent, Graph* net, Graph_KCores* m_kcores, Graph_Visual* m_visual) {
	shellIndex = m_shellIndex;
	size = 0;
	central_core_k = 0;
	central_core_ratio = 1;
	tLogDegree = 0;
	tLog2Degree = 0;
	tLogBetaDegree = 0;
	shellCardinal = 0;
	shellTLogDegree = 0;
	shellTLog2Degree = 0;
	shellTLogBetaDegree = 0;
	parentComponent = m_parentComponent;
	n = net;

	rho = -1.0;
	phi = 0.0;

	xCoord = 0.0;
	yCoord = 0.0;
	u = parameters.getU();

	ratio = 1.0;
	endRatio = 0.0;

	kcores = m_kcores;
	visual = m_visual;
}

void KCores_Component::placeInCircularSector(float ratio, float alfa, int n, int total, float random, float& rho, float& phi, bool unique) {
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

void KCores_Component::neighboursComponentPropagation(Vertex* v, KCores_Component* component, std::list<Vertex*>* comp_cluster) {
	std::vector<Vertex*>::iterator neighb_it;
//	int i = 0;

	int nfront=0;
	int ntail=0;
	std::vector<int> prop_list;
	int longitud = n->tEdges()+1;
	prop_list.resize(longitud);
	prop_list[ntail]=v->getNumber();
	ntail = (ntail+1)%longitud;
	Vertex* v_act;
	int ngb_number;
	while (nfront!=ntail) {
		v_act = n->getVertex(prop_list[nfront]);
		std::vector<Vertex*>* ngbs = v_act->getNeighbours();
		for (neighb_it=ngbs->begin();neighb_it!=ngbs->end();neighb_it++) {
			ngb_number = (*neighb_it)->getNumber();
//			if ((i%100000==0)&&(i>0))
//				std::cout << i << std::endl;
//			i++;
			if ((mark[ngb_number]!=shellIndex+1)&&(kcoresvalues[ngb_number] > shellIndex)) {
				comp_cluster->push_back(*neighb_it);
				component->size++;
				component->tLogDegree+=log((*neighb_it)->getDegree()+1);
				component->tLog2Degree+=pow(log((*neighb_it)->getDegree()+1),2);
				component->tLogBetaDegree+=pow(log((*neighb_it)->getDegree()+1),2.00/parameters.getBeta());
//				component->all_vertices.push_back(*neighb_it);
				mark[ngb_number] = shellIndex+1;
				prop_list[ntail] = (*neighb_it)->getNumber();
				ntail = (ntail+1)%longitud;
			}
		}
		nfront = (nfront+1)%longitud;
	}
}

void KCores_Component::neighboursClusterPropagation(Vertex* v, std::list<Vertex*>* cluster) {
	std::vector<Vertex*>* neighb = v->getNeighbours();
	std::vector<Vertex*>::iterator neighb_it;
	for (neighb_it=neighb->begin();neighb_it!=neighb->end();neighb_it++) {
		if (((*neighb_it)->getCluster()==NULL)&&(kcores->getKCoresProperties(*neighb_it)==shellIndex)) {
			//Assigning cluster to vertex
			(*neighb_it)->setCluster(cluster);
			//Introducing vertex in cluster
			cluster->push_back(*neighb_it);
			neighboursClusterPropagation(*neighb_it, cluster);
		}
	}
}

void KCores_Component::computeComponents(float &central_core_parent, float maxShellIndex, float minShellIndex, float maxDegree, int level) {
//	std::cout << "Level: " << level << std::endl;

	for (int i=0;i<mark.size();i++)
		mark[i] = 0;
//	std::cout << "calculando componente shell index " << shellIndex << std::endl;
	std::list<Vertex*>* vertices = clusters.front();

	std::list<Vertex*>::iterator vertex_it;
	Vertex* v;
	std::list<Vertex*>* cluster = new std::list<Vertex*>();

	for (vertex_it=vertices->begin(); vertex_it!=vertices->end(); vertex_it++) {
		v = *vertex_it;
		//Vertexes which should be in an internal core are marked with a number equal to this core + 1		

		//If this vertex must be in an internal core
		if (mark[v->getNumber()]<shellIndex+1) {
			if (kcores->getKCoresProperties(v) != shellIndex) {
				KCores_Component* component = new KCores_Component(shellIndex+1, this, n, kcores, visual);
				components.push_back(component);

				//Cluster for this new component
				std::list<Vertex*>* comp_cluster = new std::list<Vertex*>();
				component->clusters.push_back(comp_cluster);
				//Assigning component to vertex
				//v->setKCores_Component(component);
				//Introducing vertex in component
				//v->setCluster(comp_cluster);
				comp_cluster->push_back(v);
				//Total size of this component
				component->size++;
				component->tLogDegree+=log(v->getDegree()+1);
				component->tLog2Degree+=pow(log(v->getDegree()+1),2);
				component->tLogBetaDegree+=pow(log(v->getDegree()+1),2.00/parameters.getBeta());
				component->all_vertices.push_back(v);
				mark[v->getNumber()] = shellIndex+1;

				neighboursComponentPropagation(v, component, comp_cluster);
			}					
			else {
				shellCardinal++;
				shellTLogDegree += log(v->getDegree()+1);
				shellTLog2Degree += pow(log(v->getDegree()+1),2);
				shellTLogBetaDegree += pow(log(v->getDegree()+1),2.00/parameters.getBeta());
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

	std::list<KCores_Component*>::iterator components_it;

/*	else {
		float c2 = 0.0;
		float c3;
		for (components_it=components.begin();components_it!=components.end();components_it++) {
			(*components_it)->computeComponents(c3, maxShellIndex, minShellIndex, maxDegree);
			c2 = c3;
		}
		central_core_k = c2;
		central_core_parent = c2;
	}
*/	for (components_it=components.begin();components_it!=components.end();components_it++) {
		(*components_it)->computeComponents(central_core_parent, maxShellIndex, minShellIndex, maxDegree, level+1);
	}
}

void KCores_Component::computeComponents(std::list<Vertex*>* vertices, float maxShellIndex, float minShellIndex, float maxDegree, int level) {
//	std::cout << "Level A: " << level << std::endl;

	clusters.push_front(vertices);
	all_vertices = *vertices;
	size += vertices->size();
	std::list<Vertex*>::iterator v_it;
	for (v_it=vertices->begin();v_it!=vertices->end();v_it++) {
		tLogDegree += log((*v_it)->getDegree()+1);
		tLog2Degree += pow(log((*v_it)->getDegree()+1),2);
		tLogBetaDegree += pow(log((*v_it)->getDegree()+1),2.00/parameters.getBeta());
	}
	float c;
	this->computeComponents(c, maxShellIndex, minShellIndex, maxDegree, level+1);
}

int comm_kcore_number = 0;

void KCores_Component::printComponent() {
	if (clusters.size()!=0) {
		for (int i=0;i<shellIndex;i++) std::cout << " ";			
		//std::cout << "KCores_Component of core " << core << std::endl;
	}

	std::list<KCores_Component*>::iterator components_it;
	std::list<std::list<Vertex*>*>::iterator clusters_it;
	std::list<Vertex*>::iterator cluster_it;
	for (clusters_it=clusters.begin();clusters_it!=clusters.end();clusters_it++) {
		for (int i=0;i<shellIndex;i++) std::cout << " ";			
		for (cluster_it=(*clusters_it)->begin();cluster_it!=(*clusters_it)->end();cluster_it++) {
			std::cout << (*cluster_it)->getNumber() << " ";
		}
		std::cout << std::endl;
	}
	for (components_it=components.begin();components_it!=components.end();components_it++) {
		(*components_it)->printComponent();
	}
}

void KCores_Component::logCoordinates() {
	int i;
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords << "KCores_Component of core " << shellIndex << "\n";
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords << "X: " << getX() << "\n";
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords << "Y: " << getY() << "\n";
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords << "R: " << getRatio() << "\n";
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords << "U: " << getU() << "\n";
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords << "Rho: " << getRho() << "\n";
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords << "Phi: " << getPhi() << "\n";
	for (i=1;i<shellIndex;i++)
		logCoords << " ";
	logCoords <<  "KCores_Component cardinal: " << getSize() << "\n";

	std::list<std::list<Vertex*>* >::iterator it_clusters;
	for (it_clusters=clusters.begin();it_clusters!=clusters.end();it_clusters++) {
		std::list<Vertex*>::iterator it_cluster;
		for (it_cluster=(*it_clusters)->begin();it_cluster!=(*it_clusters)->end();it_cluster++) {
			Vertex* h = *it_cluster;
			for(i=1;i<=shellIndex;i++)
				logCoords << " ";
			tVisualProperties vp = visual->getVisualProperties(h);
			logCoords << "Host " << h->getNumber() << ", x=" << vp.x << " y=" << vp.y << " rho=" << vp.rho << " phi=" << vp.phi << "\n";
		}
	}

	std::list<KCores_Component*>::iterator it_comp;
	for (it_comp=components.begin();it_comp!=components.end();it_comp++) {
		(*it_comp)->logCoordinates();
	}
}

//Computes a cluster hosts' coordinates
void KCores_Component::findClusterCoordinates(float maxShellIndex, float minShellIndex, std::list<Vertex*>& cluster, float partialClusterSum) {
	float mean = pi * cluster.size() / shellCardinal;
	float dev = pi * cluster.size() / shellCardinal;
	Normal norm(mean, dev);
	Uniform uniform(0.00, 1.00);
	float eps = parameters.getEpsilon();
	float delta = parameters.getDelta();
	float gamma = parameters.getGamma();

	std::list<Vertex*>::iterator it_cluster;
	Vertex* h;
	for (it_cluster=cluster.begin();it_cluster!=cluster.end();it_cluster++) {
		h = *it_cluster;
		int shell_h = kcores->getKCoresProperties(h);
		//Calculate rho

		std::vector<Vertex*>::iterator it_nb;
		std::vector<Vertex*>* nb = h->getNeighbours();
		std::vector<double> weights = h->getWeights();
		std::vector<double>::iterator weight_it = weights.begin();
		double w;

		double sum_w = 0;

		//For this formula, see [NIPS2005], formula (1)
		int largerShellIndexNeighbours = 0;
		int sumatory = 0;
		Uniform uni(0.0, 1.0);
		//Mixing
		std::list<Vertex*> new_nb;
		std::list<Vertex*>::iterator it_new_nb;
		std::list<double> new_weights;
		std::list<double>::iterator it_w2;
		weight_it = weights.begin();
		for (it_nb=nb->begin();it_nb!=nb->end();it_nb++) {
			if (kcores->getKCoresProperties(*it_nb) > shell_h) {
				largerShellIndexNeighbours += 1;
				if (n->isWeighted()){
					sum_w += (*weight_it);
					sumatory += (*weight_it) * (maxShellIndex - kcores->getKCoresProperties(*it_nb) + 1);
				}
				else
					sumatory += maxShellIndex - kcores->getKCoresProperties(*it_nb) + 1;
			}
			if (uni.getValue()<0.5) {
				new_nb.push_back(*it_nb);
				new_weights.push_back(*weight_it);
			}
			else {
				new_nb.push_front(*it_nb);
				new_weights.push_front(*weight_it);
			}
			weight_it++;
		}
		float average;


		if (largerShellIndexNeighbours != 0) {
				if (n->isWeighted()) {
					average = (float)sumatory / (largerShellIndexNeighbours * sum_w * (maxShellIndex - shell_h));
//					std::cout << "sum: " << sumatory << std::endl;
//					std::cout << "sum_w: " << sum_w << std::endl;
//					std::cout << "avg: " << average << std::endl;
				}
				else
					average = (float)sumatory / (largerShellIndexNeighbours * (maxShellIndex - shell_h));
		}
		else
			average = ((float)rand() / RAND_MAX);


		if ((parameters.getNoCliques()==true)&&(shell_h==maxShellIndex)) {
			visual->setVertexRho(h, ratio * uniform.getValue()); //This will be multiplied by u when formulas (6) and (7) are applied
			visual->setVertexPhi(h, uniform.getValue() * 2 * pi);
		} else {
			float rhoHost = ratio * (1.0-eps) + eps * ratio * average;
			
			visual->setVertexRho(h, rhoHost);
			//End of rho computation

			//Calculate phi
			float ang_init = 2.0 * pi * uniform.getValue();
			float phiHost = 0;
//			float phiHost = ang_init;
			float x_sum = 0;
			float y_sum = 0;
			if (parameters.getNoCliques()==false) {
				int amount = 0;
				int new_amount;
				Circular_average ca;

				it_w2 = new_weights.begin();

				for (it_new_nb=new_nb.begin();it_new_nb!=new_nb.end();it_new_nb++) {
					if (kcores->getKCoresProperties(*it_new_nb) > shell_h) {
						w = *it_w2;
						it_w2++;
						//x_sum += ((*it_nb)->getShellIndex() + 1 - h->getShellIndex()) * cos((*it_nb)->getPhi() + ang_init);
						//y_sum += ((*it_nb)->getShellIndex() + 1 - h->getShellIndex()) * sin((*it_nb)->getPhi() + ang_init);
						//std::cout << "w: " << w / sum_w << std::endl;
						if (n->isWeighted())
							new_amount = w / sum_w;
						else
							new_amount = (kcores->getKCoresProperties(*it_new_nb) + 1 - shell_h);

						if (new_amount != 0)
							phiHost = ca.average(phiHost, amount, atan2(visual->getVisualProperties(*it_new_nb).y-yCoord, visual->getVisualProperties(*it_new_nb).x-xCoord)+ang_init, new_amount);
//						phiHost = ca.average(phiHost, 2*amount/(amount + new_amount), (*it_nb)->getPhi(), 2*new_amount/(amount + new_amount));
						amount += new_amount;
					}
				}
				//phiHost = atan2(y_sum, x_sum);
				phiHost -= ang_init;

				//if (h->getShellIndex()==22)
				//	std::cout << "phi: " << phiHost << std::endl;
				if (amount==0) {
					phiHost = 2.0 * pi * uniform.getValue();
				}

			} else {
				//See [NIPS2005], formula (2)
				phiHost = 2.0 * pi * ((float)partialClusterSum / shellCardinal) + norm.getValue();
			}
		
			visual->setVertexPhi(h, phiHost);
			//End of phi computation
		}
						
		//See [NIPS2005], formula (6)
		//Calculate x
		tVisualProperties vp = visual->getVisualProperties(h);

		float xHost = xCoord + (gamma * u * vp.rho * cos(vp.phi));
		visual->setVertexX(h, xHost);
		//End of x computation
	
		//Calculate y
		float yHost = yCoord + (gamma * u * vp.rho * sin(vp.phi));
		visual->setVertexY(h, yHost);
		//End of y computation
		
	}	
}			

//Sumatory of previous components cardinals
int KCores_Component::getBrotherComponentsCardinal() {
	int sumatory = 0;
	std::list<KCores_Component*> brothers = getParentComponent()->getComponents();
	std::list<KCores_Component*>::iterator it;
	for (it=brothers.begin();it!=brothers.end();it++)
		sumatory += (*it)->getSize();
	return sumatory;
}
	
//Sumatory of previous components cardinals
int KCores_Component::getPreviousComponentsCardinal() {
	int sumatory = 0;
	std::list<KCores_Component*> brothers = getParentComponent()->getComponents();
	std::list<KCores_Component*>::iterator it;
	for (it=brothers.begin();*it!=this;it++)
		sumatory += (*it)->getSize();
	sumatory += (*it)->getSize() / 2;
	return sumatory;
}

void KCores_Component::computeCliqueHostCoordinates(std::list<Vertex*>& clique, int hostsSum, int hostsTotal, int cliqueHosts, float random) {
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

		float x = rho * (cos(phi + initialAngle)) + 0.1 * ratio * cos(initialAngle + angle / 2.0);
	
		float y = rho * (sin(phi + initialAngle)) + 0.1 * ratio * sin(initialAngle + angle / 2.0);
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
		float xHost = xCoord + (parameters.getGamma() * u * vp.rho * cos(vp.phi));
		visual->setVertexX(h, xHost);

		//Calculate y
		float yHost = yCoord + (parameters.getGamma() * u * vp.rho * sin(vp.phi));
		visual->setVertexY(h, yHost);
		i++;
	}
}

//This method calculates coordinates for all hosts in this connected component and, recursively, in its children
void KCores_Component::findCoordinatesClassic(int maxShellIndex, int minShellIndex, int maxDegree, int phiIni, int part) {
	//Calculate this component's center
	float eps = parameters.getEpsilon();
	float delta = parameters.getDelta();

	float phiIn = 3.0;
	std::list<KCores_Component*>::iterator it_component;

	if (part==1) {

		central_core_k = shellIndex;

		//Recursive callings
		for (it_component=components.begin();it_component!=components.end();it_component++) {
			(*it_component)->findCoordinatesClassic(maxShellIndex, minShellIndex, maxDegree, phiIn, part);
			if ((*it_component)->central_core_k > central_core_k) {
				central_core_k = (*it_component)->central_core_k;
				central_core_ratio = (*it_component)->central_core_ratio;
			}
		}

		if (components.size()==0) {

			std::list<std::list<Vertex*>* >::iterator it_clusters;
			std::list<Vertex*>::iterator it_cluster;
			//Finding cluster coordinates
			for (it_clusters=clusters.begin();it_clusters!=clusters.end();it_clusters++) {
			
				for (it_cluster=(*it_clusters)->begin();it_cluster!=(*it_clusters)->end();it_cluster++) {
					Vertex* hg = *it_cluster;
					degree_squares += log(1 + hg->getDegree()) * log (1 + hg->getDegree());
				}		
			}
			central_core_k = shellIndex;
			central_core_ratio = 2 * sqrt(40 * 40 * 0.01 * 0.01 * degree_squares / (log(n->getMaxStrength())*log(n->getMaxStrength())));

			ratio = central_core_ratio;
		}

		ratio = central_core_ratio + (central_core_k - shellIndex);
//		std::cout << "central_core_ratio: " << central_core_ratio << ", SI: " << shellIndex << ", ratio: " << ratio << " u: " << u << std::endl;
	}

	if (part==2) {

		if (hasParentComponent()==1) {
		
			//See [NIPS2005], formula (4)
			rho = 1.00 - ((float)size / getBrotherComponentsCardinal());
			phiIni = 0;
			phi = phiIni + 2.0 * pi * pow((float)getPreviousComponentsCardinal() / getBrotherComponentsCardinal(), 2);
		
			//See [NIPS2005], formula (3)
			xCoord = getParentComponent()->getX() + /*delta * */getParentComponent()->getRatio() * getParentComponent()->getU() * rho * cos(phi);
			yCoord = getParentComponent()->getY() + /*delta * */getParentComponent()->getRatio() * getParentComponent()->getU() * rho * sin(phi);
			//See [NIPS2005], formula (5)
			if (getParentComponent()->getComponents().size() != 1)
				u = sqrt((float)size / getBrotherComponentsCardinal()) * getParentComponent()->getU() / delta;
			else
				u = getParentComponent()->getU();
		}

		for (it_component=components.begin();it_component!=components.end();it_component++) {
			(*it_component)->findCoordinatesClassic(maxShellIndex, minShellIndex, maxDegree, phiIn, part);
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
							g1.addVertex(hg);
							cantHostsClique += 1;
						}		

						//std::cout << "clique en " << xCoord << " " << yCoord << std::endl;
						Clique clique;
						clique.buildCliques(g1, maxShellIndex);
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
						findClusterCoordinates(maxShellIndex, minShellIndex, *(*it_clusters), partialClusterSum);
	
				}
				else
					findClusterCoordinates(maxShellIndex, minShellIndex, *(*it_clusters), partialClusterSum);
			}
			partialClusterSum += (*it_clusters)->size();
		}		
	}
}

void KCores_Component::findCoordinatesModern(int maxShellIndex, int minShellIndex, int maxDegree, int phiIni) {
	//Calculate this component's center
	float eps = parameters.getEpsilon();

	float phiIn = 3.0;
	std::list<KCores_Component*>::iterator it_component;

	double R;
	if (components.size()!=0) {
		if (shellTLog2Degree!=0) {
			if (parameters.getCoordDistributionAlgorithm()=="log") {
				R = (float)(sqrt(size-shellCardinal))/sqrt(size) * ratio;
				endRatio = R;
			}
			else {
				R = min(pow((float)(sqrt(tLog2Degree-shellTLog2Degree))/sqrt(tLog2Degree),0.25) * ratio, 0.96 * ratio);
				endRatio = R;
//				R = 0.96 * ratio;
			}
		}
		else
			R = ratio;
//		double R = ((float)(sqrt(tLog2Degree-shellTLog2Degree))/sqrt(tLog2Degree)) * ratio;

		if (components.size()>1) {
			double myW[components.size()];

			double myX[components.size()];
			double myY[components.size()];
			double myR[components.size()];
			int i=0;
//			std::cout << "Hay " << components.size() << std::endl;
//			std::cout << "tLog2Degree " << tLog2Degree << std::endl;
//			std::cout << "shellTLog2Degree " << shellTLog2Degree << std::endl;

			for (it_component=components.begin();it_component!=components.end();it_component++) {
//				std::cout << "it " << (float)(*it_component)->tDegree << std::endl;

				if (parameters.getCoordDistributionAlgorithm()=="modernLog")
					myW[i] = (double)(*it_component)->size;
				else
					myW[i] = (double)(*it_component)->tLogBetaDegree;

//				std::cout << "W[" << i << "]: " << myW[i] << std::endl;
				i++;
			}
//			std::cout << "Mi centro es (" << xCoord << ", " << yCoord << ") y mi radio es " << R << std::endl;
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

	//Recursive callings
	for (it_component=components.begin();it_component!=components.end();it_component++) {
		(*it_component)->findCoordinates(maxShellIndex, minShellIndex, maxDegree, phiIn);
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

						degree_squares += log(1 + hg->getDegree()) * log (1 + hg->getDegree());
						g1.addVertex(hg);
						cantHostsClique += 1;
					}							
	
					Clique clique;
					clique.buildCliques(g1, maxShellIndex);
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
					findClusterCoordinates(maxShellIndex, minShellIndex, *(*it_clusters), partialClusterSum);
	
			}
			else
				findClusterCoordinates(maxShellIndex, minShellIndex, *(*it_clusters), partialClusterSum);
		}
		partialClusterSum += (*it_clusters)->size();
	}		

}

//This method calculates coordinates for all hosts in this connected component and, recursively, in its children
void KCores_Component::findCoordinates(int maxShellIndex, int minShellIndex, int maxDegree, int phiIni) {

	if (parameters.getCoordDistributionAlgorithm()=="classic") {
		findCoordinatesClassic(maxShellIndex, minShellIndex, maxDegree, phiIni, 1);
		findCoordinatesClassic(maxShellIndex, minShellIndex, maxDegree, phiIni, 2);
	}
	else
		findCoordinatesModern(maxShellIndex, minShellIndex, maxDegree, phiIni);

	visual->setCoordinatesComputed(true);

}

void KCores_Component::buildClustersMap(std::map<int, std::list<std::list<Vertex*>* > >& clustersMap) {
	std::list<std::list<Vertex*>* >::iterator it_clusters;
	for (it_clusters=clusters.begin();it_clusters!=clusters.end();it_clusters++) {
		clustersMap[shellIndex].push_back(*it_clusters);
	}

	std::list<KCores_Component*>::iterator it_comp;
	for (it_comp=components.begin();it_comp!=components.end();it_comp++) {
		(*it_comp)->buildClustersMap(clustersMap);
	}
}

