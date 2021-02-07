//
// Created by Riccardo Mereu on 15/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#include "SSearch.h"
#include <iostream>


SSearch::SSearch(char *filein, char* fileout, char* txtout):problem(filein) {
	
	// Specify ratio formula, uncomment one to select
	// int ratio = problem.nconfig / problem.nquery; // RS1 in paper
	// int ratio = (problem.nconfig*problem.nindex) / (problem.nquery*100); // RS2 in paper
	int ratio = (problem.nconfig*problem.nindex) / (problem.nquery * 50); // RS3 in paper
	// int ratio = problem.nconfig / 100; // RS5 in paper

	// Specify population and reference set size based on ratio
	pop = Population((50 > ratio) ? 50 : ratio * 10, &problem);
	refSet = RefSet((10 > ratio) ? 10 : ratio, &problem);
	problem.filein = filein;
	problem.fileout = fileout;
	problem.txtout = txtout;
}


SSearch::SSearch(int sizeP, int sizeR, int maxiter, char* filein, char* fileout, char* txtout) : problem(filein){
	maxIteration = maxiter;
	pop = Population(sizeP, &problem);
	refSet = RefSet(sizeR, &problem);
	problem.filein = filein;
	problem.fileout = fileout;
	problem.txtout = txtout;
}


void SSearch::printInstance() {
	problem.print();
}

void SSearch::solve(int maxIteration){
	//diversification method
	pop.create();

	std::cout<<"BEST : "<<problem.best<<std::endl;

	refSet.create(pop);

	std::cout<<"population size   : "<<pop.divSet.size()<<"\n";
	std::cout<<"referenceSet size : "<<refSet.referenceSet.size()<<"\n";

	for(int iter = 0; iter < maxIteration; iter++){
		std::cout<<iter<<std::endl;
		if(refSet.isnewsolution()){
			std::cout<<"update"<<std::endl;
			refSet.update(iter);
		}else{
			std::cout<<" rebuild "<<std::endl;
			refSet.rebuild(iter, pop.sizePop);
		}
		std::cout<<"\nBEST : "<<problem.best<<std::endl;
	}

	std::cout<<"\n BEST : "<<problem.best<<std::endl;
}


