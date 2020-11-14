#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];
int status;

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

void change_directory(char* dirN) {
	if(chdir(dirN) == -1 ) {
		printf("cd ERROR!\n");
	}
}
void cmd(int argc) {
	pid_t pid;
	switch(pid = fork()){
		case 0:
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execvp(cmdvector[0], cmdvector);
			fatal("error child!");
			exit(1);
		case -1:
			fatal("error!");
			exit(0);
		default:
			waitpid(pid, &status, WUNTRACED);
			if(WIFSIGNALED(status)){
				printf("\n");
			}
	}
}

void cmd_back(int argc) {
	struct sigaction ss;
	pid_t pid;
	switch(pid = fork()) {
		case 0:
			execvp(cmdvector[0], cmdvector);
			fatal("back_child error!");
			exit(1);
		case -1: 
			fatal("background_error!");
			exit(0);
		default:
			sigemptyset(&ss.sa_mask);
			ss.sa_flags = SA_NOCLDSTOP;
			ss.sa_handler = SIG_IGN;
			sigaction(SIGCHLD, &ss, NULL);
	}
}
int main(int argc, char**argv){
  int i=0;
  pid_t pid;
  int arguments_cnt;
  while (1) {
	
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
  	fputs(prompt, stdout);
	if(strcmp(fgets(cmdline, BUFSIZ, stdin),"\n")==0)
		continue;
	cmdline[strlen(cmdline) -1] = '\0';

	arguments_cnt = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

	if (strcmp(cmdvector[0], "cd") == 0) {
		change_directory(cmdvector[1]);
		continue;
	}
	else if (strcmp(cmdvector[0], "exit") == 0) {
		break;
	}

	if(strcmp(cmdvector[arguments_cnt - 1], "&") == 0) {
		cmdvector[arguments_cnt - 1] = '\0';
		cmd_back(arguments_cnt);
	}
	else {
		cmd(arguments_cnt);
	}
  }
  return 0;
}
