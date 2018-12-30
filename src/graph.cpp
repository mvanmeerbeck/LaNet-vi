#include "graph.h"
#include "math.h"
#include <iostream>
#include <algorithm>
#include "parameters.h"

Graph::Graph(bool m_simple, bool m_weighted, bool m_directed) {
	simple = m_simple;
	weighted = m_weighted;
	directed = m_directed;

	totalVertices = 0;
	total2Edges = 0;
	total2Weights = 0.0;
	iq=0;
	sumSquareDegrees = 0;
	maxDegree = -1;
	maxOutDegree = -1;
	maxInDegree = -1;
	minWeight = -1;
	maxWeight = -1;
	maxStrength = -1.0;
	maxVertex=-1;
}

Graph::~Graph() {
	std::list<Vertex*>::iterator vertex_it;
	for (vertex_it=vertices.begin();vertex_it!=vertices.end();vertex_it++) {
		delete(*vertex_it);
	}
}

Graph* Graph::induceGraph(std::list<Vertex*> vl) {
	Graph* h = new Graph();
	std::list<Vertex*>::iterator vertex_it;
	for (vertex_it=vl.begin();vertex_it!=vl.end();vertex_it++) {
		h->addVertex(*vertex_it);
	}
	return h;
}

std::map<double, int> Graph::getDegreeDistribution() {
	std::map<double, int> dist;
	std::list<Vertex*>::iterator v_it;
	int d;
	for (v_it = vertices.begin(); v_it != vertices.end() ; v_it++) {
		d = (*v_it)->getNeighbours()->size();
		if (dist.find(d) != dist.end())
			dist[d]++;
		else
			dist[d] = 1;
	}
	return dist;
}

Vertex* Graph::getVertex(int i) {
	std::map<int, Vertex*>::iterator it;
	it = vertices_map.find(i);
	if (it == vertices_map.end()) {
		ExceptionVertexNotExistent e(i);
		throw e;
	}
	else
		return it->second;
}

void Graph::addEdge(unsigned int n1, unsigned int n2, double w) {
	if ((w!=1.0)&&(!weighted)) {
		ExceptionGraphNotWeighted e;
		throw e;
	}
	//std::cout << n1 << " " << n2 << std::endl;
	//If a vertex does not exist, it's created
	Vertex *vt1, *vt2;
	if (vertices_map.find(n1)==vertices_map.end()) {
		Vertex* vt = new Vertex(n1, directed);
		totalVertices++;
		vertices.push_back(vt);
		vertices_map[n1] = vt;
		vt1 = vt;
		if ((int)n1>maxVertex)
			maxVertex=n1;
		iq++;
	}
	else
		vt1 = vertices_map[n1];

	if (vertices_map.find(n2)==vertices_map.end()) {
		Vertex* vt = new Vertex(n2, directed);
		totalVertices++;
		vertices.push_back(vt);
		vertices_map[n2] = vt;
		vt2 = vt;
		if ((int)n2>maxVertex)
			maxVertex=n2;
		iq++;
	}
	else
		vt2 = vertices_map[n2];

	if ((!simple)||(vt1->hasNeighbour(vt2) == false)) {
		vt1->addNeighbour(vt2, w);
		total2Edges+=1;
		total2Weights+=w;

		if ((n1!=n2)&&(!directed)) {
			vt2->addNeighbour(vt1, w);
			total2Edges+=1;
			total2Weights+=w;

			if (vt1->getDegree() > maxDegree)
				maxDegree = vt1->getDegree();	

			if (vt1->getStrength() > maxStrength)
				maxStrength = vt1->getStrength();	

			if (vt2->getDegree() > maxDegree)
				maxDegree = vt2->getDegree();	

			if (vt2->getStrength() > maxStrength)
				maxStrength = vt2->getStrength();	

			if (w > maxWeight) {
				maxWeight = w;
			}
			if (((w < minWeight)||(minWeight==-1))&&(w!=0)) {
				minWeight = w;
			}

			sumSquareDegrees += 2*vt1->getDegree() - 1 + 2*vt2->getDegree() - 1;

		}
		else if (directed) {
			vt2->addIsNeighbourOf(vt1, w);

			if (vt1->getOutDegree() > maxOutDegree)
				maxOutDegree = vt1->getOutDegree();	

			if (vt2->getInDegree() > maxInDegree)
				maxInDegree = vt2->getInDegree();	

		}

		//Communities
//		if (v1->getCommunity() != NULL) {
			//v2->addCommunityCut(v1->getCommunity());
//			v2->getCommunity()->addCommunityCut(v1->getCommunity());
//		}
//		if (v2->getCommunity() != NULL) {
			//v1->addCommunityCut(v2->getCommunity());
//			v1->getCommunity()->addCommunityCut(v2->getCommunity());
//		}

	}
}

