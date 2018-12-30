#include "utils.h"
#include <iostream>

double sign(double n) {
	if (n>=0)
		return 1;
	return -1;
}

double abs(double n) {
	if (n>=0)
		return n;
	return -n;
}

double min(double a, double b) {
	if (a<b)
		return a;
	return b;
}

std::list<int> intersection(std::list<int> list1, std::list<int> list2) {
	list1.sort();
	list2.sort();
	std::list<int>::iterator it_vertices1, it_vertices2;
	bool avanzar = true;

	std::list<int> list_result;

	it_vertices2 = list2.begin();
	for (it_vertices1 = list1.begin(); it_vertices1 != list1.end(); it_vertices1++) {
		//std::cout << "Tomo el vertice  " << *it_vertices1 << std::endl;
		avanzar = true;
		while (avanzar) {
			//std::cout << "Contra el vertice  " << *it_vertices2 << std::endl;
			if (it_vertices2 == list2.end()) 
				avanzar = false;
			else {
				if (*it_vertices2 == *it_vertices1) {
					list_result.push_back(*it_vertices1);
				}
				if (*it_vertices2 < *it_vertices1) {
					//std::cout << "Coinciden " << *it_vertices2 << " y " << *it_vertices1 << "\n";
					it_vertices2++;
					avanzar = true;
				}
				else
					avanzar = false;
			}
		}
	}
	return list_result;
}

int compara_comm(std::map<int, std::list<int> > partition_1, std::map<int, std::list<int> > partition_2) {

	std::map<int, bool> comunidades1_usadas, comunidades2_usadas;

	int n1, n2;
	int cant_vertices = 0;
	
	std::map<int, std::list<int> >::iterator it_comunidades1, it_comunidades2;
	std::list<int>::iterator it_vertices1, it_vertices2;
	std::map<std::pair<int, int>, int> coincidencias;

	int puntaje_total = 0, mejor1, mejor2;
	int iguales = 0;

	for (it_comunidades1 = partition_1.begin(); it_comunidades1 != partition_1.end(); it_comunidades1++) {
		it_comunidades1->second.sort();
	}
	for (it_comunidades2 = partition_2.begin(); it_comunidades2 != partition_2.end(); it_comunidades2++) {
		it_comunidades2->second.sort();
		cant_vertices += it_comunidades2->second.size();
	}

	for (it_comunidades1 = partition_1.begin(); it_comunidades1 != partition_1.end(); it_comunidades1++) {
		for (it_comunidades2 = partition_2.begin(); it_comunidades2 != partition_2.end(); it_comunidades2++) {
	
			it_vertices2 = it_comunidades2->second.begin();
			bool avanzar = true;
			int puntaje_comunidad2 = 0;
			for (it_vertices1 = it_comunidades1->second.begin(); it_vertices1 != it_comunidades1->second.end(); it_vertices1++) {
				//std::cout << "Tomo el vertice  " << *it_vertices1 << std::endl;
				avanzar = true;
				while (avanzar) {
					//std::cout << "Contra el vertice  " << *it_vertices2 << std::endl;
					if (it_vertices2 == it_comunidades2->second.end()) 
						avanzar = false;
					else {
						if (*it_vertices2 == *it_vertices1) {
							puntaje_comunidad2++;
						}
						if (*it_vertices2 <= *it_vertices1) {
							//std::cout << "Coinciden " << *it_vertices2 << " y " << *it_vertices1 << "\n";
							it_vertices2++;
							avanzar = true;
						}
						else
							avanzar = false;
					}
				}
			}
			std::pair<int, int> p;
			p.first = it_comunidades1->first;
			p.second = it_comunidades2->first;
			coincidencias[p] = puntaje_comunidad2;
		}
	}

	int i=0;
	while (1) {
		int puntaje_mejor_comunidad2 = -1;

		std::map<std::pair<int, int>, int>::iterator it_par;
		for (it_par = coincidencias.begin(); it_par != coincidencias.end(); it_par++) {
				if (it_par->second > puntaje_mejor_comunidad2) {
					mejor1 = it_par->first.first;
					mejor2 = it_par->first.second;
					puntaje_mejor_comunidad2 = it_par->second;
//					std::cout << "Lo mejor es " << puntaje_mejor_comunidad2 << " entre " << it_par->first.first << " y " << it_par->first.second << std::endl;

				}
		}

		if (puntaje_mejor_comunidad2 != -1) {
			//std::cout << "Comunidades " << mejor1 << " y " << mejor2 << "(" << puntaje_mejor_comunidad2 << "/" << (partition_1[mejor1].size() + partition_2[mejor2].size()) / 2 << ") Patrón: " << partition_1[mejor1].size() << " Segunda: " << partition_2[mejor2].size() << ". " << std::endl;
			i++;
			//std::cout << i << " " << (int)partition_1[mejor1].size() - puntaje_mejor_comunidad2 << " " << puntaje_mejor_comunidad2 << " " << (int)partition_2[mejor2].size() - puntaje_mejor_comunidad2 << " " << mejor1 << " " << mejor2 << std::endl;
/*			if (puntaje_mejor_comunidad2 == (partition_1[mejor1].size() + partition_2[mejor2].size()) / 2) {
				iguales++;
				std::cout << "(" << iguales << ")";
			}
			std::cout << std::endl;
*/			partition_1.erase(mejor1);
			partition_2.erase(mejor2);
			for (it_par = coincidencias.begin(); it_par != coincidencias.end(); it_par++) {
					if ((it_par->first.first == mejor1) || (it_par->first.second == mejor2))  {
						it_par->second = -1;
					}
			}
			puntaje_total += puntaje_mejor_comunidad2;
		}
		else
			break;
	}
//	std::cout << "Puntaje: " << puntaje_total*100/cant_vertices << "\%" << std::endl;	
	return puntaje_total;
}

