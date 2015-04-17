#include "Initiator.h"

Initiator::Initiator() {
	non_terminals = new vector<string>();
	terminals = new vector<string>();
	non_terminal_defs = new vector<vector<vector<string> > >();
}
string Initiator::trim(string x) {
	if (x.compare("") == 0)
		return x;

	unsigned int i;
	for (i = 0; i < x.length() && x.at(i) == ' '; i++)
		;

	if (i == x.length())
		return "";

	unsigned int j;
	for (j = x.length() - 1; j >= 0 && x.at(j) == ' '; j--)
		;

	string toRet = "";
	unsigned int k;
	for (k = i; k <= j; k++)
		toRet += x.at(k);

	return toRet;
}

string Initiator::get_nonterminal(string s) {
	string x;
	for (unsigned int i = 2; s[i] != ' '; i++) {
		x += s[i];
	}
	return x;
}

vector<string>* Initiator::get_terminals(string s) {
	vector<string>* x = new vector<string>();
	string to_add;
	bool add = 0;
	for (unsigned int i = 0; i < strlen(&s[0]); i++) {

		if (!add) {
			if ((int) s[i] == -104) {
				add = 1;
			}
		} else {
			if ((int) s[i] == -103 | (int) s[i] == -104) {
				add = false;
				if (trim(to_add).compare("") != 0)
					x->push_back(trim(to_add));
				to_add = "";
			} else {
				if (s[i] == -104) {
					to_add += '‘';
				} else {
					to_add += ((int) s[i] < 0 ? ' ' : s[i]);
				}
			}
		}
	}
	if (trim(to_add).compare("") != 0)
		x->push_back(trim(to_add));
	return x;
}

int Initiator::get_eq(string s) {
	int i = 0;
	for (; s[i] != '='; i++)
		;
	return i;
}

vector<string> Initiator::split(string s, int start, char regex) {
	int begin = start, end = start;
	vector<string> vec;

	for (int i = start; i < (strlen(&s[0])); i++) {
		if (s[i] == regex) {
			end = i;
			string x = s.substr(start, end - start);
			if (trim(x).compare("") != 0)
				vec.push_back(trim(x));
			start = end + 1;
			end = start;
		}
	}
	if (trim(s.substr(start, strlen(&s[0]) - start)).compare("") != 0)
		vec.push_back(s.substr(start, strlen(&s[0]) - start));

	return vec;
}

void Initiator::finalize(vector<vector<string> > non_t_defs) {
	for (int i = 0; i < non_t_defs.size(); i++) {
		vector<vector<string> > x;
		for (int j = 0; j < non_t_defs.at(i).size(); j++) {
			x.push_back(split(non_t_defs.at(i).at(j), 0, ' '));
		}
		Initiator::non_terminal_defs->push_back(x);
	}
}

void Initiator::fill_map(vector<string>* lines) {
	vector<vector<string> > non_t_defs;
	for (vector<string>::iterator it = lines->begin(); it != lines->end();
			it++) {
		int equal = get_eq(*it);
		vector<string> returned = split(*it, equal + 1, '|');
		non_t_defs.push_back(returned);
	}
	finalize(non_t_defs);
}

void Initiator::get_terminals_and_nonterminals(vector<string>* lines) {
	int i = -1;
	for (vector<string>::iterator it = lines->begin(); it != lines->end();
			it++) {
		i++;
		string x = get_nonterminal(*it);
		if (trim(x).compare("") != 0)
			non_terminals->push_back(trim(x));
		if (i == 0) {
			starting = x;
		}
		vector<string>* y = get_terminals(*it);
		for (vector<string>::iterator it = y->begin(); it != y->end(); it++) {
			if (trim((*it)).compare("") != 0)
				terminals->push_back(trim((*it)));
		}
	}

	fill_map(lines);
}

bool Initiator::is_terminal(string s) {
	string tempo = "";
	if ((int) s[0] < 0) {
		for (int i = 1; i < strlen(&s[0]) - 1; i++) {
			if ((int) s[i] > 0)
				tempo += s[i];
		}
	}else{
		tempo = s;
	}
	for (int i = 0; i < terminals->size(); i++) {
		if (tempo.compare(terminals->at(i)) == 0) {
			return true;
		}
	}
	return false;
}
bool Initiator::is_non_terminal(string s) {
	for (int i = 0; i < non_terminals->size(); i++) {
		if (s.compare(non_terminals->at(i)) == 0) {
			return true;
		}
	}
	return false;
}
string Initiator::get_starting() {
	return starting;
}
vector<vector<string> > Initiator::get_def(string s) {
	int index = -1;
	for (int i = 0; i < non_terminals->size(); i++) {
		if (s.compare(non_terminals->at(i)) == 0) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		throw new exception();
	}
	return non_terminal_defs->at(index);
}