void Graph::buildDegreesList() {
	degreesList.clear();
	std::map<int, Vertex*>::iterator it;
	for (it=vertices_map.begin();it!=vertices_map.end();it++) {
		degreesList[(it)->second->getDegree()][(it)->second->getNumber()]=(it)->second;
	}
}

void Graph::printGraph() {
	std::cout << "Graph BEGIN" << std::endl;
	std::vector<Vertex*>* pNeighbours;
	std::list<Vertex*>::iterator it;
	std::vector<Vertex*>::iterator it2;
	for (it=vertices.begin();it!=vertices.end();it++) {
		std::cout << (*it)->getNumber() << " --> ";
		pNeighbours = (*it)->getNeighbours();
		for (it2=pNeighbours->begin();it2!=pNeighbours->end();it2++) {
			std::cout << (*it2)->getNumber() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Graph END" << std::endl;
}

void Graph::printDegreesList() {
	std::cout << "Degrees List BEGIN" << std::endl;
	std::map<int, std::map<int, Vertex*> >::iterator it;
	std::map<int, Vertex*>::iterator it2;
	for (it=degreesList.begin();it!=degreesList.end();it++) {
		std::cout << it->first << ": ";
		for (it2=it->second.begin();it2!=it->second.end();it2++)
			std::cout << (it2)->first << " ";
		std::cout << std::endl;
	}
	std::cout << "Degrees List END" << std::endl;
}

int Graph::getMinDegree() {
	int minDegree = -1;
	std::list<Vertex*>::iterator it;
	for (it=vertices.begin();it!=vertices.end();it++) {
		if (((*it)->getDegree() < minDegree)||(minDegree==-1))
			minDegree = (*it)->getDegree();
	}
	return minDegree;
}

//Adds a list of vertices to the graph. Those vertex neighbours that don't belong to the graph, are removed from the list of neighbours. The vertices are copies from the parameter. The vertices are returned.
std::vector<Vertex*> Graph::addVertices(std::vector<Vertex*>& vertices) {

	std::vector<Vertex*> new_vertices;
	Vertex* v;

	std::vector<Vertex*>::iterator it;
	for (it=vertices.begin();it!=vertices.end();it++) {
		v = addVertex(*it);
		new_vertices.push_back(v);
	}
	return new_vertices;
}

std::list<Vertex*> Graph::addVertices(std::list<Vertex*>& vertices) {

	std::list<Vertex*> new_vertices;
	Vertex* v;

	std::list<Vertex*>::iterator it;
	for (it=vertices.begin();it!=vertices.end();it++) {
		v = addVertex(*it);
		new_vertices.push_back(v);
	}
	return new_vertices;
}

//Adds a vertex to the graph. Those vertex neighbours that don't belong to the graph, are removed from the list of neighbours. The vertex is a copy from the parameter. The vertex is returned.
Vertex* Graph::addVertex(Vertex* vn) {
	Vertex* vertex = NULL;
	std::vector<Vertex*>::iterator it;
	std::vector<double>::iterator it_w;
	std::map<int, Vertex*>::iterator it1, it2;
	Vertex *vt1, *vt2;
	it1 = vertices_map.find(vn->getNumber());
//	if (it1==vertices_map.end()) {
	if (vertices_map.find(vn->getNumber())==vertices_map.end()) {
		vertex = new Vertex(vn->getNumber(), directed);
		totalVertices++;
		vertices.push_back(vertex);
		vertices_map[vn->getNumber()] = vertex;
		vt1 = vertex;
		if (vn->getNumber()>maxVertex)
			maxVertex=vn->getNumber();
		iq++;
	}
	else {
		vt1 = it1->second;
		vertex = vertices_map[vn->getNumber()];
	}

	it_w = vn->getWeights().begin();
	for (it=vn->getNeighbours()->begin();it!=vn->getNeighbours()->end();it++) {
		//If the neighbour exists in this graph
		it2 = vertices_map.find((*it)->getNumber());
		if (it2!=vertices_map.end()) {
			if (vertex->hasNeighbour(it2->second)==false) {
				(it2->second)->addNeighbour(vertex, *it_w);
				vertex->addNeighbour(it2->second, *it_w);
				total2Weights+=*it_w;
				total2Edges++;
				if (vertex->getNumber()!=(it2->second)->getNumber()) {
					total2Weights+=*it_w;
					total2Edges++;
				}
			if (*it_w > maxWeight) {
				maxWeight = *it_w;
			}
			if (((*it_w < minWeight)||(minWeight==-1))&&(*it_w!=0)) {
				minWeight = *it_w;
			}
				sumSquareDegrees += 2*(it2->second)->getDegree() - 1 + 2*vertex->getDegree() - 1;
			}
			if ((it2->second)->getDegree() > maxDegree) {
				maxDegree = (it2->second)->getDegree();	
			}
			if ((it2->second)->getStrength() > maxStrength)
				maxStrength = (it2->second)->getStrength();	

			//Communities
			//if ((it2->second)->getCommunity() != NULL) {
				//vertex->addCommunityCut((it2->second)->getCommunity());
				//vertex->getCommunity()->addCommunityCut((it2->second)->getCommunity());
			//}
		}
		it_w++;
	}
	if (vertex->getDegree() > maxDegree)
		maxDegree = vertex->getDegree();	
	if (vertex->getStrength() > maxStrength)
		maxStrength = vertex->getStrength();	

	return vertex;
}

//Removes a vertex from the graph.
void Graph::delVertex(Vertex* vn) {
	std::map<int, Vertex*>::iterator it1, it2;
	it1 = vertices_map.find(vn->getNumber());
//	if (it1==vertices_map.end()) {
	if (vertices_map.find(vn->getNumber())!=vertices_map.end()) {
		vertices_map.erase(vn->getNumber());
	}
	totalVertices--;
	vertices.remove(vn);

	std::vector<Vertex*>::iterator it;
	for (it=vn->getNeighbours()->begin();it!=vn->getNeighbours()->end();it++) {
		//If the neighbour exists in this graph
		(*it)->deleteNeighbour(vn);
		total2Edges--;
		total2Edges--;
	}
}

bool Graph::computeDiameter2() {
	unsigned int ov = (pow(2, 31) - 1);
	ov += pow(2, 31);
	unsigned int maV = vertices.size();

	//Constructing vectors
	std::vector<unsigned int> v;
	v.resize((maV-1)/32 + 1);
	for (unsigned int i=0;i<(maV-1)/32 + 1;i++)
		v[i] = 0;
	if ((maV) % 32 != 0) {
		v[(maV-1)/32] = ov - (unsigned int)round(pow(2, maV % 32) - 1);
	}

	std::vector<std::vector<unsigned int> > vl, vl2;
	for (unsigned int i=0;i<maV;i++)
		vl.push_back(v);
	for (unsigned int i=0;i<maV;i++)
		vl2.push_back(v);
	
	std::list<Vertex*>::iterator v_it, it;
	std::vector<Vertex*>::iterator nb_it;

	unsigned int i=0;
	for (v_it=vertices.begin();v_it!=vertices.end();v_it++) {
		Vertex* v = *v_it;
		unsigned int posArray = i/32;
		vl[i][posArray] = vl[i][posArray] | (unsigned int)round(pow(2, i % 32));
		for (nb_it=v->getNeighbours()->begin();nb_it!=v->getNeighbours()->end();nb_it++) {
			Vertex* n = *nb_it;
			unsigned int number=0;
			for (it=vertices.begin();*it!=n;it++)
				number++;
			unsigned int posArray = number/32;
			vl[i][posArray] = vl[i][posArray] | (unsigned int)round(pow(2, number % 32));
		}
		i++;
	}

	//ORs
	i=0;
	for (v_it=vertices.begin();v_it!=vertices.end();v_it++) {
		Vertex* v = *v_it;


		for (unsigned int j=0;j<=(maV-1)/32;j++)
			vl2[i][j] = vl[i][j];

		for (nb_it=v->getNeighbours()->begin();nb_it!=v->getNeighbours()->end();nb_it++) {
			Vertex* n = *nb_it;
			unsigned int number=0;
			for (it=vertices.begin();*it!=n;it++)
				number++;
			for (unsigned int j=0;j<=(maV-1)/32;j++)
				vl2[i][j] = vl2[i][j] | vl[number][j];
		}
		//Prunsigned intf.prunsigned intf "Vector %d: %x\n" n#getNumberInFile (!vl2).(i).(0);
		i++;
	}

	//Verifying
	unsigned int result = 2;
	for (unsigned int i=0;i<=maV-1;i++)
		for (unsigned int j=0;j<=(maV-1)/32;j++) {
			if (vl2[i][j] != ov) {
				//std::cout << "i,j,Diff: " << i << " " << j << ov - vl2[i][j] << std::endl;
				result = 3;
			}
		}

	return (result==2);
}

//TODO
/**method endGraph =
		(**Sorting of strengths list, if it's a weighted graph*)
		if (Parameters.parameters#getWeighted = true)&&(Parameters.parameters#getMultigraph = false) then
		begin
			for i= 0 to (List.length vertices * arraySize -1) do
				let posList = i / arraySize in
				if (List.nth vertices posList).(i mod arraySize)#isVertex then
				begin
					let strength = (List.nth vertices posList).(i mod arraySize)#getStrength in
					strengths <- strength::strengths;
					if strength > maxStrength then
						maxStrength <- strength;
				end
			done;
			strengths <- List.sort (fun a b -> (if a > b then 1 else -1)) strengths;

			(**let rec printStrengthsList w = 
			match w with
				[] -> ()
				| a :: l ->
					Printf.printf "%f " a;
					printStrengthsList l;
			in
			printStrengthsList strengths;
			Printf.printf "\n";*)
			(**p-function is built*)
			self#buildPFunction;
		end;

		(**p-function is applied to every vertex (every vertex applies it to each of its edges*)
		for i= 0 to (List.length vertices * arraySize -1) do
			let posList = i / arraySize in
			if (List.nth vertices posList).(i mod arraySize)#isVertex then
			begin
				let p = (List.nth vertices posList).(i mod arraySize)#getP pFunction (-2) in
				if p > maxPValue then
					maxPValue <- p;
			end
			
		done;
*/

Graph* Graph::extractComponent(ConnectedComponent* comp) {
	Graph* n2 = new Graph(simple, weighted);
//	n2->initialize(maxVertex);
	std::vector<Vertex*>::iterator it = comp->vertices.begin();
	while (it!=comp->vertices.end()) {
		std::vector<Vertex*>::iterator it2 = (*it)->getNeighbours()->begin();
		std::vector<double>::iterator it3 = (*it)->getWeights().begin();
		while (it2!=(*it)->getNeighbours()->end()) {
			//std::cout << "Agregando eje " << (*it)->getNumber() << " " << (*it2)->getNumber() << std::endl;
			n2->addEdge((*it)->getNumber(),(*it2)->getNumber(), (*it3));
			it2++;
			it3++;
		}
		it++;
	}
	return n2;
}

void Graph::addNodesToConnectedComponent(ConnectedComponent* comp, Vertex* v) {
	std::list<Vertex*> pile;
	pile.push_back(v);
	comp->addVertex(v);
	mark[v]=1;
	Vertex* f;
	while (!pile.empty()) {
		f = pile.front();
		pile.pop_front();
		std::vector<Vertex*>::iterator neighbours = f->getNeighbours()->begin();
		while (neighbours != f->getNeighbours()->end()) {
			if (mark.find(*neighbours)==mark.end()) {
				pile.push_back(*neighbours);
				comp->addVertex(*neighbours);
				mark[*neighbours]=1;
			}
			neighbours++;
		}
	}
};

void Graph::findConnectedComponents() {
	mark.clear();
	std::list<Vertex*>::iterator vertex_it;

	for (vertex_it = vertices.begin(); vertex_it != vertices.end() ; vertex_it++) {
		if (mark.find(*vertex_it)==mark.end()) {
			ConnectedComponent* comp = new ConnectedComponent();
			connectedComponents.push_back(comp);
			addNodesToConnectedComponent(comp, *vertex_it);
		}
	}
}

void Graph::printConnectedComponents() {
	std::vector<ConnectedComponent*>::iterator it = connectedComponents.begin();
	while (it!=connectedComponents.end()) {
		(*it)->printConnectedComponent();
		it++;
	}
}

ConnectedComponent* Graph::getBiggestConnectedComponent() {
	if (connectedComponents.size()==0) {
		throw ExceptionImproperOrder("findConnectedComponents()", "getBiggestConnectedComponent()");
	}
	ConnectedComponent* biggest;
	int biggestSize = -1;
	std::vector<ConnectedComponent*>::iterator it = connectedComponents.begin();
	while (it!=connectedComponents.end()) {
		if ((int)(*it)->vertices.size() > biggestSize) {
			biggest = *it;
			biggestSize = (*it)->vertices.size();
		}
		it++;
	}
	return biggest;
}

std::vector<ConnectedComponent*>& Graph::getConnectedComponents() {
	return connectedComponents;
}

