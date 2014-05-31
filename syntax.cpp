#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
#include <map>

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

void read_G()
{
	cout << number << endl;
		
	char temp = '0';
	string TTT;
	int i = 0, j = 0;
	//G_ifile >> temp;
	while(!G_ifile.eof())
	{
		tempofinput[temp] = true;
		G[i][j++] = temp;
		
		G_ifile.get(temp);
		G_ifile.seekg(-1, ios::cur);
		//G_ifile >> temp;
		getline(G_ifile, TTT);
		cout << trimEnd(TTT) << endl;
	}
	length[i] = j;
	
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
			str_vn[size_vn++] = i;
}

int main()
{
	initial();

	G_ifile.open("./inputFile/grammar_orig.txt", ifstream::in);
	//cout << G_ifile;
	read_G();

	G_ifile.close();
}
