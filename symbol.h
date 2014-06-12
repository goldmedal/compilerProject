#include<iostream>
#include<iomanip>
#include<map>
#include<string>
#include<algorithm>
using namespace std;

typedef struct symbolColumn {

	string token;
	string dataType;
	bool init;
	int scope;

}SCOL;

typedef map<string, SCOL> STable;
map<int, STable> build_symbol_table(TOKENLIST tokenList);
void output_symbol_table(map<int,STable> symTableList);
int search_different_scope(map<int, STable>, map<int, bool> checkTable, string index, int nowScope);

SCOL SCOL_init(){

	SCOL newSCOL;
	newSCOL.token = "id";
	newSCOL.init = false;
	return newSCOL;

}

bool SCOL_empty(SCOL scol){

	if(scol.dataType.empty())
		return true;
	else
		return false;
	
}


map<int, STable> build_symbol_table(TOKENLIST tokenList){

	int scope = 0;
	int lastscope = 0;
	int searchScope = -1;
	bool notDeclared = false;
	bool funcPara = false;
	bool assign = false;
	int keywordLine = -1;
	map<int, bool> numOfSTable;
	map<int, STable> symTableList;
	STable symTable;
	STable assignTable;
	string tmpIndex;
	string assignIndex;
	SCOL tmpColumn;
	SCOL findCol;
	numOfSTable[0] = true;
	tmpColumn.init = false;

	for(TOKENLIST::iterator iter = tokenList.begin(); iter != tokenList.end(); iter++){
		string category = (*iter).category;
// 		cout << (*iter).symbol << " " << (*iter).category << " " << (*iter).line << endl;
		if((*iter).line == keywordLine){

			if((*iter).symbol == "{"){
				keywordLine = -1;
			}else{
				continue;
			}

		}
		if(!(category.compare("Keyword"))){
			if((*iter).symbol ==  "int" || (*iter).symbol == "char"){
				tmpColumn.dataType = (*iter).symbol;
			}else if((*iter).symbol == "while" || (*iter).symbol == "if"){
				keywordLine = (*iter).line;
			}
		}else if(!(category.compare("Operators"))){
			string symbol = (*iter).symbol;
			if(!(symbol.compare("="))){
				if(!(tmpIndex.empty())){
						tmpColumn =  assignTable[tmpIndex];
						assign = true;
						assignIndex = tmpIndex;
				}
			}
		}else if(!(category.compare("Special"))){
			string symbol = (*iter).symbol;	
			if(!(symbol.compare("{"))){
				if(funcPara){
					lastscope = scope;
					symTableList[scope] = symTable;
					while(numOfSTable.find(++scope) != numOfSTable.end());
					numOfSTable[scope] = true;
					STable newSTable;
					symTable = newSTable;
				}
			}else if(!(symbol.compare("}"))){
				numOfSTable[scope] = false;
				symTableList[scope] = symTable;
				while(!(numOfSTable[--scope]) && scope == 0);
				if(scope < 0 ) scope = 0;
				symTable = symTableList[scope];
			}else if(!(symbol.compare("("))){
				TOKENLIST::iterator lastIter = iter - 2;
				if(!((*lastIter).category.compare("Keyword"))){
					if(!((*lastIter).symbol.compare("int")) || !((*lastIter).symbol.compare("char"))){
						lastscope = scope;
						symTableList[scope] = symTable;
						while(numOfSTable.find(++scope) != numOfSTable.end());
						STable newSTable;
						symTable = newSTable;
						numOfSTable[scope] = true;
						funcPara = true;
					}
				}
			}else if(!(symbol.compare(")"))){
				if(funcPara) funcPara = false;
			}else if(!(symbol.compare(";"))){
				if(assign){
					assign = false;
					if(searchScope == -1)
						symTable[assignIndex].init = true;
					else{
						symTableList[searchScope][assignIndex].init = true;
						searchScope = -1;
					}
				}

			}

		}else if(!(category.compare("Number"))){
//			if(SCOL_empty(tmpColumn)){
//				cout << "error: no delcare any variable" << endl;
//				exit(1);
//			}else{
			TOKENLIST::iterator lastIter = iter - 1;

				if(symTable.find(tmpIndex) != symTable.end() && (*lastIter).symbol.compare("[") ){
					findCol = symTable[tmpIndex];
					if(notDeclared){	
						tmpColumn.scope = scope;
						symTable[tmpIndex] = tmpColumn;
						findCol = SCOL_init();
						tmpColumn = SCOL_init();
						tmpIndex.clear();
					}else{
						cout << "error: this variable is declared" << endl;
						exit(1);
					}
				}
//			}
		}else if(!(category.compare("Identifier"))){
			
			TOKENLIST::iterator lastIter = iter - 1;
			TOKENLIST::iterator nextIter = iter + 1;
			string arraySign("[]");
			if(!((*lastIter).symbol.compare("int")) || !((*lastIter).symbol.compare("char"))){
				if( symTable.find((*iter).symbol) == symTable.end()){
					tmpColumn.scope = scope;
					tmpIndex = (*iter).symbol;
					if(!((*nextIter).symbol.compare("[")))
						tmpColumn.dataType = tmpColumn.dataType + arraySign;
					tmpColumn.token = "id";
					if(funcPara)
						tmpColumn.init = true;
					symTable[tmpIndex] = tmpColumn;
					findCol = SCOL_init();
					tmpColumn = SCOL_init();
					tmpIndex.clear();
				}else{
					cout << "error: variable repeat declared" << endl;
					exit(1);
				}
			}else if( symTable.find((*iter).symbol) != symTable.end()){
				notDeclared = true;
				tmpIndex = (*iter).symbol;
				assignTable = symTable;
			}else{
				searchScope = search_different_scope(symTableList, numOfSTable, (*iter).symbol, scope); 
				assignTable = symTableList[searchScope];
				notDeclared = true;
				tmpIndex = (*iter).symbol;
						
			}

		}


	}
	return symTableList;

}

void output_symbol_table(map<int,STable> symTableList){

	fstream fout;
	fout.open("symbol.txt", ios::out);

	STable symTable;
	for(map<int,STable>::iterator iterList = symTableList.begin(); iterList != symTableList.end(); iterList++){
		symTable = iterList->second;
		for(STable::iterator iter = symTable.begin(); iter != symTable.end(); iter++){
			fout << left << setw(5) << iter->first << " " << (iter->second).token << " " << left << setw(6) << (iter->second).dataType << " " << (iter->second).scope << endl;
//			if((iter->second).init) fout << " true" << endl;
//			else fout << " false" << endl;
		}
		fout << "\n";
	}
	fout.close();
}


int search_different_scope(map<int, STable> symTableList, map<int, bool> checkTable, string index, int nowScope){
	int i = nowScope-1;
	if(i >= 0){
		while(!(checkTable[i]) && i >= 0) i -= 1;
		if( symTableList[i].find(index) != symTableList[i].end() )
		{
			return i;
		}
		else
			return search_different_scope(symTableList, checkTable, index, i);
	}else{
		cout << "error: this variable no declared " << i << endl;
		exit(1);
	}

}
