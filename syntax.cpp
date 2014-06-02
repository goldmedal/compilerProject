#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
#include <map>
#include <sstream>

using namespace std;

ifstream G_ifile;

multimap<string, string> grammar;

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

string trimEnd(string &str)
{
	const string &delim =" \t" ;
	string r=str.erase(str.find_last_not_of(delim)+1);
	return r.erase(0,r.find_first_not_of(delim));
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
		}
		else
		{
			getline(G_ifile, r_grammar);
			grammar.insert(pair<string, string>(trimEnd(l_grammar), trimEnd(r_grammar)));  
		}
		//G_ifile.seekg(1, ios::cur);
	}
	
	map_overview(grammar);
	/*length[i] = j;
	
	G[0][0] = 'S';
	G[0][1] = G[1][0];
	length[0] = 2;
	for(int i = 0; i < 64; i++)
		if(tempofinput[i])
			str_vt[size_vt++] = i;
	for(int i = 91; i < 128; i++)
		if(tempofinput[i])
			str_vt[size_vt++] = i;
	for(int i = 65; i < 91; i++)
		if(tempofinput[i])
			str_vn[size_vn++] = i;*/
}

bool find_nullable(string str)
{
	if(str == "epsilon")
		return true;
	
	if((str.at(0) <= 65) || (str.at(0) >= 90))
		return false;

	bool l_res = false, r_res = true;
	map<string, string>::iterator beg, end;  
	string token;
	
	beg = grammar.lower_bound(str);
	end = grammar.upper_bound(str);
	//end++;
	//beg++;

//	cout << beg -> second <<endl;
//	cout << end -> second << endl;

	int count = grammar.count(str);
	cout <<count <<endl;

	while(0) 
	{cout << "1" << endl;
		istringstream iss(beg++ -> second);
		//string token;
	
		r_res = true;
		while(getline(iss, token, ' '))
		{cout << "2" << endl;
			//cout << token << endl;
			r_res &= find_nullable(token);
		}

		l_res |= r_res;
	}

	return l_res;
}

int main()
{
	initial();

	G_ifile.open("./inputFile/grammar_orig.txt", ifstream::in);
	//cout << G_ifile;
	read_G();

	cout << find_nullable("BinOp") << endl;

	G_ifile.close();
}
