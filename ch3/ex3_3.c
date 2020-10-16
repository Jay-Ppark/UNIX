#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int oct[4];
char ls[9];
int lsoct(char *ls_v){
	int i;
	int ls_sum = 0;
	for(i=0; i<9; i++){
		if(ls_v[i] == 'r' && i%3 == 0)
			ls_sum += 4;
		else if(ls_v[i] == 'w' && i%3 == 1)
			ls_sum += 2;
		else if(ls_v[i] == 'x' && i%3 == 2)
			ls_sum += 1;
		else if(ls_v[i] == '-')
			ls_sum += 0;
		else
			return -1;
		if(i%3 == 2){
			oct[i/3 + 1] = ls_sum;
			ls_sum = 0;
		}
	}
	return 0;
}
int octls(char *oct_v){
	int i;
	for(i=1; i<4; i++){
		if(oct_v[i] == '7')
			strcat(ls, "rwx");
		else if(oct_v[i] == '6')
			strcat(ls, "rw-");
		else if(oct_v[i] == '5')
			strcat(ls, "r-x");
		else if(oct_v[i] == '4')
			strcat(ls, "r--");
		else if(oct_v[i] == '3')
			strcat(ls, "-wx");
		else if(oct_v[i] == '2')
			strcat(ls, "-w-");
		else if(oct_v[i] == '1')
			strcat(ls, "--x");
		else if(oct_v[i] == '0')
			strcat(ls, "---");
		else
			return -1;
	}
	return 0;
}
int main(int argc, char* argv[]){
	if(argc == 3){
		if(!strcmp(argv[1],"lsoct")){
			if(lsoct(argv[2]) == 0){
				printf("%d%d%d%d\n", oct[0], oct[1], oct[2], oct[3]);
			}else
				printf("Error! incorrect format! ---------\n");
		}else if(!strcmp(argv[1],"octls")){
			if(octls(argv[2]) == 0){
				printf("%s\n", ls);
			}else
				printf("Error! incorrect format! ----\n");
		}else
			printf("Error! no option!\n");
	}else
		printf("Error! format\n1.lsoct ---------\n2.octls ----\n");
	return 0;
}
