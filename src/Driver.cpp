/*
 * Driver.cpp
 *
 *  Created on: 12 Apr 2015
 *      Author: ahmedaboemera
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stack>

#include "Eliminator.h"
#include "Initiator.h"
#include "Parser.h"

using namespace std;

vector<string>* modify_grammar_file(const char* file_path) {
	vector<string>* to_return = new vector<string>();
	ifstream in;
	in.open(file_path);
	string to_push = "";
	while (!in.eof()) {
		string line;
		getline(in, line);
//		cout<<line<<endl;
		if (line[0] == '#') {
			// concatenate current line with to push
			if (to_push.compare("") != 0) {
				to_return->push_back(to_push);
			}
			to_push = line;
		} else {
			to_push = to_push + line;
		}
	}
	to_return->push_back(to_push);
	return to_return;
}

string concatenate_vector(vector<string> prod, Initiator* i) {
	string str_prod;
	for (vector<string>::iterator it = prod.begin(); it != prod.end(); it++) {

		str_prod = str_prod + " " + *it;
	}
	str_prod = i->trim(str_prod);
	return str_prod;
}

vector<string> stack_top_extractor(int depth, stack<string>* s) {
	vector<string> extracted_tops;
	int i = 0;
	for (; i < depth; i++) {
		extracted_tops.push_back(s->top());
		s->pop();
	}
	for(i = extracted_tops.size()-1; i >= 0; i--)
		s->push(extracted_tops.at(i));
	return extracted_tops;
}

/*
 * Final code should be using a lexical analyzer instance that supplies
 * the driver with tokens by a function "get_next_token()"
 *
 * The lexical analyzer will simply receive the text file containing
 * the source code required to be parsed, and using a serialized DFA
 * header file it's going to extract tokens out of this source code and
 * supply the driver with them
 *
 * Meanwhile, we're going to hard code the tokens in a text file "tokens.txt"
 * and the driver will simulate the lexical analyzer's "get_next_token"
 * by simply reading one line at a time from the text file
 * */
int main() {
	vector<string>* modified = modify_grammar_file("src/CFG.txt");
	Initiator i;
	i.get_terminals_and_nonterminals(modified);
	Eliminator e(&i);
	e.eliminate_LR();
	e.eliminate_LF();

	Utilities u(&i);
	Parser p(&i, &u);
	p.built_predictive_table();

	/* change this part with function calls from the lexical analyzer */

	ifstream in;
	in.open("src/tokens.txt"); // w
	ofstream out;
	out.open("output.txt");
	vector<string> input_buffer; // contains everything in tokens.txt
	int stack_depth = 1; // tracks how many elements should be printed from the
						 // stack top
	vector<string> matched_tokens; // tracks all the tokens that has been already
								  // popped from the stacked to be printed
	stack<string> s;
	s.push("$");
	s.push(i.get_starting());
	out << p.remove_uni_quotes(s.top()) << endl;
	while (!in.eof()) {
		string s;
		getline(in, s);
		input_buffer.push_back(s);
	}
	in.close();

	int ip = 0; // next token to be read
	string top = p.remove_uni_quotes(s.top()); // X
	while (top.compare("$") != 0) { // $ is not on the top of the stack; i.e. stack is not empty
		if (ip == input_buffer.size()) {
			break;
		} else if (top.compare(input_buffer.at(ip)) == 0) {
			cout << "matching " << input_buffer.at(ip) << endl;
			matched_tokens.push_back(s.top());
			s.pop();
			stack_depth--;
			ip++;
		} else if (i.is_terminal(top)) {
			cerr << "Panic mode: " << top << "does not match any tokens" << endl;
			cerr << "popping terminal and continuing..." << endl;
			s.pop();
			stack_depth--;
			ip++;
		} else {
			try {
				vector<string> prod = p.get_entry(top, input_buffer.at(ip));

				string str_prod = concatenate_vector(prod, &i);
				for (vector<string>::iterator it = matched_tokens.begin();
					it != matched_tokens.end(); it++) {
					out << *it << " ";
				}
				s.pop();
				stack_depth--;
				int i = prod.size() - 1;
				for (; i >= 0; i--) {
					if (prod.at(i).compare("\\L") != 0) {
						s.push(prod.at(i));
						stack_depth++;
					}
				}
				vector<string> stack_elems = stack_top_extractor(stack_depth, &s);
				for (vector<string>::iterator it = stack_elems.begin();
						it != stack_elems.end(); it++) {
					out << *it << " ";
				}
				out << "\n";

			} catch (int i) {
				if (i == 3) {
					cerr << "Panic mode: synch was encountered" << endl;
					cerr << "recovering..." << endl;
					s.pop();
					stack_depth--;
					ip++;
				}
				else {
					cerr << "Panic mode: entry is empty, cannot recover from error. Abort." << endl;
					return 0;
				}
			}
		}
		top = p.remove_uni_quotes(s.top());
	}
	if (p.remove_uni_quotes(s.top()).compare("$") != 0) {
		cerr << "Error: Could not match expression" << endl;
	}
	out.close();
	cout << "..done" << endl;
}
/* ************************************************************** */
