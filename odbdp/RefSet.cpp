//
// Created by Riccardo Mereu on 15/12/2018.
// Maintained by Raslan Kain since 01/09/2019.
//
#include "RefSet.h"
#define TRIAL_SOL 3

#include <iostream>
using namespace std;



RefSet::RefSet(int sizeR, ODBP* inst){
	sizeRef = sizeR;
	problem = inst;
	newsolution = false;
	current_iter = 0;
}

void RefSet::create(Population pop1){
	this->pop = &pop1;
	current_iter++;

	//Best solutions
	std::set<Member>::reverse_iterator rit = pop->divSet.rbegin();
	while(referenceSet.size() < sizeRef/2){
		referenceSet.insert(*rit);
		rit++;
	}

	//Most diverse solutions
	while(referenceSet.size() < sizeRef){
		float bestdistance = std::numeric_limits<float>::min();
		std::set<Member>::iterator cand = pop->divSet.begin();
		Member candidate = *cand;

		for(auto const& itP: pop->divSet){
			float distance = 0.0;
			for(auto const& itR: referenceSet){
				boost::dynamic_bitset<> tmp = itP.configuration ^ itR.configuration;
				distance += tmp.count();
			}
			distance/=referenceSet.size();
			if(distance > bestdistance){
				bestdistance = distance;
				candidate = itP;
			}
		}
		pop->divSet.erase(candidate);
		candidate.iteration = current_iter;
		referenceSet.insert(candidate);
	}

	newsolution=true;
}

void RefSet::update(int iter) {
	current_iter = iter;
	newsolution = false;
	combineRefSet();
	cout<<pool1.size()<<endl;
	std::cout<<"New solutions:\n";
	for(auto const& it: pool1){
		Member candidate(it, problem);
		std::set<Member>::iterator it1=referenceSet.begin();
		if(candidate.objf > (*it1).objf){
			referenceSet.erase(it1);
			candidate.iteration = current_iter+1;
			if(!(referenceSet.find(candidate) != referenceSet.end())) {
				newsolution = true;
				referenceSet.insert(candidate);
				cout<<candidate.objf<<endl;
			}
		}

		//tries to add the opposite of the seed
		Member ncandidate(~it, problem);
		it1=referenceSet.begin();
		if(ncandidate.objf > (*it1).objf){
			referenceSet.erase(it1);
			ncandidate.iteration = current_iter+1;
			if(!(referenceSet.find(ncandidate) != referenceSet.end())) {
				newsolution = true;
				referenceSet.insert(ncandidate);
				cout<<candidate.objf<<endl;
			}
		}

	}
	pool1.clear();
	//cout<<pool1.size()<<endl;
}

void RefSet::combineRefSet() {
	cout<<"current_iter="<<current_iter<<endl;
	std::set<Member>::reverse_iterator it1;
	std::set<Member>::reverse_iterator it2;
	for(it1 = referenceSet.rbegin(); it1 != referenceSet.rend(); it1++){
		it2=it1; it2++;
		for(; it2!=referenceSet.rend(); it2++){
			if((*it1).iteration == current_iter or (*it2).iteration == current_iter){
				//cout<<(*it1).objf<<" "<<(*it2).objf<<" "<<(*it1).iteration<<" "<<(*it2).iteration<<endl;
				std::vector<boost::dynamic_bitset<>> trialsol = combine((*it1), (*it2));
				for(auto const& it3: trialsol){
					//Member candidate(it3, problem);
					pool1.insert(it3);
				}
			}
		}
	}
}

std::vector<boost::dynamic_bitset<>> RefSet::combine(Member m1, Member m2){
	//for(int i=0; i<m1.configuration.size(); i++)cout<<m1.configuration[i];cout<<endl<<endl;
	//for(int i=0; i<m2.configuration.size(); i++)cout<<m2.configuration[i];cout<<endl;

	std::default_random_engine eng{static_cast<unsigned int>(time(0))};
	std::uniform_real_distribution<double> urd(0.0, 1.0);

	std::vector<boost::dynamic_bitset<>> trialsol(TRIAL_SOL);
	std::vector<float> random_threshold(TRIAL_SOL);

	for(int i = 0; i<trialsol.size(); i++){
		random_threshold[i]=urd(eng);
		trialsol[i]=boost::dynamic_bitset<>(problem->nconfig+1);
		for(int j=1; j<trialsol[i].size(); j++){
			double score = (double)(m1.objf * m1.configuration[j] + m2.objf * m2.configuration[j])/(double)(m1.objf + m2.objf);
			//std::cout<<score<<" "<<random_threshold[i]<<std::endl;
			
			// implementation 1
			if(score > random_threshold[i])
				trialsol[i][j]=1;
			
			//implementation 2
			/*
			float rand = urd(eng);
			if(score > rand)
				trialsol[i][j]=1;
			 */

			//implementation 3
			/*
			if(score > 0.5)
				trialsol[i][j]=1;
			*/
		}
	}

	return trialsol;
}

bool RefSet::isnewsolution() {
	return newsolution;
}

void RefSet::rebuild(int iter, int size) {
	
	static int count = 0;
	static boost::dynamic_bitset<> last_seed(problem->conf_best);
	cout << size << endl;
	Population pop1(size, problem);
	if(last_seed==problem->conf_best){
		if (count < 3) {
			cout << "rebuild with best " << count << endl;
			pop1.create(problem->conf_best);
			count++;
		}
		else if(count < 6){
			cout << "rebuild random " << endl;
			count++;
			pop1.create(pop1.random_seed(problem->conf_best));
		}else {
			count = 0;
			pop1.create();
		}
	}
	else{
		last_seed = problem->conf_best;
		pop1.create(problem->conf_best);
		count = 0;
		cout << "rebuild with best" << count << endl;
	}
	
	pop = &pop1;
	std::set<Member>::iterator it = referenceSet.begin();

	while(referenceSet.size() > sizeRef/2){
		referenceSet.erase(it);
		it =  referenceSet.begin();
	}

	while(referenceSet.size()<sizeRef){
		Member candidate;
		float bestdistance=std::numeric_limits<float>::min();
		for(auto const& itP: pop->divSet){
			float distance = 0;
			for(auto const& itR: referenceSet){
				boost::dynamic_bitset<> tmp = itP.configuration ^ itR.configuration;
				distance+=tmp.count();
			}
			distance/=referenceSet.size();

			if(distance>bestdistance){
				bestdistance=distance;
				candidate = itP;
			}
		}
		pop->divSet.erase(candidate);
		candidate.iteration = iter+1;
		referenceSet.insert(candidate);
		newsolution=true;
	}
	//cout<<"1 refset size"<<referenceSet.size()<<endl;
}