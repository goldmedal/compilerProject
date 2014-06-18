#include <ctype.h>
#include"lexer.h"
#include"symbol.h"
#include"quadruples.h"

string opcode(string str)
{
      if(str == "+")return "ADD";
      if(str == "-")return "SUB";
      if(str == "*")return "MUL";
      if(str == "/")return "DIV";
      if(str == ">")return "GT";
      if(str == ">=")return "GE";
      if(str == "<")return "LT";
      if(str == "<=")return "LE";
      if(str == "&&")return "AND";
      if(str == "||")return "OR";
      if(str == "!=")return "NE";
      if(str == "==")return "EQ";

      cout << "Wrong op!" << endl;
}

struct Quad_row{ 
	int index;
	string op;
	string arg1;
	string arg2;
	string result;
};

vector<string> variable;
int mem_add(string tok);
void machine_code(vector<struct Quad_row> quadruple)
{
      ofstream outfile;
      outfile.open ("code.tm");
      
      int line = 0;
      int reg_num = 1;
      int reg1, mem, while_jump = 0;
      outfile << line++ << ": LDC 0,0(0)" << endl;  
     
      vector<struct Quad_row>::iterator qiter ; 
      for(qiter = quadruple.begin(); qiter != quadruple.end(); qiter++)
      {
            if(qiter -> op == "=")
            {
                  if(qiter -> arg1[0]=='t' && qiter -> result[0]!='t')
                  {
                        outfile << line++ << ": ST " << reg1 << "," << mem_add(qiter->result) << "(0)"<< endl;
                  }
                  else if(qiter->arg1[0]=='t' && qiter->result[0]=='t')
                  {
                        if(reg1==1)reg1=6;
                        outfile << line++ << ": ST " << reg1-1 << "," << mem << "(0)"<< endl;
                  }
                  else if(qiter->arg1[0]!='t' && isdigit(qiter->arg1[0])) 
                  {
                        reg1=reg_num;
                        outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg1 << "(0)"<< endl;
                  }
                  else if(qiter->arg1[0]!='t' && !(isdigit(qiter->arg1[0])))  
                  {
                        reg1=reg_num;
                        outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg1) << "(0)"<< endl;
                  }
            }
            else if(qiter->op=="+" || qiter->op=="-" || qiter->op=="*" || qiter->op=="/")
            {
                  if(reg_num+3>6)reg_num=1;
                  
                  if(qiter->arg1[0]!='t' && qiter->arg2[0]!='t')
                  {
                        if(   !(isdigit(qiter->arg1[0])) && !(isdigit(qiter->arg2[0])) )
                        {
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg1) << "(0)"<< endl;
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg2) << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                              
                        }
                        else if((isdigit(qiter->arg1[0])) && !(isdigit(qiter->arg2[0])))
                        {
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg1 << "(0)"<< endl;
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg2) << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                        }
                        else if(!(isdigit(qiter->arg1[0])) && (isdigit(qiter->arg2[0])))
                        {
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg1) << "(0)"<< endl;
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg2 << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                        }
                        else
                        {
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg1 << "(0)"<< endl;
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg2 << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                        }
                        
                  }
                  else
                  {
                        outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg1 << "," << reg1-1 << endl;
                        reg1=reg_num;
                        reg_num++;
                  }
                  
            }
            /*else if(qiter->op=="[]")  
            {
                  mem=mem_add(qiter->arg1+qiter->op[0]+qiter->arg2+qiter->op[1]);
                  reg1=reg_num;
                  outfile << line++ << ": LD " << reg_num++ << "," << mem << "(0)"<< endl;
            }*/
            
            else if(qiter->op==">" || qiter->op==">=" || qiter->op=="<" || qiter->op=="<=" || qiter->op=="&&" || qiter->op=="||" || qiter->op=="!=" || qiter->op=="==")
            {
                  if(reg_num+3>6)reg_num=1;
                  if(qiter->arg1[0]!='t' && qiter->arg2[0]!='t')
                  {
                        while_jump = line;
                        if(   !(isdigit(qiter->arg1[0])) && !(isdigit(qiter->arg2[0])) )
                        {
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg1) << "(0)"<< endl;
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg2) << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                              
                        }
                        else if((isdigit(qiter->arg1[0])) && !(isdigit(qiter->arg2[0])))
                        {
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg1 << "(0)"<< endl;
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg2) << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                        }
                        else if(!(isdigit(qiter->arg1[0])) && (isdigit(qiter->arg2[0])))
                        {
                              outfile << line++ << ": LD " << reg_num++ << "," << mem_add(qiter->arg1) << "(0)"<< endl;
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg2 << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                        }
                        else
                        {
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg1 << "(0)"<< endl;
                              outfile << line++ << ": LDC " << reg_num++ << "," << qiter->arg2 << "(0)"<< endl;
                              outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg_num-2 << "," << reg_num-1 << endl;
                              reg1=reg_num;
                        }
                        
                  }
                  else
                  {
                        outfile << line++ << ": " << opcode(qiter->op)<< " " << reg_num << "," << reg1 << "," << reg1-1 << endl;
                        reg_num++;
                  }
                  
            }
            else if(qiter->op=="jfalse")
            {
                  vector<struct Quad_row>::iterator qiter ; 
                  int things=0;
                  
                  for(qiter= qiter; qiter != quadruple.end(); qiter++)
                  {
                        if(qiter->op=="=" || qiter->op=="[]")things=things+1;
                        else if(qiter->op=="+" || qiter->op=="-" || qiter->op=="*" || qiter->op=="/")
                        {
                              if(qiter->arg1[0]!='t' && qiter->arg2[0]!='t')things=things+3;
                              else things=things+1;
                        }
                        
                        if(qiter->op=="jmp")break;
                  }
                  
                  outfile << line++ << ": JEQ " << reg1 << "," << (line+things+2) << "(0)"<< endl;
            }
            else if(qiter->op=="jmp")
            {
                  int myarg1 =atoi((qiter->arg1).c_str());
                  if((qiter->index) > myarg1)  //while
                        outfile << line++ << ": JEQ 0,"<< while_jump << "(0)"<< endl;
                        
                  else //if-else
                  {
                        int quadnum=myarg1-(qiter->index);
                        vector<struct Quad_row>::iterator qiter ; 
                        int things=0;
                  
                        for(qiter= qiter; quadnum != 0; qiter++,quadnum--)
                        {
                              if(qiter->op=="=" || qiter->op=="[]")things=things+1;
                              else if(qiter->op=="+" || qiter->op=="-" || qiter->op=="*" || qiter->op=="/")
                              {
                                    if(qiter->arg1[0]!='t' && qiter->arg2[0]!='t')things=things+3;
                                    else things=things+1;
                              }
                        }
                        outfile << line++ << ": JEQ 0," << (line+things+2) << "(0)"<< endl;
                  }     
            }
            /*else if(qiter->op=="return")
            {
                  if(qiter->result[0]!='t')
                  {
                        if(isdigit(qiter->result[0]))outfile << line++ << ": LDC 1," << qiter->result << "(0)"<< endl;
                        else if(!(isdigit(qiter->result[0])))outfile << line++ << ": LD 1," << mem_add(qiter->result) << "(0)"<< endl;
                  }
                  else
                  {
                        outfile << line++ << ": LDA 1,0(" << reg1 << ")"<< endl;
                  }
            }*/
            
            if(reg_num==6)reg_num=1;
      }
      
      outfile << line++ << ": OUT 1,0,0" << endl;
      outfile << line++ << ": HALT 1,0,0" << endl;
      
      for(int i=0;i<variable.size();i++)
        cout << variable[i] << " " << i << endl;
}

