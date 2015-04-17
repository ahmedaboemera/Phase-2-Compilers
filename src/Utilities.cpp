
#include "Utilities.h"




Utilities::Utilities(Initiator* i){
	initiator= i;
}


set<string> Utilities::get_follow(string non_terminal){
	if(non_terminals_follow.size() == 0)
		compute_follow();
//	if(!initiator->is_non_terminal(non_terminal))
//		return NULL;
	return non_terminals_follow[non_terminal];
}


vector<set<string> > Utilities::get_first(string non_terminal){

	vector<set<string> > result;
	if(!initiator->is_non_terminal(non_terminal)){
		set<string> s;
		s.insert(non_terminal);
		result.push_back(s);
		return result;
	}
	if(non_terminals_first.find(non_terminal)!=non_terminals_first.end())
		return non_terminals_first[non_terminal];

	vector<vector<string> > sop = initiator->get_def(non_terminal);

	for(vector<vector<string> >::iterator mul_terms=sop.begin(); mul_terms!=sop.end(); mul_terms++){

		set<string> firsts;
		vector<string>::iterator term_it = mul_terms->begin();
		for(; term_it!=mul_terms->end(); term_it++){
			if(!initiator->is_non_terminal(*term_it) || term_it->compare(EPSLON) == 0){
				firsts.insert(*term_it);
				break;
			}else{
				vector<set<string> > term_first = get_first(*term_it);
				bool found=false;
				for(vector<set<string> >::iterator it_vs=term_first.begin(); it_vs!=term_first.end(); it_vs++){
					if(it_vs->find(EPSLON)!=it_vs->end()){
						term_first.erase(it_vs);
						found=true;
					}
					firsts.insert(it_vs->begin(), it_vs->end());
				}

				if(!found)
					break;
			}
		}
		if(term_it == mul_terms->end() && firsts.find(EPSLON)==firsts.end())
			firsts.insert(EPSLON);
		result.push_back(firsts);
	}
	non_terminals_first[non_terminal] = result;
	return result;
}



void Utilities::compute_follow(){
	non_terminals_follow[initiator->starting].insert("$");

	for(vector<string>::iterator non_terminal=initiator->non_terminals->begin();
			non_terminal!=initiator->non_terminals->end(); non_terminal++){
		set<string> s1 = compute_follow_rule1(*non_terminal);
		non_terminals_follow[*non_terminal].insert(s1.begin(), s1.end());
	}

	for(vector<string>::iterator non_terminal=initiator->non_terminals->begin();
			non_terminal!=initiator->non_terminals->end(); non_terminal++)
		compute_follow_rule2(*non_terminal);
}


set<string> Utilities::compute_follow_rule1(string non_terminal){

	set<string> followers;
	for(vector<string>::iterator non_terminal_it=initiator->non_terminals->begin();
				non_terminal_it!=initiator->non_terminals->end(); non_terminal_it++){

		if(non_terminal_it->compare(non_terminal) == 0)
			continue;

		vector<vector<string> > sop = initiator->get_def(*non_terminal_it);
		for(vector<vector<string> >::iterator mul_terms=sop.begin(); mul_terms!=sop.end(); mul_terms++){
			vector<string>::iterator it2 = find(mul_terms->begin(), mul_terms->end(), non_terminal);

			if(it2 != mul_terms->end()){
				vector<set<string> > temp;
				for(it2=it2+1; it2!=mul_terms->end(); it2++){
					temp = get_first(*it2);
					set<string> temp_set = union_sets(temp);
					followers.insert(temp_set.begin(), temp_set.end());
					if(temp_set.find(EPSLON) == temp_set.end())
						break;
				}
			}
		}
	}
	followers.erase(EPSLON);
	return followers;
}



void Utilities::compute_follow_rule2(string non_terminal){

	vector<vector<string> > sop = initiator->get_def(non_terminal);
	set<string> term_follow = get_follow(non_terminal);

	for(vector<vector<string> >::iterator mul_terms=sop.begin(); mul_terms!=sop.end(); mul_terms++){

		vector<string>::iterator term;
		for(term=mul_terms->end()-1; term>=mul_terms->begin(); term--){

			set<string> temp_set = union_sets(get_first(*term));

			non_terminals_follow[*term].insert(term_follow.begin(), term_follow.end());

			if(temp_set.find(EPSLON)==temp_set.end())
				break;
		}
	}
}



vector<string>::iterator Utilities::vector_find(vector<string>* vec, string str){
	vector<string>::iterator it = vec->begin();
	for(; it!=vec->end() && it->compare(str)!=0; it++);
	return it;
}


void Utilities::print_set(set<string> firsts){
	cout<<"----------------------------\n";
	for(set<string>::iterator s=firsts.begin(); s!=firsts.end(); s++)
		cout<<"\t"<<*s<<"\n";
	cout<<"============================\n";
	cout<<"\n";
}


set<string> Utilities::union_sets(vector<set<string> > vec){
	set<string> result;
	for(int i = 0; i<vec.size(); i++){
		result.insert(vec.at(i).begin(), vec.at(i).end());
	}
	return result;
}



void Utilities::print_first(){
	for(map<string, vector< set<string> > >::iterator it=non_terminals_first.begin(); it!=non_terminals_first.end(); it++){
		cout<<it->first<<endl;
		for(vector<set<string> >::iterator it2=it->second.begin(); it2!=it->second.end(); it2++)
			print_set(*it2);
		cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
	}
}


void Utilities::print_follow(){
	for(map<string, set<string> >::iterator it=non_terminals_follow.begin(); it!=non_terminals_follow.end(); it++){
		cout<<it->first<<endl;
		print_set(it->second);
	}
}
