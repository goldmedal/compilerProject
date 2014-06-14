#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
#include <map>
#include <set>
#include <vector>
#include <sstream>
//#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

ifstream G_ifile;

set<string> null_set;
map<string, string> null_map;
vector<string> tree;

multimap<string, string> grammar;
map<string, bool> nullable;
map<string, set<string> > first;
map<string, set<string> > follow;
multimap<string, map<string, string> > LLtable;

void initial()
{
	null_set.clear();
	null_map.clear();
}

string trimEnd(string str)
{
	string delim = " " ;

	if (str.at(str.length() - 1) != delim.at(0))
		return str.erase(0, str.find_first_not_of(delim));

	string r = str.erase(str.find_last_not_of(delim) + 1, str.length() - 1);
	return r.erase(0, r.find_first_not_of(delim));
}

bool is_nullable(string str)
{
	if (str == "epsilon")
		return true;
	else if ((str.at(0) < 65) || (str.at(0) > 90))
		return false;
	else 
	{
		map<string, bool>::iterator iter;

		iter = nullable.find(str);

		return iter -> second;
	}
}

void multimap_overview(multimap<string, string> G)
{
	for (multimap<string, string>::iterator iter = G.begin(); iter != G.end(); iter++)
	{
		cout << iter->first << endl;
		cout << " -> " ;
		cout << iter->second << endl;
	}
}

void map_overview(map<string, bool> G)
{
	for (map<string, bool>::iterator iter = G.begin(); iter != G.end(); iter++)
	{
		cout << iter->first << endl;
		cout << " -> " ;
		cout << iter->second << endl;
	}
}

void read_G()
{
	char temp = '0';
	string l_grammar, r_grammar;
	int i = 0, j = 0;
	//G_ifile >> temp;

	while(G_ifile.get(temp))
	{
		//G_ifile.get(temp);
		G_ifile.seekg(-1, ios::cur);

		if(temp != ' ')
		{
			//G_ifile >> temp;
			getline(G_ifile, l_grammar);
			//cout << trimEnd(l_grammar) << endl;

			l_grammar = l_grammar.erase(l_grammar.length() - 1);

			nullable.insert(pair<string, bool>(trimEnd(l_grammar), false));
			first.insert(pair<string, set<string> >(trimEnd(l_grammar), null_set)); 
			follow.insert(pair<string, set<string> >(trimEnd(l_grammar), null_set)); 
			LLtable.insert(pair<string, map<string, string> >(trimEnd(l_grammar), null_map));
		}
		else
		{
			getline(G_ifile, r_grammar);
			r_grammar = r_grammar.erase(r_grammar.length() - 1);

			grammar.insert(pair<string, string>(trimEnd(l_grammar), trimEnd(r_grammar)));
			//cout << trimEnd(l_grammar) << "123" << endl;

			//cout << trimEnd(l_grammar).length() <<endl;
		}
		//G_ifile.seekg(1, ios::cur);
	}

	//map_overview(grammar);

}

bool is_produce_epsilon(string str)
{
	multimap<string, string>::iterator Mbeg;

	Mbeg = grammar.lower_bound(str);

	while(Mbeg -> first == str)
	{
		if(Mbeg -> second == "epsilon")
			return true;

		if (++Mbeg == grammar.end())
			break;
	}

	return false;
}

void find_nullable()
{
	map<string, bool>::iterator iter, beg, end, find;
	multimap<string, string>::iterator Mbeg, Mend;
	string str, token;

	for (iter = nullable.begin(); iter != nullable.end(); iter++)  
	{
			//cout << iter->first << endl; 
			//cout << "  " << iter->second << endl;
		str = iter -> first;
		Mbeg = grammar.lower_bound(str);

		while(Mbeg -> first == str)
		{//cout << (Mbeg -> second).length() << endl;
			if(Mbeg -> second == "epsilon")
			{
				find = nullable.find(str);
				find -> second = true;
			}

			if (++Mbeg == grammar.end())
				break;

			//Mbeg++;
		}
	}
	
	bool check = true, r_res = true;

	while(check)
	{
		check = false;

		for (iter = nullable.begin(); iter != nullable.end(); iter++) 
		{ 
			//cout << iter->first << " " << iter->second << endl;
			str = iter -> first;
			Mbeg = grammar.lower_bound(str);

			while(Mbeg -> first == str)
			{
				istringstream iss(Mbeg -> second);
				string token;

				r_res = true;
				while(getline(iss, token, ' '))
					r_res &= is_nullable(token);

				if(r_res)
					break;

				if (++Mbeg == grammar.end())
					break;

				//Mbeg++;
			}

			find = nullable.find(str);
			
			if (find -> second != r_res) // continue
			{
				check = true;
				find -> second = r_res;
			}
		}
	}
}