int mem_add(string tok)
{
    for(int i=0 ;i<variable.size() ;i++)
        if(variable[i]==tok)
            return i;
                  
    variable.push_back(tok);
      
    return variable.size()-1;
}

vector<struct Quad_row> map2vector(QList quadList){

	vector<struct Quad_row> Qrow;
	for(QList::iterator iter = quadList.begin(); iter != quadList.end(); iter++){
		struct Quad_row tmpRow;
		tmpRow.index = iter->first;
		tmpRow.op = (iter->second).op;
		tmpRow.arg1 = (iter->second).arg1;
		tmpRow.arg2 = (iter->second).arg2;
		tmpRow.result = (iter->second).result;
		Qrow.push_back(tmpRow);
	}
	return Qrow;

}


int main(int argc, char* argv[]){

	char* fileName = argv[1];
	fstream fin;
	fin.open(fileName, ios::in);
	TOKENLIST tokenList;
	vector<struct Quad_row> Qrow;
	STable symTable; 
	QList quadList;
	map<int, STable> symTableList;
	
	tokenList = lexer(fin);
	output_token_list(tokenList);
	symTableList = build_symbol_table(tokenList);
	output_symbol_table(symTableList);
	quadList = quadruples(tokenList, symTableList);
   output_quadruples(quadList);
	Qrow = map2vector(quadList);
	machine_code(Qrow);

	fin.close();

}

