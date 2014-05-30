#include<iostream>
#include<iomanip>
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
	fin.close();

	return 0;

}

int lexer(fstream &fin){

	int start, line=1;
	char buffer[100];
	fstream fout;
	string token;
	string front("<");
	string category;

	fout.open("token.txt", ios::out);

	while(fin.getline(buffer, sizeof(buffer), '\n')){
	
		string bufferString(buffer);
		int i = 0;
		start = 0;

		fout << "Line" << right << setw(3) << line << ":" << endl;

		while(bufferString[i] != '\0'){

			if(bufferString[i] == ' ' || bufferString[i] == '	'){	
				token = bufferString.substr(start, i-start);
//				cout << token << endl;
				if(!token.empty()){
					category = divide_token(token);
					if(!category.empty()){
						category = front + category;
						category.append(">");
						fout << "         " << left << setw(13) << category << ": " << token << endl;
					}
				}
				token.clear();	
				start = i+1;
			}

			i++;

		}

		token = bufferString.substr(start, 1);

		if(!token.empty()){
			category = divide_token(token);
			if(!category.empty()){
				category = front + category;
				category.append(">");
				fout << "         " << left << setw(13) << category << ": " << token << endl;
			}
		}
		token.clear();
		line++;
	}
	fout.close();
}

string divide_token(string token){

	string category;

	if(token[0] == '/' && token[1] == '/') category.assign("Comment");
	
	else if(token[0] == 39) category.assign("Char");

	
		// Keyword first

		else if(!(token.compare("int"))) category.assign("Keyword"); 
		else if(!(token.compare("char"))) category.assign("Keyword");
		else if(!(token.compare("return"))) category.assign("Keyword");
		else if(!(token.compare("if"))) category.assign("Keyword");
		else if(!(token.compare("else"))) category.assign("Keyword");
		else if(!(token.compare("while"))) category.assign("Keyword");
		else if(!(token.compare("break"))) category.assign("Keyword");

		// Operators

		else if(!(token.compare("="))) category.assign("Operators");
		else if(!(token.compare("!"))) category.assign("Operators");
		else if(!(token.compare("+"))) category.assign("Operators");
		else if(!(token.compare("-"))) category.assign("Operators");
		else if(!(token.compare("*"))) category.assign("Operators");
		else if(!(token.compare("/"))) category.assign("Operators");
		else if(!(token.compare("=="))) category.assign("Operators");
		else if(!(token.compare("!="))) category.assign("Operators");
		else if(!(token.compare("<"))) category.assign("Operators");
		else if(!(token.compare(">"))) category.assign("Operators");
		else if(!(token.compare("<="))) category.assign("Operators");
		else if(!(token.compare(">="))) category.assign("Operators");
		else if(!(token.compare("&&"))) category.assign("Operators");
		else if(!(token.compare("||"))) category.assign("Operators");

		// Special

		else if(!(token.compare("["))) category.assign("Special");
		else if(!(token.compare("]"))) category.assign("Special");
		else if(!(token.compare("("))) category.assign("Special");
		else if(!(token.compare(")"))) category.assign("Special");
		else if(!(token.compare("{"))) category.assign("Special");
		else if(!(token.compare("}"))) category.assign("Special");
		else if(!(token.compare(";"))) category.assign("Special");
		else if(!(token.compare(","))) category.assign("Special");

	else if(token[0] >= 48 && token[0] <=57) category.assign("Number");
	else if((token[0] >=65 && token[0] <=90) || (token[0] >= 97 && token[0] <= 122)) category.assign("Identifier");
	
	return category;

}