set<string> get_right_first(string str)
{
	set<string> res;
	map<string, set<string> >::iterator iter;

	istringstream iss(str);
	string token;

	while(getline(iss, token, ' '))
	{
		if (token == "epsilon")
			break;
		else if ((token.at(0) < 65) || (token.at(0) > 90))
			res.insert(token);
		else
		{
			iter = first.find(token);
			res.insert((iter -> second).begin(), (iter -> second).end());

			if(nullable.find(token) -> second)
				continue;
		}

		break;
	}

	return res;
}

set<string> get_first(string str)
{
	set<string> res;
	multimap<string, string>::iterator Mbeg, Mend;
	map<string, set<string> >::iterator iter;

	Mbeg = grammar.lower_bound(str);

	while(Mbeg -> first == str)
	{
		istringstream iss(Mbeg -> second);
		string token;
		bool temp = true;

		while(getline(iss, token, ' '))
		{
			if (token == "epsilon")
				break;
			else if ((token.at(0) < 65) || (token.at(0) > 90))
				res.insert(token);
			else
			{
				iter = first.find(token);
				res.insert((iter -> second).begin(), (iter -> second).end());

				if(nullable.find(token) -> second)
					continue;
			}

			break;
		}

		if (++Mbeg == grammar.end())
			break;
				//Mbeg++;
	}
	
	return res;
}

void find_first()
{
	bool check = true;
	set<string> get_res;
	map<string, set<string> >::iterator iter;
	string str;

	while(check)
	{
		check = false;

		for (iter = first.begin(); iter != first.end(); iter++) 
		{ 
			//cout << iter->first << " " << iter->second << endl;
			str = iter -> first;

			get_res = get_first(str);
		
			if (get_res != iter -> second) // continue
			{	
				(iter -> second).insert(get_res.begin(), get_res.end());				
				check = true;
			}	
		}
	}
}

set<string> get_follow(string str)
{	
	set<string> res;
	multimap<string, string>::iterator Mbeg, Mend, Miter;
	map<string, set<string> >::iterator iter;

	for(Miter = grammar.begin(); Miter != grammar.end(); Miter++)
	{
		istringstream iss(Miter -> second);
		string token;
		bool next = false, is_tail = false;
		
		while(getline(iss, token, ' '))
		{ 
			if(next)
			{
				if((token.at(0) < 65) || (token.at(0) > 90))
					res.insert(token);
				else
				{
					iter = first.find(token);
					res.insert((iter -> second).begin(), (iter -> second).end());
				}
				
				if (nullable.find(token) -> second)
					next = true;
				else
					next = false;
			}

			if (token == str)
				next = true;
		}

		if (next) // at the tail
		{
			iter = follow.find(Miter -> first);
			res.insert((iter -> second).begin(), (iter -> second).end());
		}
	}
	return res;
}

void find_follow()
{
	bool check = true;
	set<string> get_res;
	map<string, set<string> >::iterator iter;
	string str;

	while(check)
	{
		check = false;

		for (iter = follow.begin(); iter != follow.end(); iter++) 
		{ 
			//cout << iter->first << " " << iter->second << endl;
			str = iter -> first;

			get_res = get_follow(str);
		
			if (get_res != iter -> second) // continue
			{	
				(iter -> second).insert(get_res.begin(), get_res.end());				
				check = true;
			}	
		}
	}
}

void create_LLtable()
{
	multimap<string, string>::iterator Mend, Miter;
	map<string, set<string> >::iterator iter ,Mbeg;
	set<string> first_set;
	set<string>::iterator Siter;
	string str;

	for(Miter = grammar.begin(); Miter != grammar.end(); Miter++)
	{
		str = Miter -> first;
		Mbeg = first.lower_bound(str);

		first_set = get_right_first(Miter -> second);

		for(Siter = first_set.begin(); Siter != first_set.end(); Siter++)
		{
			if(((*Siter).at(0) < 65) || ((*Siter).at(0) > 90))
				((LLtable.find(str)) -> second).insert(pair<string, string>(*Siter, Miter -> second));
		}

		if (is_produce_epsilon(str))
		{
			iter = follow.find(str);
			for (Siter = (iter -> second).begin(); Siter != (iter -> second).end(); Siter++)
			{
				if(((*Siter).at(0) < 65) || ((*Siter).at(0) > 90))
					((LLtable.find(str)) -> second).insert(pair<string, string>(*Siter, "epsilon"));
			}
		}
	}
}

