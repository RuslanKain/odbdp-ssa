//
// Created by Riccardo Mereu on 14/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#ifndef ODBDP_MEMBER_H
#define ODBDP_MEMBER_H
#include "ODBP.h"
#include <set>
#include <vector>
#include <map>

#include "boost/dynamic_bitset.hpp"


class Member {
private:
	ODBP* problem;

	void feasible(boost::dynamic_bitset<>);
	//void feasible_(boost::dynamic_bitset<>);

	void improve();
	//void improve_();

public:
	std::vector<int> query;
	boost::dynamic_bitset<> configuration;
	boost::dynamic_bitset<> index;

	std::vector<int> indexnum;
	std::set<int> conf_used;
	int objf;
	int totalgain;
	int memoccupation;
	int fixedcosts;

	int iteration;

	Member() {}
	Member(boost::dynamic_bitset<> sol, ODBP*);
	//Member(boost::dynamic_bitset<> sol, ODBP*, bool);

	bool operator==(const Member& m) const {
		return configuration == m.configuration;
	}
	bool operator!=(const Member& m) const {
		return configuration != m.configuration;
	}

	bool operator<(const Member& m) const {
		if (objf == m.objf) {
			return memoccupation < m.memoccupation;
		}
		return objf < m.objf;
	}

	bool operator>(const Member& m) const {
		if (objf == m.objf) {
			return configuration > m.configuration;
		}
		return objf > m.objf;
	}

	bool operator()(const Member& lhs, const Member& rhs) const {
		return lhs.objf < rhs.objf;
	}


};

#endif //ODBDP_MEMBER_H
