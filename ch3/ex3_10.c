#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

#define BUFSIZE 512
struct stat sb;

void slowwatch(const char *, time_t);

int main(int argc, char* argv[]){
	time_t last_time;
	int filedes;
	if(argc != 3 || strcmp(argv[1],"lookout") != 0){
		fprintf(stderr, "format: lookout 'filename'\n");
		exit(1);
	}
	if((filedes = open(argv[2], O_WRONLY | O_CREAT, 0644)) == -1){
	       printf("Couldn't create %s\n",argv[2]);
	       exit(1);
	}
	else	
		close(filedes);
	if(stat(argv[2], &sb) == -1){
		fprintf(stderr, "lookout: couldn't stat %s\n",argv[2]);
		exit(1);
	}
	last_time = sb.st_mtime;
	for(;;){
		slowwatch(argv[2],last_time);
		sleep(5);
	}
}
void slowwatch(const char *name, time_t last){
        if(stat(name, &sb) == -1 || sb.st_mtime != last){
                printf("lookout: %s changed\n", name);
		char buff[BUFSIZE];
		int filedes;
		ssize_t nread;
		if((filedes = open(name, O_RDONLY)) == -1){
			printf("Couldn't open file\n");
			exit(1);
		}
		else{
			while((nread = read(filedes, buff, BUFSIZE-1)) > 0){
				buff[BUFSIZE]='\0';
                                printf("%s\n",buff);
			}
                        close(filedes);
		}
		printf("finished! press: (ctrl + c)\n");
                exit(1);
        }
	printf("Not Changed!\n");
}

