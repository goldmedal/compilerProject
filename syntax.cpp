#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
#include <map>
#include <sstream>

using namespace std;

ifstream G_ifile;

multimap<string, string> grammar;
map<string, bool> nullable;

char G[20][20]; //use a matrix to store grammar G
int length[20]; //length use to store each formula's length
int number = 0;
bool tempofinput[150]; //buffer of input
char str_vn[20]; //put all vn into it
int size_vn = 0;
char str_vt[150]; //put all vt into it
int size_vt = 0;

void initial()
{

}

string trimEnd(string str)
{
	string delim = " " ;
	string r = str.erase(str.find_last_not_of(delim) + 0);
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

void map_overview(multimap<string, string> G)
{
	for (multimap<string, string>::iterator iter = G.begin(); iter != G.end(); iter++)
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
			//cout << l_grammar << endl;

			nullable.insert(pair<string, bool>(trimEnd(l_grammar), false));
		}
		else
		{
			getline(G_ifile, r_grammar);
			grammar.insert(pair<string, string>(trimEnd(l_grammar), trimEnd(r_grammar)));
			//cout << trimEnd(l_grammar) << endl;
			//cout << " || " ;
			//cout << trimEnd(l_grammar).length() <<endl;
		}
		//G_ifile.seekg(1, ios::cur);
	}

	//map_overview(grammar);

}

/*bool find_nullable(string str)
{//cout <<str<<endl;
	if(str == "epsilon")
		return true;

	if((str.at(0) < 65) || (str.at(0) > 90))
		return false;

	bool l_res = false, r_res = true;
	multimap<string, string>::iterator beg, end;
	string token;

	beg = grammar.lower_bound(str);
	//end = grammar.upper_bound(str);
	//end++;
	//beg++;

	//cout << (beg -> first).length() << endl;
	//cout << beg++ -> second << endl;
	//cout << beg++ -> second << endl;
	//cout << (str).length() << endl;
	//int count = grammar.count("BinOp");
	//cout << count << endl;

	while(beg -> first == str)
	{//cout << "1" << endl;
		istringstream iss(beg -> second);
		string token;

		r_res = true;
		while(getline(iss, token, ' '))
		{//cout << "2" << endl;
			//cout << token << endl;
			r_res &= find_nullable(token);
		}

		l_res |= r_res;

		beg++;
	}

	return l_res;
}
*/

void find_nullable()
{
	map<string, bool>::iterator iter, beg, end, find;
	multimap<string, string>::iterator Mbeg, Mend;
	string str, token;;

	for (iter = nullable.begin(); iter != nullable.end(); iter++)  
	{
			//cout << iter->first << " " << iter->second << endl;
		str = iter -> first;
		Mbeg = grammar.lower_bound(str);

		while(Mbeg -> first == str)
		{//cout << str << endl;
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


int main()
{
	initial();

	G_ifile.open("./inputFile/grammar_orig.txt", ifstream::in);
	//cout << G_ifile;
	read_G();

	find_nullable() ;

	G_ifile.close();
}
