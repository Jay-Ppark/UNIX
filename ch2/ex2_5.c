#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

#define BUFSIZE 1

int main(){
	char* buffer;
	int filedes;
	ssize_t nread;
	long total;
	long line;
	long isword;
	total=0;
	line=0;
	isword=0;
	if((filedes = open("anotherfile", O_RDWR)) == -1){
		printf("file open fail!\n");
		exit(1);
	}
	while((nread = read(filedes,buffer,BUFSIZE)) > 0) {
		if(buffer[0] == '\n') {
			total++;
			line++;
			if(isword == 1){
				total--;
			}
			isword = 1;
		}
		else if(buffer[0] == '\t') {
			total++;
			if(isword == 1){
				total--;
			}
			isword = 1;
		}
		else if(buffer[0] == ' ') {
			total++;
			if(isword == 1) {
				total--;
			}
			isword = 1;
		}
		else {
			isword = 0;
		}
	}
	printf("total chars in anotherfile: %ld\n",total);
	printf("total lines in anotherfile: %ld\n",line);
	exit(0);
}

