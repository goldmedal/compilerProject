#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<string>
using namespace std;

int lexer(fstream &fin);
string divide_token(string token);

int main(int argc, char* argv[]){

	char* fileName = argv[1];

	fstream fin;
	fin.open(fileName, ios::in);
	lexer(fin);

	return 0;

}

int lexer(fstream &fin){

	int start, end;
	char buffer[100];
	string token;

	while(fin.getline(buffer, sizeof(buffer), '\n')){
	
		string bufferString(buffer);
//		cout << buffer << endl;

		int i = 0;
		start = 0;

		while(bufferString[i] != '\0'){

			if(bufferString[i] == ' ' || bufferString[i] == '	'){	
				token = bufferString.substr(start, i-start);
//				cout << token << endl;
				divide_token(token);
				token.clear();	
				start = i+1;
			}

			i++;

		}

		token = bufferString.substr(start, 1);
		cout << token << endl;
		token.clear();
	}
}

string divide_token(string token){

	string category;

	// Keyword first

	if(!(token.compare("int"))) category.assign("Keyword"); 
	else if(!(token.compare("char"))) category.assign("Keyword");
	else if(!(token.compare("return"))) category.assign("Keyword");
	else if(!(token.compare("if"))) category.assign("Keyword");
	else if(!(token.compare("else"))) category.assign("Keyword");
	else if(!(token.compare("while"))) category.assign("Keyword");
	else if(!(token.compare("break"))) category.assign("Keyword");

}
