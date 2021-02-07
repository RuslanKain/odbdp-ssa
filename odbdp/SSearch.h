//
// Created by Riccardo Mereu on 15/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#ifndef ODBDP_SSEARCH_H
#define ODBDP_SSEARCH_H

#include "ODBP.h"
#include "Population.h"
#include "RefSet.h"
#include "Member.h"
#include "timer.h"

class SSearch {
	ODBP problem;
	Population pop;
	RefSet refSet;
	int maxIteration;
	//void init(char*);

public:
	SSearch(char *filein, char* fileout, char* txtout);
	SSearch(int sizePop, int sizeRef, int maxIter, char* filein, char* fileout, char* txtout);
	void printInstance();

	void solve(int);
};


#endif //ODBDP_SSEARCH_H
