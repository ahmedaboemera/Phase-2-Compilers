/*
 * Parser.h
 *
 *  Created on: 17 Apr 2015
 *      Author: ahmedaboemera
 */

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include "Utilities.h"
#include "Initiator.h"

class Parser{
public:
	Parser(Initiator* i, Utilities* u);
	void built_predictive_table();
	vector<string> get_entry(string non_terminal, string terminal);
	void print_predictive_table();
	string remove_uni_quotes(string s);
private:
	map<string,map<string, vector<string> > > predictive_table;
	Initiator* i;
	Utilities* u;
};


#endif /* SRC_PARSER_H_ */
