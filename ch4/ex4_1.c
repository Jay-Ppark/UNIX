#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int my_double_ls(const char *name, int skip){
	struct dirent *d;
	DIR *dp;
	if((dp=opendir(name)) == NULL)
		return -1;
	if(skip == 0){
		while(d=readdir(dp)){
			if(d->d_ino != 0)
				printf("%s\n", d->d_name);
		}
		rewinddir(dp);
		while(d = readdir(dp)){
			if(d->d_ino != 0)
				printf("%s\n", d->d_name);
		}
		closedir(dp);
	}else if(skip == 1){
		while(d=readdir(dp)){
			if(strcspn(d->d_name,".") == 0){
				my_double_ls(d->d_name,0);
			}
		}
		closedir(dp);
	}else
		printf("option is 0 or 1\n");
				
	return 0;
}
int main(int argc, char *argv[]){
	int opt;
	if(argc != 2)
		printf("Incorrect format!\n'filename'\n");
	else{
		printf("insert opt 0 or 1 :");
		scanf("%d", &opt);
		my_double_ls(argv[1], opt);
	}
	return 0;
}
