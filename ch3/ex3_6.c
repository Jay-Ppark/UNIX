#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void whatable(char *filename){
	if(access(filename, R_OK) == -1)
		perror("cannot read this file!\n");
	else
		printf("can read this file!\n");
	if(access(filename, W_OK) == -1)
		perror("cannot write this file!\n");
	else
		printf("can write this file!\n");
	if(access(filename, X_OK) == -1)
		perror("cannot execute this file!\n");
	else
		printf("can execute this file!\n");
}

int main(int argc, char* argv[]){
	if(argc == 2)
		whatable(argv[1]);
	else
		printf("Error! incorrect format -> 'filename'\n");
	return 0;
}	
