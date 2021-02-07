//
// Created by Riccardo Mereu on 14/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#include "ODBP.h"
#include "RefSet.h"
#include <utility>
#include <iostream>
#include <ctime>
using namespace std;

ODBP::ODBP(char* infile){
	t0 = clock();
	if (freopen(infile, "r", stdin) == NULL) {
		fprintf(stderr, "Input error: %s not found", infile);
		exit(-1);
	}

	int i, j, tmp;
	best = 0;
	int cnt = 1;
	clock_t t2 = NULL;
	scanf("%*s %d\n", &nquery);
	scanf("%*s %d\n", &nindex);
	scanf("%*s %d\n", &nconfig);
	scanf("%*s %d\n", &M);

	// input matrix CONFIGURATIONS_INDEXES_MATRIX into e
	e = vector<boost::dynamic_bitset<>>(nconfig+1);
	e[0] = boost::dynamic_bitset<>(nindex);
	scanf("%*s \n");
	for ( i = 1; i <= nconfig; i++) {
		e[i] = boost::dynamic_bitset<>(nindex);
		for ( j = 0; j < nindex; j++){
			scanf("%d", &tmp);
			if(tmp == 1)
				e[i].set(j);
		}
	}

	// input array INDEXES_FIXED_COST into f
	f = vector<int>(nindex);
	scanf("%*s \n");
	for ( i = 0; i < nindex; i++) {
		scanf("%d", &f[i]);
	}

	// input matrix INDEXES_MEMORY_OCCUPATION into m
	m = vector<int>(nindex);
	scanf("%*s \n");
	for ( i = 0; i < nindex; i++) {
		scanf("%d", &m[i]);
	}

	// input matrix CONFIGURATIONS_QUERIES_GAIN into g
	g = vector<vector<int>>(nconfig+1);

	//g = boost::multi_array<int, 2>(boost::extents[nconfig][nquery]);
	scanf("%*s \n");
	g[0] = vector<int>(nquery, 0);
	for ( i = 1; i <= nconfig; i++) {
		g[i] = vector<int>(nquery);
		for (j = 0; j < nquery; j++) {
			scanf("%d", &g[i][j]);
		}
	}
	t1 = clock();
	
}

void ODBP::update_best(int objf, boost::dynamic_bitset<> sol, std::vector<int> query){
	if (objf > best) {
		this->cnt++;
		t2 = clock();
		float time_best = float(t2-t1) / CLOCKS_PER_SEC;
		float time_file_read = float(t1 - t0) / CLOCKS_PER_SEC;
		cout << "Time to read instance " << time_file_read << endl;
		best = objf;
		conf_best = boost::dynamic_bitset<>(sol);
		query_best = std::vector<int>(query);
		print_best();
		cout << "BEST------------------------------------------->" << this->best << endl;
		FILE* t = fopen(this->txtout, "a");
		fprintf(t, "ParseTime: %f ", time_file_read);
		fprintf(t, "Best: %d ", best);
		fprintf(t, "Time: %f", time_best);
		fprintf(t, "\n");
		fclose(t);
	}
}

void ODBP::print_best(){

	FILE* f = fopen(this->fileout, "w");
	for (int i = 1; i < this->conf_best.size(); i++) {
		for (int q = 0; q < this->nquery; q++) {
			if (query_best[q] == i) {
				fprintf(f, "1 ");
			}
			else {
				fprintf(f, "0 ");
			}
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void ODBP::print(){
	int i, j;
	cout<<"nindex: "<<nindex<<endl;
	cout<<"nquery:"<<nquery<<endl;
	cout<<"nconfig: "<<nconfig<<endl;
	cout<<"Memory: "<<M<<endl;

	cout<<"e matrix:"<<endl;
	for ( i = 0; i <= nconfig; i++) {
		for(j=0; j< nindex; j++){
			cout<<e[i][j];
		}
		cout<<endl;
	}
	cout<<"fixed costs:"<<endl;
	for ( i = 0; i < nindex; i++) {
		cout<<f[i]<<endl;
	}
	cout<<"memory: "<<endl;
	for ( i = 0; i < nindex; i++) {
		cout<<m[i]<<endl;
	}
	cout<<"gains:"<<endl;
	for ( i = 0; i <= nconfig; i++) {
		for (j = 0; j < nquery; j++) {
			cout<<g[i][j]<<" ";
		}
		cout<<endl;
	}
}