string get_rule(string nonterminal, string terminal)
{
	multimap<string, map<string, string> >::iterator beg;

	beg = LLtable.lower_bound(nonterminal);

	while(beg -> first == nonterminal)
	{
		return (beg -> second).find(terminal) -> second;

		if (++beg == LLtable.end())
			break;
	}

	////// add exception //////////////////
}

void create_tree()
{
	char filename[] = "token.txt";

	ifstream ifile;
	ifile.open(filename, ios::in);

	if(!ifile)
	{
    	cout << "Fail to open file: " << filename << endl;
		return;
	}

	vector<string> stack, stack_temp;
	char temp = '0';
	string token_oneline, rule;
	
//for(vector<string>::iterator i = tree.begin(); i != tree.end(); i++) cout << *i << endl;
	
	stack.push_back("S");
	while(ifile.get(temp))
	{
		ifile.seekg(-1, ios::cur);

		if(temp == ' ')
		{
			while(ifile.get(temp))
			{
				if(temp == ':')
					break;
			}
			ifile.get();

			getline(ifile, token_oneline);
			token_oneline = trimEnd(token_oneline);

			//rule = get_rule(*(--stack.end()), token_oneline);
			cout << token_oneline.length() <<endl;
			exit(0);
		}
		else
			getline(ifile, token_oneline);
	}

	ifile.close();
}

void output_set()
{
	char filename[] = "set.txt";

	ofstream ofile;
	ofile.open(filename, ios::out);

	if(!ofile)
	{
    	cout << "Fail to open file: " << filename << endl;
		return;
	}

	map<string, set<string> >::iterator iter;
	map<string, bool>::iterator Biter;
	set<string>::iterator iiter;

// output Nullable
	ofile << "Nullable" << endl;
	for (Biter = nullable.begin(); Biter != nullable.end(); Biter++)
	{
		ofile << setiosflags(ios::left) << setw(20) << Biter -> first << " : " ;

		if (Biter -> second)
			ofile << "true";
		else
			ofile << "false";
		
		ofile << endl;
	}
	ofile << endl;

//output First
	ofile << "First" << endl;
	for (iter = first.begin(); iter != first.end(); iter++)
	{
		ofile << setiosflags(ios::left) << setw(20) << iter -> first << " :" ;

		for (iiter = (iter -> second).begin(); iiter != (iter -> second).end(); iiter++)
			ofile << " " << *iiter;

		ofile << endl;
	}
	ofile << endl;

//output Follow
	ofile << "Follow" << endl;
	for (iter = follow.begin(); iter != follow.end(); iter++)
	{
		ofile << setiosflags(ios::left) << setw(20) << iter -> first << " :" ;

		for (iiter = (iter -> second).begin(); iiter != (iter -> second).end(); iiter++)
			ofile << " " << *iiter;

		ofile << endl;
	}

	ofile.close();
}

void output_LLtable()
{
	char filename[] = "LLtable.txt";

	ofstream ofile;
	ofile.open(filename, ios::out);

	if(!ofile)
	{
    	cout << "Fail to open file: " << filename << endl;
		return;
	}

	map<string, map<string, string> >::iterator iter;
	map<string, string>::iterator iiter;

	ofile << "S" << endl;

	for (iter = LLtable.begin(); iter != LLtable.end(); iter++)
	{
		for (iiter = (iter -> second).begin(); iiter != (iter -> second).end(); iiter++)
		{
			ofile << setiosflags(ios::left) << setw(20) << iter -> first;
			ofile << setiosflags(ios::left) << setw(10) << iiter -> first;
			ofile << setiosflags(ios::left) << iiter -> second;
			ofile << endl;
		}

	}

	ofile.close();
}


int main()
{
	initial();

	G_ifile.open("./inputFile/grammar_orig.txt", ifstream::in);
	//cout << G_ifile;
	read_G();

	find_nullable() ; 
	find_first();
	find_follow();

	output_set();

	create_LLtable();
	output_LLtable();

	//create_tree();

/*	for (map<string, bool >::iterator iter = nullable.begin(); iter != nullable.end(); iter++)
	{
		cout << iter->first << endl;
		cout << " -> " ;
		//for (set<string>::iterator iiter = (iter->second).begin(); iiter != (iter->second).end(); iiter++)
		//	cout << *iiter << " ";
		cout << iter->second << endl;
		cout <<endl;
	}*/

	G_ifile.close();
}
