
#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <algorithm>
#include <vector>
#include <map>
#include <set>

#include "Initiator.h"

using namespace std;

class Initiator;

class Utilities{
	public:
		Utilities(Initiator* i);
		map<string, vector<string> > compute_first();
		void compute_follow();
		vector<set<string> > get_first(string non_terminal);
		set<string> get_follow(string non_terminal);
		void print_set(set<string> firsts);
		void print_first();
		void print_follow();

	private:
		Initiator *initiator;
		const string EPSLON = "\\L";
		map<string, vector<set<string> > > non_terminals_first;
		map<string, set<string> > non_terminals_follow;
		set<string> compute_follow_rule1(string non_terminal);
		void compute_follow_rule2(string non_terminal);
		vector<string>::iterator vector_find(vector<string>* vec, string str);
		set<string> union_sets(vector<set<string> > vec);
};


#endif UTILITIES_H_
