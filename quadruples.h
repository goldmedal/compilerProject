#include<iostream>
#include<iomanip>
#include<map>
#include<string>
#include<sstream>
#include<set>

using namespace std;

typedef struct quadruples {
	
	string op;
	string arg1;
	string arg2;
	string result;

} QUAD;

typedef map<int, QUAD> QList;

QUAD quad_init(){

	QUAD newQuad;
	return newQuad;

}

string int2str(int &i){

	string s;
	stringstream ss;
	ss << i;

	return ss.str();
}

QList quadruples(TOKENLIST tokenList, map<int, STable> symTableList){

	QList quadList;
	int qnum = 0;
	int reg = 0;
	int i = 0;
	int record = -1;
	int ifwhile = 0;
	set<int> breaklist;

	for(TOKENLIST::iterator iter = tokenList.begin(); iter != tokenList.end(); iter++){

		QUAD tmpQuad;
		if((*iter).symbol == "="){
			tmpQuad.op = "=";		

			if(record >=0 ) record++;
			TOKENLIST::iterator last = iter - 1;
			while((*last).category != "Identifier" ) last--;
			tmpQuad.result = (*last).symbol;

			last = iter + 1;
			while((*last).category != "Identifier" && (*last).category != "Number" ) last++;
			tmpQuad.arg1 = (*last).symbol;
			last = last + 1;
			while((*last).category != "Operators" && (*last).category != "Number" && (*last).symbol != ";") last++;
			if((*last).symbol == ";") {
				quadList[qnum] = tmpQuad;
				tmpQuad = quad_init();
			}else if((*last).category == "Operators"){
				if(record >=0 ) record++;
				QUAD operaQuad;
				string tStr("t");
				tStr = tStr + int2str(reg);
				reg++;
				tmpQuad.arg1 = tStr;
				operaQuad.result = tStr;
				operaQuad.op = (*last).symbol;
					
				while((*last).category != "Identifier" && (*last).category != "Number" ) last--;
				operaQuad.arg1 = (*last).symbol;
				
				last = last + 1;
				while((*last).category != "Identifier" && (*last).category != "Number" ) last++;
				operaQuad.arg2 = (*last).symbol;
				
				quadList[qnum++] = tmpQuad;
				quadList[qnum] = operaQuad;
				tmpQuad = quad_init();
		
			}

			qnum++;

		}else if((*iter).symbol == "while" || (*iter).symbol == "if" ){

			tmpQuad.op = "jfalse";
			record++;
			TOKENLIST::iterator last = iter + 1;
			while((*last).category != "Operators") last++;
			if((*last).symbol == "!"){
				QUAD operaQuad;
				string tStr("t");
				tStr = tStr + int2str(reg);
				reg++;
				tmpQuad.arg2 = tStr;
				operaQuad.result = tStr;
				operaQuad.op = "!";
				while((*last).category != "Identifier") last++;
				operaQuad.arg1 = (*last).symbol;
				quadList[qnum++] = operaQuad;

			}else{

				QUAD operaQuad;
				string tStr("t");
				tStr = tStr + int2str(reg);
				reg++;
				tmpQuad.arg2 = tStr;
				operaQuad.result = tStr;
				operaQuad.op = (*last).symbol;
				while((*last).category != "Identifier" && (*last).category != "Number" ) last--;
				operaQuad.arg1 = (*last).symbol;
				
				last = last + 1;
				while((*last).category != "Identifier" && (*last).category != "Number" ) last++;
				operaQuad.arg2 = (*last).symbol;
				
				quadList[qnum++] = operaQuad;

			}

			quadList[qnum] = tmpQuad;
			ifwhile = qnum;
			qnum++;

		}else if((*iter).symbol == "}" && record >= 0){
			int result = ifwhile + record + 1;
			quadList[ifwhile].arg1 = int2str(result);
			for(set<int>::iterator a = breaklist.begin(); a != breaklist.end(); a++)
				quadList[*a].arg1 = int2str(result);
			record = -1;

		}else if((*iter).symbol == "break"){
			tmpQuad.op = "jmp";
			if(record >=0 ) record++;
			breaklist.insert(qnum);
			quadList[qnum++] = tmpQuad;
		}



	}

	return quadList;

}

void output_quadruples(QList quadList){
	fstream fout;
	fout.open("quadruples.txt", ios::out);
	for(QList::iterator iter = quadList.begin(); iter != quadList.end() ; iter++){

		QUAD tmp = iter->second;
		fout << iter->first << " "  << tmp.op << " " << tmp.arg1 << " " << tmp.arg2 << " " << tmp.result << endl;

	}

}






