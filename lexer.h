#include<iostream>
#include<iomanip>
#include<fstream>
#include<map>
#include<vector>
#include<string>
using namespace std;

typedef struct Token {

	int line;
	string category;
	string symbol;

}TOKEN;

typedef vector<TOKEN> TOKENLIST;

TOKENLIST lexer(fstream &fin);
string divide_token(string token);
void output_token_list(TOKENLIST);
TOKEN set_token(int line, string category, string symbol);

TOKENLIST lexer(fstream &fin){

	int start, line=1;
	char buffer[100];
	TOKENLIST backTokenList;

	string token;
	string category;


	while(fin.getline(buffer, sizeof(buffer), '\n')){
	
		string bufferString(buffer);
		int i = 0;
		start = 0;


		while(bufferString[i] != '\0'){

			if(bufferString[i] == ' ' || bufferString[i] == '	'){	
				token = bufferString.substr(start, i-start);
				if(!token.empty()){
					category = divide_token(token);
					if(!category.empty()){
						backTokenList.insert(backTokenList.end(), set_token(line, category, token));
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
				backTokenList.insert(backTokenList.end(), set_token(line, category, token));
			}
		}
		token.clear();
		line++;
	}
	return backTokenList;
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

TOKEN set_token(int line, string category, string symbol){

	TOKEN tmpToken;

	tmpToken.symbol = symbol;
	tmpToken.category = category;
	tmpToken.line = line;
	
	return tmpToken;

}

void output_token_list(TOKENLIST tokenList){

	fstream fout;
	fout.open("token.txt", ios::out);
	int i = 0;
	TOKEN nextToken;

	while(i < tokenList.size()){

		if(i == 0 || ( tokenList[i-1].line != tokenList[i].line )){
			fout << "Line" << right << setw(3) << tokenList[i].line << ":" << endl;
		}
		string category("<");
		category = category + tokenList[i].category + ">";
		fout << "         " << left << setw(13) << category << ": " << tokenList[i].symbol << endl;

		i++;
	}

	fout.close();
	
}
