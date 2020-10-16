#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
char oct[4] = "";
int user_chk;
int group_chk;
int other_chk;
int any_chk;
int opt_chk;
void chmod_num(char *per, char *name){
	int i;
	if(strlen(per) == 4){
		for(i=0;i<4;i++)
			oct[i] = per[i];
	}else if(strlen(per) == 3){
		oct[0] = '0';
		for(i=1;i<4;i++)
			oct[i] = per[i-1];
	}else if(strlen(per) == 2){
		oct[0] = '0';
		oct[1] = '0';
		for(i=2;i<4;i++)
			oct[i] = per[i-2];
	}else{
		for(i=0;i<3;i++)
			oct[i] = '0';
		oct[3] = per[0];
	}
	int i_per;
	i_per = strtol(oct,NULL,8);
	if(chmod(name, i_per) == -1){
		perror("call to chmod failed!\n");
		exit(1);
	}
	printf("chmod succeeded!\n");
	exit(0);
}
void chmod_char(char *per, char *name){
	int i;
	int opt;
	int u_sum = 0;
	int g_sum = 0;
	int o_sum = 0;
	for(i=0;i<strlen(per);i++){
		if(per[i] == 'u' && user_chk == 0){
			user_chk = 1;
			any_chk = 1;
		}else if(per[i] == 'g' && group_chk == 0){
			group_chk = 1;
			any_chk = 1;
		}else if(per[i] == 'o' && other_chk == 0){
			other_chk = 1;
			any_chk = 1;
		}else if(per[i] == '+' && any_chk != 0 && opt_chk == 0){
			opt = 1;
			opt_chk = 1;
		}else if(per[i] == '-' && any_chk != 0 && opt_chk == 0){
			opt = -1;
			opt_chk = 1;
		}else if(per[i] == 'r' && any_chk != 0 && opt_chk != 0){
			if(user_chk != 0)
				u_sum += 4;
			if(group_chk != 0)
				g_sum += 4;
			if(other_chk != 0)
				o_sum += 4;
		}else if(per[i] == 'w' && any_chk != 0 && opt_chk != 0){
                         if(user_chk != 0)
                                u_sum += 2;
                         if(group_chk != 0)
                                g_sum += 2;
                         if(other_chk != 0)   
                                o_sum += 2;
		}else if(per[i] == 'x' && any_chk != 0 && opt_chk != 0){
			if(user_chk != 0)
                                u_sum += 1;
                        if(group_chk != 0)
                                g_sum += 1;
                        if(other_chk != 0)   
                                o_sum += 1;
		}else{
			printf("Format chmod opt 'filename'\nopt: u|g|o|(+,-)|r|w|x\n");
			exit(1);
		}
	}
	oct[0] = '0';
	if(opt == 1){
		if(user_chk == 1)
			oct[1] = (u_sum+48);
		else
			oct[1] = '0';
		if(group_chk == 1)
			oct[2] = (g_sum+48);
		else
			oct[2] = '0';
		if(other_chk == 1)
			oct[3] = (o_sum+48);
		else
			oct[3] = '0';
	}
	else{
		if(user_chk == 1)
			oct[1] = (7-u_sum+48);
		else
			oct[1] = '7';
		if(group_chk == 1)
			oct[2] = (7-g_sum+48);
		else
			oct[2] = '7';
		if(other_chk == 1)
			oct[3] = (7-o_sum+48);
		else
			oct[3] = '7';
	}
	int i_per;
        i_per = strtol(oct,NULL,8);
        if(chmod(name, i_per) == -1){
                perror("call to chmod failed!\n");
                exit(1);
        }
        printf("chmod succeeded!\n");
        exit(0);		
}
int main(int argc, char* argv[]){
	if(argc != 4 || strcmp(argv[1],"chmod") != 0){
		printf("Format: chmod opt 'filename'\n");
		exit(1);
	}else{
		int j;
		for(j=0;j<=9;j++){
			if(argv[2][0] == (j+48)){
				chmod_num(argv[2],argv[3]);
				break;
			}
		}
		chmod_char(argv[2], argv[3]);
	}
	exit(0);
}
