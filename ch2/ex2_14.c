#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int fileopen(char* filename, char* types){
	int filedes;
	if(strcmp(types,"r") == 0){
		filedes = open(filename,O_RDONLY);
	}
	else if(strcmp(types,"rw") == 0){
		filedes = open(filename,O_RDWR);
	}
	else if(strcmp(types,"w") == 0){
		filedes = open(filename,O_WRONLY);
	}
	else if(strcmp(types,"a") == 0){
		filedes = open(filename, O_WRONLY | O_APPEND);
	}
	else {
		printf("type error!\n");
		filedes=-1;
		return -1;
	}
	printf("success!\n");
	return filedes;
}

int main(int argc, char* argv[]){
	int filedes;
	filedes=fileopen(argv[1], argv[2]);
	printf("filedes :%d\n",filedes);
	exit(0);
}
