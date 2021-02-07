//
// Created by Riccardo Mereu on 14/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#ifndef ODBDP_POPULATION_H
#define ODBDP_POPULATION_H

#include <set>
#include <boost/dynamic_bitset.hpp>
#include <random>
#include "Member.h"
#include <unordered_set>

class Population {
	ODBP* problem;
public:
	std::set<boost::dynamic_bitset<>> created;
	int sizePop;
	Population(){}
	Population(int sizeP, ODBP* inst);
	void create();
	void create(boost::dynamic_bitset<>);
	std::set<Member> divSet;

	boost::dynamic_bitset<> random_seed();
	boost::dynamic_bitset<> random_seed(boost::dynamic_bitset<>);
};



#endif //ODBDP_POPULATION_H
