#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#define MAX_CMD_ARG 20
#define BUFSIZ 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];
int status;
int arguments_cnt = 0;

int pipe_arg_cnt;
char* pipe_arg[MAX_CMD_ARG];

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
void erase_vac(int cnt) {
	int j = 0;
	for(int i=0;i<arguments_cnt;i++){
		if(cmdvector[i] != "\0"){
			cmdvector[j] = cmdvector[i];
			j++;
		}
	}
	arguments_cnt -= cnt;
}
void redirect() {
	int temp = 0;
	int fd;
	for(int i=0;i<arguments_cnt;i++) {
		if(strcmp(cmdvector[i], ">") == 0) {
			if((fd = open(cmdvector[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
				fatal("redirection fail");
				break;
			}
			dup2(fd,1);
			cmdvector[i] = '\0';
			temp++;
			close(fd);
		}
		else if(strcmp(cmdvector[i], "<") == 0) {
			if((fd = open(cmdvector[i + 1], O_RDONLY | O_CREAT, 0777)) == -1) {
				fatal("redirection fail");
				break;
			}
			dup2(fd,0);
			cmdvector[i]='\0';
			temp++;
			close(fd);
		}
	}

	erase_vac(temp);
}
void do_pipe() {
	int i = 0;
	int p[2], pid;
	pipe_arg_cnt = makelist(cmdline, "|", pipe_arg, MAX_CMD_ARG);
	for(i=0;i<pipe_arg_cnt-1;i++){
		if(pipe(p) < 0)
			fatal("pipe fail");
		switch(pid = fork()) {
			case 0:
				close(p[0]);
				dup2(p[1], 1);
				arguments_cnt = makelist(pipe_arg[i], " \t", cmdvector, MAX_CMD_ARG);
				redirect();
				execvp(cmdvector[0], cmdvector);
				fatal("pipe fork fail(child)");
			case -1:
				fatal("pipe fork fail");
			default :
				close(p[1]);
				dup2(p[0], 0);
		}
	}
	arguments_cnt = makelist(pipe_arg[0], " \t",cmdvector, MAX_CMD_ARG);
	redirect();
	execvp(cmdvector[0], cmdvector);
	fatal("error");
}
void cmd(int argc) {
	pid_t pid;
	switch(pid = fork()){
		case 0:
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			do_pipe();
			fatal("error child!");
		case -1:
			fatal("error!");
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
			do_pipe();
			fatal("back_child error!");
		case -1: 
			fatal("background_error!");
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
  while (1) {
	char cmd_tmp[BUFSIZ];
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
  	fputs(prompt, stdout);
	if(strcmp(fgets(cmdline, BUFSIZ, stdin),"\n")==0)
		continue;
	memcpy(cmd_tmp, cmdline, sizeof(cmdline));
	cmdline[strlen(cmdline) -1] = '\0';

	arguments_cnt = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

	if (strcmp(cmdvector[0], "cd") == 0) {
		change_directory(cmdvector[1]);
		continue;
	}
	else if (strcmp(cmdvector[0], "exit") == 0) {
		break;
	}

	memcpy(cmdline,cmd_tmp, sizeof(cmd_tmp));
	cmdline[strlen(cmdline)-1] = '\0';

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
