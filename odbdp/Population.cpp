//
// Created by Riccardo Mereu on 14/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#include "Population.h"
#include <iostream>
using namespace std;


Population::Population(int sizeP, ODBP* inst) {
	cout << "Population :: sizeP = " << sizeP << endl;
	problem = inst;
	sizePop = sizeP;

}

void Population::create(){
	// seed of all zeroes 
	//boost::dynamic_bitset<> seed(problem->nconfig + 1);
	//create(seed);

	create(random_seed());
}


boost::dynamic_bitset<> Population::random_seed() {
	boost::dynamic_bitset<> seed(problem->nconfig + 1);
	std::default_random_engine eng{static_cast<unsigned int>(time(0)) };
	std::uniform_int_distribution<int> uid(0, problem->nconfig+1);
	//std::uniform_int_distribution<int> uid1(1, problem->nconfig);
	int nset =(unsigned int) uid(eng);
	int i;

	std::uniform_real_distribution<double> urd(0.0, 1.0);
	for (i = 0; i < seed.size(); i++) {
		seed[i] = (urd(eng) > 0.5) ? 1 : 0;
	}

	return seed;
}

boost::dynamic_bitset<> Population::random_seed(boost::dynamic_bitset<> seed) {
	std::default_random_engine eng{ static_cast<unsigned int>(time(0)) };
	std::uniform_int_distribution<int> uid(0, problem->nconfig + 1);
	std::uniform_int_distribution<int> uid1(1, problem->nconfig);

	int nset = (unsigned int)uid(eng);
	for(int i = 0; i<nset; i++){
		int tmp = (unsigned int)uid1(eng);
		seed.set(tmp);
	}
	return seed;
}


void Population::create(boost::dynamic_bitset<>seed){
	int max_solution, max_h, max_q, max_k;
	int h, q, k, iter = 0;

	boost::dynamic_bitset<>  sol(seed);

	max_solution = sizePop;
	max_h = problem -> nconfig - 1;


	while(iter++<2 && divSet.size()<=max_solution){
		seed = boost::dynamic_bitset<>(sol);
		h = 2;
		while(h<=max_h  && divSet.size()<=max_solution){
			q = 1; max_q = h;
			while(q<=max_q && divSet.size()<max_solution-1){
				sol = boost::dynamic_bitset<>(seed);
				max_k = (problem->nconfig - q) / h;
				for(k=0; k<=max_k; k++){
					sol[q+k*h] = 1 - seed[q+k*h];
				}

				Member candidate(sol, problem);
				Member ncandidate(~sol, problem);
				divSet.insert(candidate);
				divSet.insert(ncandidate);
				if (divSet.size() % 10 == 0) cout << divSet.size() << " " << max_solution << endl;
		
				q++;
			}
			h++;
		}
	}
}
