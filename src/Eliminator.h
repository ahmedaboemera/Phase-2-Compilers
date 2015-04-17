/*
 * Eliminator.h
 *
 *  Created on: 13 Apr 2015
 *      Author: ahmedaboemera
 */

#ifndef ELIMINATOR_H_
#define ELIMINATOR_H_

#include "Initiator.h"
using namespace std;

class Initiator;

class Eliminator{
public:
	Initiator* initiator;
	Eliminator(Initiator* i);
	void eliminate_LF() ;
	void eliminate_LR();
	void print_grammer();
private:
	bool find (vector<string>* taken, string s);
	vector<vector<int> > get_left_factors(int index);
	void eliminate_LF(int index) ;
	bool probe_for_lf(int index);
	int find_index(string s);
	vector<vector<string > > replace(int i);
	void eliminate_from_updated(vector<vector<string> > replaced, int index);
};



#endif /* ELIMINATOR_H_ */
