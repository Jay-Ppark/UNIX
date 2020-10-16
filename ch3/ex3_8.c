#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
//format: rm filename
int main(int argc, char* argv[]){
	if(argc == 3 && strcmp(argv[1],"rm") == 0){
		if(access (argv[2], W_OK) == -1){
			char c;
			printf("you don`t have a write permission!\n");
			printf("Do you really want to delete this file? (y/n)\n");
			scanf("%c",&c);
			if(c == 'y'){
				if(unlink(argv[2]) == -1){
					perror("unlink failed!\n");
					exit(1);
				}
			}
			else{
				printf("delete canceled!\n");
				exit(1);
			}	
		}
		else{
			if(unlink(argv[2]) == -1){
				perror("unlink failed!\n");
				exit(1);
			}
		}
	}
	else{
		printf("Incorrect format!\n Format: rm 'filename'\n");
		exit(1);
	}
	printf("Delete Succeeded!\n");
	exit(0);
}
