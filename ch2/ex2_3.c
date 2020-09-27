#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
int main(){
	int filedes;
	if((filedes = creat("/home/jaehyung/practice/ch2/newfile", 0644)) == -1){
		printf("create fail!\n");
	        exit(1);	
	} 
	else {
		printf("create success!\n");
	}
	if((filedes = open ("newfile", O_RDWR)) == -1){
		printf("Couldn't open newfile\n");
		exit(1);
	}
	else {
		printf("open Success!\n");
	}
	exit(0);
}	
