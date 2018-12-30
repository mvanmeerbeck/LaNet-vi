#ifndef _UTILS_H_
#define _UTILS_H_

#include <list>
#include <vector>
#include <map>

double sign(double n);

//double abs(double n);

double min(double a, double b);

std::list<int> intersection(std::list<int> list1, std::list<int> list2);

int compara_comm(std::map<int, std::list<int> > partition_1, std::map<int, std::list<int> > partition_2);

#endif

