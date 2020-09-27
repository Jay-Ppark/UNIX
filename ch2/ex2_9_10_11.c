#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

#define NAMELENGTH 5
#define NROOMS 10

char namebuf[NAMELENGTH];
int infile = -1;
off_t offset;
ssize_t nread;

char *getoccupier (int roomno) {
	infile = -1;
	if(infile == -1 && (infile = open("residents", O_RDONLY)) == -1) {
		return NULL;
	}
	offset = (roomno - 1) * NAMELENGTH;
	if(lseek(infile,offset,SEEK_SET) == -1) {
		return NULL;
	}
	if((nread = read(infile,namebuf,NAMELENGTH)) <= 0){
		return NULL;
	}
	namebuf[nread - 1] = '\0';
	if(namebuf[0] == ' ') {
		return NULL;
	}
	return namebuf;
}

int findfree(){
	int tmp;
	tmp = NROOMS;
	char *getoccupier(int), *p;
	int j;
	for(j = 1; j <= NROOMS; j++){
		if(!(p = getoccupier(j))) {
			tmp = j;
			break;
		}
	}
	return tmp;
}	

void addguest() {
	int vacant;
	char name[NAMELENGTH];
	vacant = findfree();
	if(vacant == NROOMS) {
		printf("No ROOMS!\n");
	}
	else {
		infile = -1;
		if(infile == -1 && (infile = open("residents", O_RDWR)) == -1) {
			printf("Error on opening file\n");
			exit(1);
		}
		offset = (vacant-1) * NAMELENGTH;
		printf("write your name(4): ");
		scanf("%s",name);
		if(lseek(infile,offset,SEEK_SET) == -1) {
			printf("Error on offset\n");
			exit(1);
		}
		else {
			name[NAMELENGTH-1]='\n';
			write(infile,name,NAMELENGTH);
		}
		close(infile);
		printf("Success!\n");
	}
}

void freeroom() {
	infile = -1;
	if (infile == -1 && (infile = open("residents", O_RDWR)) == -1) {
		printf("Error on opening file!\n");
		exit(1);
	}
	int empty_room;
	printf("freeroom? roomnum: ");
	scanf("%d",&empty_room);
	offset = (empty_room-1) * NAMELENGTH;
	if(lseek(infile,offset,SEEK_SET) == -1) {
		printf("error on offset\n");
		exit(1);
	}
	else {
		char *emptyname[NAMELENGTH];
		int i;
		for(i=0;i<NAMELENGTH;i++) {
			emptyname[i]=' ';
		}
		emptyname[NAMELENGTH-1] ='\n';
		write(infile,emptyname,NAMELENGTH);
	}
	close(infile);
	printf("Success!\n");
}

int main(){
	int j;
	char *getoccupier(int), *p;
	int option;
	int emptyroom;
	while(1)
	{
		printf("************************************\n");
		printf("WELCOME TO THE HOTEL\n");
		printf("option\n");
		printf("1. Getoccupier\n");
		printf("2. Find Free\n");
		printf("3. Free Room\n");
		printf("4. Add Guest\n");
		printf("5. Exit\n");
		printf("*************************************\n");
		printf("Choose Option : ");
		scanf("%d",&option);
		switch(option) {
			case 1:
				for(j=1; j <= NROOMS; j++){
					if(p = getoccupier(j)) {
						printf("Room %d, %s\n", j, p);
					}
					else {
						printf("Empty room %d\n", j);
					}
				}
				break;
			case 2:
				emptyroom = findfree();
				printf("lowest free room is %d\n", emptyroom);
				break;
			case 3:
				freeroom();
				break;
			case 4:
				addguest();
				break;
			case 5:
				exit(0);
				break;
			default :
				printf("Choose option!\n");
				break;
		}
	}
}
