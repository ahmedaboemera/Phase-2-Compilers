/*
 * Parser.cpp
 *
 *  Created on: 17 Apr 2015
 *      Author: ahmedaboemera
 */
#include "Parser.h"

Parser::Parser(Initiator* i, Utilities* u) {
	this->i = i;
	this->u = u;
}

void Parser::print_predictive_table() {
	for (map<string, map<string, vector<string> > >::iterator it =
			predictive_table.begin(); it != predictive_table.end(); it++) {
		cout << it->first << ":" << endl;
		for (map<string, vector<string> >::iterator it2 = it->second.begin();
				it2 != it->second.end(); it2++) {
			cout << "\t" << it2->first << ":" << endl << "\t\t";
			for (vector<string>::iterator it3 = it2->second.begin();
					it3 != it2->second.end(); it3++) {
				cout << *it3 << " ";
			}
			cout << endl;
		}
	}
}

string Parser::remove_uni_quotes(string s) {
	string tempo = "";
	if ((int) s[0] < 0) {
		for (int i = 1; i < strlen(&s[0]) - 1; i++) {
			if ((int) s[i] > 0)
				tempo += s[i];
		}
	}else{
		tempo = s;
	}
	return tempo;
}

void Parser::built_predictive_table() {
	int non_terminal_index = 0;
	for (; non_terminal_index < i->non_terminals->size();
			non_terminal_index++) {
		string non_term_name = i->non_terminals->at(non_terminal_index);
		vector<set<string> > firsts = u->get_first(non_term_name);
		set<string> follow = u->get_follow(non_term_name);

		map<string, vector<string> > entry; // each entry in predictive table corresponds to a non-terminal symbol
		vector<vector<string> > non_term_def = i->get_def(non_term_name); // definition
		int production = 0;
		for (; production != non_term_def.size(); production++) {
			set<string> set_of_firsts = firsts.at(production);
			for (set<string>::iterator it = set_of_firsts.begin();
					it != set_of_firsts.end(); it++) {
				if (it->compare("\\L") == 0) {
					for (set<string>::iterator it2 = follow.begin();
							it2 != follow.end(); it2++) {
						if (entry.find(*it2) != entry.end()) {
							cerr << "Grammar is not LL(1): abort1" << endl;
							throw new exception();
						}
						entry.insert(pair<string, vector<string> >(remove_uni_quotes(*it2),non_term_def.at(production)));
					}
				} else {
					if (entry.find(*it) != entry.end()) {
						cerr << "Grammar is not LL(1): abort2" << endl;
						throw new exception();
					}
					entry.insert(pair<string, vector<string> >(remove_uni_quotes(*it),non_term_def.at(production)));
				}
			}
		}
		for (set<string>::iterator it2 = follow.begin();
			it2 != follow.end(); it2++) {
			if (entry.find(*it2) == entry.end()) {
				vector<string> synch;
				synch.push_back("synch");
				entry.insert(pair<string, vector<string> >(remove_uni_quotes(*it2),synch));
			}
		}
		predictive_table.insert(pair<string, map<string, vector<string> > >(non_term_name,entry));

	}
}

vector<string> Parser::get_entry(string non_terminal, string terminal) {
	if (predictive_table.find(non_terminal) == predictive_table.end())
		throw 1; // non-terminal doesn't exist in map
	map<string, vector<string> > entry = predictive_table.find(non_terminal)->second;
	if (entry.find(terminal) == entry.end())
		throw 2; // loc(non-terminal,terminal) doesn't exist in map
	if (entry.find(terminal)->second.at(0).compare("synch") == 0)
		throw 3;
	return entry.find(terminal)->second;
}

