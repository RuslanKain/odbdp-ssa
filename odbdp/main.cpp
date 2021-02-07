// OMA Project: ODBDP
// Created by Riccardo Mereu on 07/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#include <iostream>
#include "SSearch.h"
#include <ctime>
using namespace std;

int main(int argc, char **argv) {
	if (argc > 4 || argc < 2) {
		fprintf(stderr, "Wrong usage: read inputfile [-t time]\n");
		return -1;
	}

	//name of output file
	char* filein = argv[1];
	std::string sfileout;
	std::string stxtout;
	sfileout.append(argv[1]);
	stxtout.append(argv[1]);
	size_t lastdot = sfileout.find_last_of(".");
	size_t lastdottxt = stxtout.find_last_of(".");
	if (lastdot != string::npos) {
		sfileout = sfileout.substr(0, lastdot);
		stxtout = stxtout.substr(0, lastdottxt);
	}
	sfileout.append(".sol");
	stxtout.append(".txt");
	char* fileout=_strdup(sfileout.c_str());
	char* txtout = _strdup(stxtout.c_str());

	//time limit
	if (argc == 4) {
		std::string c = argv[2];
		std::string time = argv[3];
		if (c == "-t") {
			float t = std::stoi(time);
			t *= 1000; //timer required in ms
			init_timer(t);
		}
		else {
			fprintf(stderr, "Wrong usage: read inputfile [-t time]\n");
			return -1;
		}
	}
	
	SSearch prova(filein, fileout, txtout);
	prova.solve(1000);

	//wait till the end of the timer
	if (argc == 4) {
		wait_timer();
	}
	
	return 0;

}


