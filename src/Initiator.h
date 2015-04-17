/*
 * Parser.h
 *
 *  Created on: 12 Apr 2015
 *      Author: ahmedaboemera
 */

#ifndef INITIATOR_H_
#define INITIATOR_H_

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>
#include "Utilities.h"

using namespace std;

class Utilities;

class Initiator {
public:
	Initiator();
	string starting;
	vector<string>* terminals;
	vector<string>* non_terminals;
	vector<vector<vector<string> > >* non_terminal_defs;
	void get_terminals_and_nonterminals(vector<string>* lines);
	bool is_terminal(string s);
	bool is_non_terminal(string s);
	string get_starting();
//	map<string,map<string, vector<string> > > built_predictive_table();
	vector<vector<string> > get_def(string s);
	string trim(string x);
private:
//	Utilities util;
	void fill_map(vector<string>* lines);
	void finalize(vector<vector<string> > non_t_defs);
	vector<string> split(string s, int start, char regex);
	int get_eq(string s);
	vector<string>* get_terminals(string s);
	string get_nonterminal(string s);
};
#endif /* INITIATOR_H_ */
