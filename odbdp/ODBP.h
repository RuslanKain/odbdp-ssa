//
// Created by Riccardo Mereu on 14/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//
// Represent the instance of the actual problem

#ifndef ODBDP_ODBP_H
#define ODBDP_ODBP_H
#include <bitset>
#include <vector>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include <boost/multi_array.hpp>

class ODBP {
public:
	ODBP(char*);
	char* filein;
	char* fileout;
	char* txtout;
	clock_t t0, t1, t2;
	float time_file_read;
	int nquery;
	int nconfig;
	int nindex;
	int M;
	int cnt;
	std::vector<boost::dynamic_bitset<>> e;
	std::vector<int> f;
	std::vector<int> m;
	std::vector<std::vector<int>> g;

	int best;
	boost::dynamic_bitset<> conf_best;
	std::vector<int> query_best;
	void update_best(int, boost::dynamic_bitset<>, std::vector<int>);
	void print_best();

	

	void print();

};


#endif //ODBDP_ODBP_H
