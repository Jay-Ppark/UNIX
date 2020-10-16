#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
char oct[4] = "";

void lsoct(char *ls_v){
	int i;
	int ls_sum = 0;
	for(i=0; i<9; i++){
		if(ls_v[i] == 'r' && i%3 == 0)
			ls_sum += 4;
		else if(ls_v[i] == 'w' && i%3 ==1)
			ls_sum += 2;
		else if(ls_v[i] == 'x' && i%3 ==2)
			ls_sum += 1;
		else if(ls_v[i] == '-')
			ls_sum += 0;
		else
			printf("Error! Incorrect format!\n File permission: ---------\n");
		if(i%3 == 2){
			oct[i/3 + 1] = (ls_sum+48);
			ls_sum=0;
		}
	}
}
int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Error! Format -> 'filename' ----, 'filename' ---------\n");
		return 0;
	}else if(strlen(argv[2]) == 9){
		oct[0] = '0';
		lsoct(argv[2]);
	}else if(strlen(argv[2]) != 4){
		printf("Error! Incorrect format!\n");
		printf("File permission: ---- or ---------\n");
		return 0;
	}else
		strcpy(oct, argv[2]);
	int per;
	per = strtol(oct, NULL, 8);
	if(chmod(argv[1], per) == -1)
		perror("call to chmod failed!\n");
	return 0;
}

