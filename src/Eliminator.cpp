/*
 * Eliminator.cpp
 *
 *  Created on: 12 Apr 2015
 *      Author: ahmedaboemera
 */
#include "Eliminator.h"

Eliminator::Eliminator(Initiator* i) {
	initiator = i;
}
int Eliminator::find_index(string s) {
	for (int i = 0; i < initiator->non_terminals->size(); i++) {
		if (s.compare(initiator->non_terminals->at(i)) == 0) {
			return i;
		}
	}
	return (1 << 31 - 1);
}

vector<vector<string> > Eliminator::replace(int i) {

	vector<vector<string> > updated;

	for (int j = 0; j < (initiator->non_terminal_defs->at(i)).size(); j++) {
		if ((initiator->non_terminal_defs->at(i)).at(j).at(0).compare(
				initiator->non_terminals->at(i)) != 0) {
			int index = find_index(
					(initiator->non_terminal_defs->at(i)).at(j).at(0));
			if (index < i) {
				for (int k = 0;
						k < (initiator->non_terminal_defs->at(index)).size();
						k++) {
					vector<string> vec2;
//					vec.push_back(initiator->non_terminal_defs.at(index).at(k));
					for (int z = 0;
							z
									< (initiator->non_terminal_defs->at(index)).at(
											k).size(); z++) {
						vec2.push_back(
								(initiator->non_terminal_defs->at(index)).at(k).at(
										z));
					}

					for (int z = 0;
							z < (initiator->non_terminal_defs->at(i)).at(j).size();
							z++) {
						vec2.push_back(
								(initiator->non_terminal_defs->at(i)).at(j).at(z));
					}
					updated.push_back(vec2);
				}

			} else {
				updated.push_back((initiator->non_terminal_defs->at(i)).at(j));
			}
		} else {
			updated.push_back((initiator->non_terminal_defs->at(i)).at(j));
		}
	}

	return updated;
}

bool Eliminator::probe_for_lf(int index) {
	for (int i = 0; i < initiator->non_terminal_defs->at(index).size(); i++) {
		string one = initiator->non_terminal_defs->at(index).at(i).at(0);
		for (int j = i + 1; j < initiator->non_terminal_defs->at(index).size();
				j++) {
			string two = initiator->non_terminal_defs->at(index).at(j).at(0);
			if (one.compare(two) == 0) {
				return true;
			}
		}
	}
	return false;
}

bool Eliminator::find(vector<string>* taken, string s) {
	for (int i = 0; i < taken->size(); i++) {
		if (s.compare(taken->at(i)) == 0) {
			return true;
		}
	}
	return false;
}

vector<vector<int> > Eliminator::get_left_factors(int index) {
	vector<string>* taken = new vector<string>();
	vector<vector<int> > lfactors;
	for (int i = 0; i < initiator->non_terminal_defs->at(index).size(); i++) {
		bool found = find(taken,
				initiator->non_terminal_defs->at(index).at(i).at(0));
		if (!found) {

			string factor = initiator->non_terminal_defs->at(index).at(i).at(0);
			taken->push_back(factor);

			vector<int> factors;
			factors.push_back(i);

			for (int j = i + 1; j < initiator->non_terminal_defs->at(index).size();
					j++) {
				string two = initiator->non_terminal_defs->at(index).at(j).at(0);
				if (factor.compare(two) == 0) {
					factors.push_back(j);
				}
			}

			lfactors.push_back(factors);
		}
	}
	free(taken);
	return lfactors;
}

