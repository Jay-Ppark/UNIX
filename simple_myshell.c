#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];

void fatal(char *str){
	perror(str);
	exit(1);
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST){	
  int i = 0;
  int numtokens = 0;
  char *snew = NULL;

  if( (s==NULL) || (delimiters==NULL) ) return -1;

  snew = s + strspn(s, delimiters);	/* Skip delimiters */
  if( (list[numtokens]=strtok(snew, delimiters)) == NULL )
    return numtokens;
	
  numtokens = 1;
  
  while(1){
     if( (list[numtokens]=strtok(NULL, delimiters)) == NULL)
	break;
     if(numtokens == (MAX_LIST-1)) return -1;
     numtokens++;
  }
  return numtokens;
}

int main(int argc, char**argv){
  int i=0;
  pid_t pid;
  int is_background;
  int arguments_cnt;
  int status;
  while (1) {
  	fputs(prompt, stdout);
	fgets(cmdline, BUFSIZ, stdin);
	cmdline[strlen(cmdline) -1] = '\0';

	is_background = 0;
	arguments_cnt = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
	if (strcmp(cmdvector[arguments_cnt - 1], "&") == 0) {
		is_background = 1;
	}
	if (strcmp(cmdvector[0], "cd") == 0) {
		if (chdir(cmdvector[1]) == -1)
			printf("cd error!\n");
		continue;
	}
	else if (strcmp(cmdvector[0], "exit") == 0) {
		break;
	}


	switch(pid=fork()){
	case 0:
		if (is_background == 1)
			argv[arguments_cnt - 1] = ' ';
		execvp(cmdvector[0], cmdvector);
		fatal("main()");
	case -1:
  		fatal("main()");
	default:
		if (is_background == 0)
			waitpid(pid, &status, 0;)
		else
			sleep(1);
	}
  }
  return 0;
}
