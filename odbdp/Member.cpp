//
// Created by Riccardo Mereu on 14/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//

#include "Member.h"
#include <limits>
#include <iostream>
using namespace std;


Member::Member(boost::dynamic_bitset<> sol, ODBP* inst) :configuration(inst->nconfig + 1),
index(inst->nindex),
indexnum(inst->nindex, 0),
query(inst->nquery, 0) {
	problem = inst;
	feasible(sol);
	improve();
	problem->update_best(this->objf, this->configuration, this->query);
	//cout<<objf<<" "<<problem->M<<" "<<memoccupation<<" "<<objf-problem->best<<endl;


}

void Member::feasible(boost::dynamic_bitset<> sol) {
	bool feasible = false;
	configuration = boost::dynamic_bitset<>(sol);
	while (!feasible) {
		objf = 0;
		memoccupation = 0;
		totalgain = 0;
		fixedcosts = 0;

		conf_used.clear();

		std::vector<int> conf_gain(problem->nconfig + 1, 0);
		std::vector<float> conf_fcost(problem->nconfig + 1, 0.0);
		std::vector<float> conf_mem(problem->nconfig + 1, 0.0);

		for (int q = 0; q < query.size(); q++) {
			int max = 0;//best gain for query q
			int maxc = 0; //configuration with the best gain for query q
			query[q] = 0;
			for (int i = 1; i < configuration.size(); i++) {
				if (configuration[i] == 1 and problem->g[i][q] > max) {
					max = problem->g[i][q];
					maxc = i;
				}
			}
			if (maxc != 0) {
				conf_used.insert(maxc);
				query[q] = maxc;
				conf_gain[maxc] += max;
				totalgain += max;
			}

		}

		/*
		 * Set index used and indexnum
		 */
		configuration.reset();
		index.reset();
		indexnum = std::vector<int>(problem->nindex, 0);
		memoccupation = 0;
		fixedcosts = 0;
		for (auto const &it : conf_used) {
			if (configuration[it] == 0) {
				configuration[it] = 1;
				index |= problem->e[it];
				for (int ind = 0; ind < problem->nindex; ind++) {
					if (problem->e[it][ind] == 1) {
						indexnum[ind]++;
						if (indexnum[ind] == 1) {
							fixedcosts += problem->f[ind];
							memoccupation += problem->m[ind];
						}
					}
				}
			}
		}

		objf = totalgain - fixedcosts;
		//cout<<"update: "<<objf<<" "<<problem->M<<" "<<memoccupation<<endl;

		if (memoccupation > problem->M) {
			feasible = false;

			for (int ind = 0; ind < problem->nindex; ind++) {
				if (index[ind] == 1) {
					for (auto const &it : conf_used) {
						if (configuration[it] == 1 && problem->e[it][ind] == 1) {
							conf_mem[it] += (((float)problem->m[ind]) / indexnum[ind]);
							conf_fcost[it] += (((float)problem->f[ind]) / indexnum[ind]);
						}
					}
				}
			}

			float worstratio = std::numeric_limits<float>::max();
			int   worstc = 0;
			for (auto const &it : conf_used) {
				float ratio = ((conf_gain[it] - conf_fcost[it]) * memoccupation) / (conf_mem[it] * objf);
				if (ratio < worstratio) {
					worstratio = ratio;
					worstc = it;
				}
			}
			configuration[worstc] = 0;


		}
		else {
			feasible = true;
		}
		//cout<<objf<<" "<<problem->M<<" "<<memoccupation<<" nf"<<endl;
	}
}

void Member::improve() {
	//Improvement phase
	bool stop = false;
	while (!stop) {
		float bestimpr = 0;
		int	bestconf = 0;
		int bestmem = 0;
		int bestfcost = 0;
		int bestgain = 0;
		boost::dynamic_bitset<> bestindex;
		std::set<int> bestquery;


		bool found = false;
		for (int c = 1; c < configuration.size(); c++) {
			if (configuration[c] == 0) {
				// index: nmem and nfixed
				int nmem = 0;
				int nfixed = 0;
				int ngain = 0;
				std::set<int> tmpquery;
				boost::dynamic_bitset<> tmpindex = (index | problem->e[c]) ^ index;
				for (int i = 0; i < tmpindex.size(); i++) {
					if (tmpindex[i] == 1 and index[i] == 0) {
						nmem += problem->m[i];
						nfixed += problem->f[i];
					}
				}

				if (memoccupation + nmem <= problem->M) {
					for (int q = 0; q < query.size(); q++) {
						if (problem->g[c][q] > problem->g[query[q]][q]) {
							ngain += problem->g[c][q];
							tmpquery.insert(q);
						}
					}

					float impr = (ngain - nfixed);

					if (nmem != 0) { float impr = (ngain - nfixed) / nmem; }


					if (impr > bestimpr) {
						bestconf = c;
						bestimpr = (float)(impr);
						bestmem = nmem;
						bestfcost = nfixed;
						bestgain = ngain;
						bestindex = boost::dynamic_bitset<>(tmpindex);
						bestquery = std::set<int>(tmpquery);
						found = true;
					}
				}
				tmpquery.clear();
			}
		}
		if (found == true) {
			for (auto const& q : bestquery) {
				objf -= problem->g[query[q]][q];
				query[q] = bestconf;

			}
			configuration[bestconf] = 1;
			memoccupation += bestmem;
			fixedcosts += bestfcost;
			index |= bestindex;
			totalgain = 0;
			for (int q = 0; q < query.size(); q++) {
				totalgain += problem->g[query[q]][q];
			}
			objf = totalgain - fixedcosts;
			stop = false;

			int sum = 0;
			for (int i = 0; i < index.size(); i++) {
				if (index[i])sum += problem->m[i];
			}


		}
		else {
			stop = true;
		}
	}
}