void Eliminator::eliminate_LF(int index) {
	vector<vector<int> > lfactors = get_left_factors(index);

	vector<vector<string> > new_forS;
	string newS = initiator->non_terminals->at(index);

	for (int i = 0; i < lfactors.size(); i++) {
		if (lfactors.at(i).size() == 1) {
			vector<string> old_and_new;
			int curr = lfactors.at(i).at(0);
			for (int j = 0;
					j < initiator->non_terminal_defs->at(index).at(curr).size();
					j++) {
				old_and_new.push_back(
						initiator->non_terminal_defs->at(index).at(curr).at(j));
			}
			new_forS.push_back(old_and_new);
		} else {
			//add new state
			newS += "*";
			vector<string> new_new;
			int fac = lfactors.at(i).at(0);
			new_new.push_back(
					initiator->non_terminal_defs->at(index).at(fac).at(0));
			new_new.push_back(newS);
			new_forS.push_back(new_new);
		}
	}

	newS = initiator->non_terminals->at(index);

	for (int i = 0; i < lfactors.size(); i++) {
		if (lfactors.at(i).size() > 1) {
			vector<vector<string> > new_for_newS;
			newS += "*";

			//add as new non_terminal
			initiator->non_terminals->push_back(newS);

			//added

			//add def
			for(int j = 0; j < lfactors.at(i).size(); j++){
				vector<string> vec;

				int size = initiator->non_terminal_defs->at(index).at(lfactors.at(i).at(j)).size();

				if(size == 1){
					vec.push_back("\\L");
					new_for_newS.push_back(vec);
				}else{
					for(int k = 1; k < size ; k++){
						vec.push_back(initiator->non_terminal_defs->at(index).at(lfactors.at(i).at(j)).at(k));
					}
					new_for_newS.push_back(vec);
				}
			}
			initiator->non_terminal_defs->push_back(new_for_newS);
			//added
		}
	}

	initiator->non_terminal_defs->at(index) = new_forS;

}
void Eliminator::eliminate_LF() {
	print_grammer();
	for (int i = 0; i < initiator->non_terminals->size(); i++) {
		bool check = probe_for_lf(i);
		if (check) {
			eliminate_LF(i);
		}
	}
	cout << "========================================================" << endl;
	print_grammer();
}

void Eliminator::eliminate_from_updated(vector<vector<string> > replaced,
		int index) {

	string non_terminal_name = initiator->non_terminals->at(index);
	vector<int> lr_indecies;
	vector<int> n_lr_indecies;
	for (int i = 0; i < (initiator->non_terminal_defs->at(index)).size(); i++) {
		if (non_terminal_name.compare(
				(initiator->non_terminal_defs->at(index)).at(i).at(0)) == 0) {
			lr_indecies.push_back(i);
		} else {
			n_lr_indecies.push_back(i);
		}
	}
	if (lr_indecies.size() == 0) {
		return;
	} else {
		string new_non_terminal = non_terminal_name + "\'";
		initiator->non_terminals->push_back(new_non_terminal);
		vector<vector<string> > updated;

		for (int i = 0; i < n_lr_indecies.size(); i++) {
			vector<string> vec = (initiator->non_terminal_defs->at(index)).at(
					n_lr_indecies.at(i));
			vec.push_back(new_non_terminal);
			updated.push_back(vec);
		}

		vector<vector<string> > new_update;

		vector<string> epsilon;
		epsilon.push_back("\\L");
		new_update.push_back(epsilon);

		for (int i = 0; i < lr_indecies.size(); i++) {
			vector<string> vec;
			for (int k = 1;
					k
							< (initiator->non_terminal_defs->at(index)).at(
									lr_indecies.at(i)).size(); k++) {
				vec.push_back(
						(initiator->non_terminal_defs->at(index)).at(
								lr_indecies.at(i)).at(k));
			}
			vec.push_back(new_non_terminal);
			new_update.push_back(vec);
		}

		initiator->non_terminal_defs->push_back(new_update);
		initiator->non_terminal_defs->at(index) = updated;
	}
}

void Eliminator::print_grammer() {
	for (int i = 0; i < initiator->non_terminals->size(); i++) {
		cout << initiator->non_terminals->at(i) << " --> ";
		for (int j = 0; j < initiator->non_terminal_defs->at(i).size(); j++) {
			for (int l = 0; l < initiator->non_terminal_defs->at(i).at(j).size();
					l++) {
				cout << initiator->non_terminal_defs->at(i).at(j).at(l) << " ";
			}
			cout << " | ";
		}
		cout << endl;
	}
}

void Eliminator::eliminate_LR() {

	print_grammer();

	for (int i = 0; i < initiator->non_terminals->size(); i++) {
		vector<vector<string> > updated = replace(i);
		eliminate_from_updated(updated, i);
	}

	cout << "=========================================================="
			<< endl;
	print_grammer();
}

