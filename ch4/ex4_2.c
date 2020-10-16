#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>

int my_double_ls(const char *name){
	struct stat statbuf;
	struct dirent *d;
	DIR *dp;
	if((dp=opendir(name)) == NULL)
		return -1;
	while(d=readdir(dp)){
		if(stat(d->d_name,&statbuf) != -1){
			if(S_ISDIR(statbuf.st_mode))
				printf("%s*\n",d->d_name);
			else
				printf("%s Size: %ld\n",d->d_name,statbuf.st_size);
		}
	}
	closedir(dp);
	return 0;
}
int main(int argc,char *argv[]){
	if(argc != 2)
		printf("Incorrect format!\nFormat: 'filename'\n");
	else
		my_double_ls(argv[1]);
	return 0;
}
