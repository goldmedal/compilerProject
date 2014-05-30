#include"iostream"
#include"fstream"
#include"stack"
#include"utility"

using namespace std;

ifstream G_ifile;

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

void Read_G()
{
	G_ifile >> number; //user should give the number of formula first
	for(int i = 1; i <= number; i++)
	{
		char temp;
		int j = 0;
		G_ifile >> temp;
		while(temp != '$')
		{
			tempofinput[temp] = true;
			G[i][j++] = temp;
			G_ifile >> temp;
		}
		length[i] = j;
	}
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

	G_ifile.open("d:\\grammar.txt");
}
