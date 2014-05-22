#include<stdio.h>
#include<stdlib.h>

int grammarToNFA(char* grammarName);


int main(int argc, char* argv[]){

	char* grammarName = argv[1];
	grammarToNFA(grammarName);

}


int grammarToNFA(char* grammarName){

	FILE *fp;
	char buffer[100];

	fp = fopen(grammarName, "r");

	if(!fp){
		printf("File open error. \n");
		exit(1);
	}

	while(fgets(buffer, 100, fp) != NULL){
		if(buffer[0] != ' '){
			printf("Header => %s", buffer);
		}else{
			printf("Body   => %s", buffer);
		}
	}

	printf("\ngrammer end\n");

	fclose(fp);

}
