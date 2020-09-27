#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

#define SIZE 512

int main(int argc, char* argv[]){
	ssize_t nread;
	int filedes;
	int i;
	char buf[SIZE];
	if(argc>1) {
		for(i=1;i<argc;i++){
			if((filedes = open(argv[i], O_RDWR)) == -1) {
				printf("cannot open!\n");
				exit(1);
			}
			while((nread = read(filedes,buf,SIZE))>0){
				write(1,buf,nread);
			}
			close(filedes);
		}
	}
	else {
		nread=read(0,buf,SIZE);
		write(1,buf,nread);
	}
	exit(0);
}
