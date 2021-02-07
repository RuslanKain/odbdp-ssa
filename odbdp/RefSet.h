//
// Created by Riccardo Mereu on 15/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#ifndef ODBDP_REFSET_H
#define ODBDP_REFSET_H

#include <set>
#include <boost/dynamic_bitset.hpp>
#include <random>
#include <limits>
#include "Member.h"
#include "Population.h"
#include "ODBP.h"


class RefSet {
	int sizeRef;
	int current_iter;

	Population* pop;
	ODBP* problem;

	bool newsolution;
	//std::set<Member> pool;
	std::set<boost::dynamic_bitset<>> pool1; // pool of possible candidate seeds

	void combineRefSet();
	std::vector<boost::dynamic_bitset<>> combine(Member, Member);
public:
	
	RefSet(){}
	RefSet(int , ODBP*);

	std::set<Member> referenceSet;

	void create(Population);

	void update(int);
	void rebuild(int, int);


	//void rebuild_random(int iter);
	//void rebuild_random_seed(int iter);

	bool isnewsolution();
};


#endif //ODBDP_REFSET_H
