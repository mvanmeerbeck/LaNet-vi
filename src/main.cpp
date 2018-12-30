#include <iostream>
#include "graph_builder_txt.h"
#include "graph_node_names.h"
#include "graph_node_colors.h"
#include "network.h"
#include "connectivity.h"
#include "stdlib.h"
#include "parameters.h"
#include "engine.h"
#include "povray.h"
#include "svg.h"
#include "graphics_kcores.h"
#include "graphics_kdenses.h"
#include "povray_renderer.h"
#include "svg_renderer.h"
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {

	std::cout << "Building graph...\n";
	Graph_builder_txt graph_builder;
	parameters.validateParameters(argc, argv);

	Network* graph = new Network(!parameters.getMultigraph(), parameters.getWeighted());
	graph_builder.readFile(parameters.getInputFile(), &(*graph));
	//graph.printGraph();

	if ((parameters.getInputNodeNamesFile()!="")&&(parameters.getInputNodeNamesFile()!="0")) {
		Graph_node_names node_names;
		node_names.readFile(*graph, graph->names, parameters.getInputNodeNamesFile());
	}
	if (parameters.getColorsFile()!="") {
		Graph_node_colors node_colors;
		node_colors.readFile(*graph, graph->colors, parameters.getColorsFile());
	}


	if (parameters.getDecomp()=="kdenses") {
		std::cout << "Generating k-dense decomposition...\n";
		graph->kdenses.findDenses();
		//graph->printDenses();
		graph->kdenses.logDenses();

		std::cout << "Finding components...\n";
		graph->kdenses_components.findComponents();
	//	graph->kdenses_components.printComponents();
		graph->kdenses_components.buildClustersMap();

		std::cout << "Finding coordinates...\n";
		graph->kdenses_components.findCoordinates();
		graph->kdenses_components.logCoordinates();
	}
	else {

		std::cout << "Generating k-core decomposition...\n";
//		graph->kcores.loadCores("/home/mbeiro/Escritorio/Facultad/Tesis Doctorado/data/data_cap4/IR_CAIDA_2011_10_r_cores.txt");
		graph->kcores.findCores();
		//graph->printCores();
		graph->kcores.logCores();

		if (parameters.getFromLayer()!=0) {
			std::cout << "Extracting central layers from " << parameters.getFromLayer() << "..." << std::endl;
			Network* g_layer = graph->getLayer(parameters.getFromLayer());
			std::vector<double> pf = graph->kcores.getPFunction();
			g_layer->kcores.findCores(&pf);
			graph->kcores.logCores();
			graph = g_layer;

			if ((parameters.getInputNodeNamesFile()!="")&&(parameters.getInputNodeNamesFile()!="0")) {
				Graph_node_names node_names;
				node_names.readFile(*graph, graph->names, parameters.getInputNodeNamesFile());
			}
		}

		std::cout << "Computing components and clusters...\n";
		graph->kcores_components.findComponents();
		graph->kcores_components.buildClustersMap();
		//graph->kcores.printComponents();

		//Getting biggest connected component
		/*std::cout << "Getting biggest component...\n";
		Network graph_biggest_component;
		std::list<Component*> connectedComponents = graph->getComponents();
		std::list<Component*>::iterator it;
		int biggestComponentSize = -1;
		Component* biggestComponent;
		for (it=connectedComponents.begin();it!=connectedComponents.end();it++) {
			if ((*it)->getSize()>biggestComponentSize) {
				biggestComponent = *it;
				biggestComponentSize = (*it)->getSize();
			}
		}
		std::list<Vertex*> listVertices = biggestComponent->getVertices();
		std::list<Vertex*>::iterator list_it;
		std::vector<Vertex*> vectorVertices;
		for (list_it=listVertices.begin();list_it!=listVertices.end();list_it++) {
			vectorVertices.push_back(*list_it);
		}
		graph_biggest_component.addVertices(vectorVertices);
		graph = graph_biggest_component;
		graph->buildDegreesList();
		graph->findCores();
		graph->findComponents();*/
	

		std::cout << "Finding coordinates...\n";
		graph->kcores_components.findCoordinates();
		graph->kcores_components.logCoordinates();

		if (parameters.getKConn()==true) {
			std::cout << "Determining k-connectivity...\n";
			graph->kcores_components.computeKConnectivity(parameters.getKConnType());
			graph->kcores_components.buildClustersMap();
			graph->kcores_components.logKConn();
		}

		//Computing edge-connectivity
		if (parameters.getConnectivity()) {
			std::cout << "Generating Gomory-Hu cut tree...\n";
			Connectivity g_conn(graph);
			g_conn.build_gomory_hu_tree(graph->tVertices());
			//std::cout << "Corte minimo: " << graph.find_minimum_cut() << std::endl;
			std::cout << "Finding edge-connectivity...\n";
			g_conn.find_connectivities();
		}

		if (parameters.getInnerConnectivity()) {
			for (int i=graph->kcores.getMaxShellIndex();i!=0;i--) {
				std::cout << "Generating Gomory-Hu cut tree for core " << i << "...\n";
				Network* g_layer = graph->getLayer(i);
				g_layer->kcores.findCores();
				g_layer->kcores_components.findComponents();
				g_layer->kcores_components.buildClustersMap();
				Connectivity g_conn_layer(g_layer);
				g_conn_layer.build_gomory_hu_tree(g_layer->tVertices());
				//std::cout << "Corte minimo: " << graph.find_minimum_cut() << std::endl;
				std::cout << "Finding inner edge-connectivity for this core...\n";
				g_conn_layer.find_inner_connectivities(i);
			}
		}
	}

	//Generating image
	Engine* e;
	if (parameters.getRenderer()=="povray") {
		std::cout << "Generating .pov file...\n";
		e = new Povray(parameters.getOutputFile());
	}
	else {
		std::cout << "Generating .svg file...\n";
		e = new Svg(parameters.getOutputFile());
	}

	if (parameters.getDecomp()=="kdenses") {
		Graphics_KDenses graphics(e);
		graphics.generateNetworkFile(graph);
	}
	else {
		Graphics_KCores graphics(e);
		graphics.generateNetworkFile(graph);
	}

	if (parameters.getNoGraphic()==false) {
		//Render visualization
		std::cout << "Rendering image...\n";
		if (parameters.getRenderer()=="povray")
			povray_renderer.render(parameters.getOutputFile(), parameters.getOutputPngFile().c_str());
		else
			svg_renderer.render(parameters.getOutputFile(), parameters.getOutputPngFile().c_str());
	}
	std::cout << "Done!\n";

}

