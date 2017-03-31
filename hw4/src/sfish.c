#include "sfish.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#define MAXARGS 128
char *shellPrompt_1;
char cwdbuf[1024];
char *oldCwd;
char **pathList;
char **redireList;
char **pro1_List;
char **pro2_List;
char **pro3_List;

int eval(char* cmd, char* shellPrompt){
	char *tokens[MAXARGS];  /* Argument list execve() */
	//pid_t pid;			    /* Process id */
	shellPrompt_1 = shellPrompt;
	char *cmd_copy = strdup(cmd);
	parseLine(cmd_copy,tokens);
	if (tokens[0] == NULL)
		return 0; /* Ignore empty lines */

	if (strcmp(tokens[0],"exit")==0){
		return -1;
	}

	if (contains_redirection(cmd)) {
		redirection(cmd,tokens);
	}
	else{
		if (builtin_command(tokens)==0)
		{
			execute(cmd,tokens);
		}
	}
	printf("%s\n", "out111");
	return 0;
}

void execute(char* cmd, char** tokens){
	/* check contain slash first*/
	char *path = strdup(cmd);
	if (contains_slash(tokens[0])){
		if (fileExists(tokens[0])==0){
			fprintf(stderr,"No such file or directory%s\n", tokens[0]);
            return;
		}
	}
	else{
		path = getPath(tokens[0]);
		free(pathList);
        if(path == NULL) {
            fprintf(stderr,"command not found: %s\n", tokens[0]);
            return;
        }
	}
	/* execute cmd */
	/* fork first */
	pid_t pid_execu;
	int child_status;
	pid_execu=Fork();
	if ((pid_execu)==0){
		int execv_return = execv(path,tokens);
		if (execv_return < 0)
		{
			fprintf(stderr, "%s\n", "command not found");
			exit(1);
		}
	}
	else{
        wait(&child_status);
	}
}

void redirection(char* cmd, char** tokens){

		int findChar;
		// printf("%s\n", cmd);

		if ((findChar = findCharIndex(cmd,'<'))!=-1)
		{

			if ((findChar = findCharIndex(cmd,'>'))!=-1) // Pro1 < input > output
			{
				printf("%s\n", "redirection input & output");


















			}
			else
			{
				// printf("%s\n", "redirection input");

				// Pro1 < input
				redireList = malloc(sizeof(char*));
				char *delim = "<";
				redireList = parsePathevn(cmd,redireList,delim);
				char* progArgv = redireList[0];
				char* inputFile = redireList[1];
				char *space = " ";
				pro1_List = malloc(sizeof(char*));
				pro1_List = parsePathevn(progArgv,pro1_List,space);

				inputFile = get_filePath(inputFile);
	    		int input_fd = open(inputFile, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	    		if (input_fd<0){
			        // exit process on invalid file
			        fprintf(stderr,"sfish: %s Open file error Or No such file or directory\n", inputFile);
			        exit(1);
	            }

				pid_t pi1;
				int child_status1;
				pi1 = Fork();
				if (pi1 == 0){
					dup2(input_fd, STDIN_FILENO);
					Close(input_fd,inputFile);
					// dup2(output_fd,1);
					// printf("redirection program path:%s\n", pro1List[0]);

					char* path = getPath(pro1_List[0]);
					// printf("redirection program path:%s\n", path);
					int execv_return = execv(path, pro1_List);
					if (execv_return<0)
					{
						fprintf(stderr, "%s\n", "command not found");
						exit(1);
					}
					// dup2(input_fd,0);
					//execute(progArgv,pro1List);
					fprintf(stderr,"%s\n", "after close");

	    		}
	    		else{
	    			wait(&child_status1);
	    		}
	    		free(redireList);
		    	free(pro1_List);
			}


		}
		else if ((findChar = findCharIndex(cmd,'>'))!=-1) // Pro1 > output
		{
			// printf("%s\n", "redirection output");

			// Pro1 < input
			redireList = malloc(sizeof(char*));
			char *delim = ">";
			redireList = parsePathevn(cmd,redireList,delim);
			char* progArgv = redireList[0];
			char* outputFile = redireList[1];
			char *space = " ";
			pro1_List = malloc(sizeof(char*));
			pro1_List = parsePathevn(progArgv,pro1_List,space);
			outputFile = get_filePath(outputFile);

    		int output_fd = open(outputFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    		if (output_fd<0){
		        // exit process on invalid file
		        fprintf(stderr,"sfish: %s Open file error Or No such file or directory\n", outputFile);
		        // exit(1);
            }

			pid_t pi1;
			int child_status1;
			pi1 = Fork();
			if (pi1 == 0){
				dup2(output_fd, STDOUT_FILENO);
				Close(output_fd,outputFile);
				// dup2(output_fd,1);
				char* path = getPath(pro1_List[0]);
				int execv_return = execv(path, pro1_List);
				printf("%s\n", "after close 1");

				if (execv_return<0)
				{
					fprintf(stderr, "%s\n", "command not found");
					exit(1);
				}

				printf("%s\n", "after close 2");

    		}
    		else{
    			wait(&child_status1);
    		}

			printf("%s\n", "free");

			free(redireList);
    		free(pro1_List);

		}
		else if ((findChar = findCharIndex(cmd,'|'))!=-1)
		{
			printf("%s\n", "pipe");
			redireList = malloc(sizeof(char*));
			char *delim = "|";
			redireList = parsePathevn(cmd,redireList,delim);
			int size = 0;
			while(redireList[size]!=NULL){
				size ++;
			}
			printf("%d\n", size);


				char* prog1 = redireList[0];
				char* prog2 = redireList[1];
				char* prog3 = redireList[2];

				pro1_List = malloc(sizeof(char*));
				pro2_List = malloc(sizeof(char*));
				pro3_List = malloc(sizeof(char*));

				char *space = " ";
				pro1_List = parsePathevn(prog1,pro1_List,space);
				pro2_List = parsePathevn(prog2,pro2_List,space);
				pro3_List = parsePathevn(prog3,pro3_List,space);


			char** commands[] = {pro1_List,pro2_List,pro3_List};
			pid_t pid;
			int child_status_pipe;
			if ((pid=fork())==0){
				fork_pipes(size,commands);
				exit(0);
			}
			else{
				wait(&child_status_pipe);
			}
		}


}

void fork_pipes (int n, char** commands[]){
  int i;
  int in, fd [2];

  in = 0;
  for (i = 0; i < n - 1; ++i)
    {
      pipe (fd);
      spawn_proc (in, fd [1], commands[i]);
      close (fd [1]);
      in = fd [0];
    }
  if (in != 0)
	dup2 (in, 0);
	char * path = getPath(commands [i][0]);
	execv(path, commands [i]);

	// pid_t pid;
	// int child_status;
 //  if ((pid = fork())==0){
 //  }
 //  else{
 //  	wait(&child_status);
 //  	printf("%s\n", "parent");
 //  }
	// return execv(path, commands [i]);
}

void spawn_proc (int in, int out, char** command){
  pid_t pid;
  int child_status;
  if ((pid = fork ()) == 0){
      if (in != 0)
        {
          dup2 (in, 0);
          close (in);
        }
      if (out != 1)
        {
          dup2 (out, 1);
          close (out);
        }

      char * path = getPath(command[0]);
      execv(path, command);
    }
    else{
  		wait(&child_status);
  		printf("%s\n", "parent-1");

    }
    // return 0;

}



pid_t Fork() {
    pid_t pid;
    if((pid = fork()) < 0) {
        fprintf(stderr,"Failed to create process.\n");
    }
    return pid;
}

void backFd(int input_fd, int output_fd){
	if (input_fd != 0) {
        dup2 (input_fd, 0);
        close (input_fd);
    }
    if (output_fd != 1) {
        dup2 (output_fd, 1);
        close (output_fd);
    }
}

void Close(int fd, char* File){
	int fc;
	printf("%s\n", "close");
	if ((fc = close(fd))<0)
	{
	    fprintf(stderr,"sfish: %s File close error\n", File);
	}
}

char* get_filePath(char* filename){
	filename = filename+1; // trim first space of filename

	char *path = strdup(filename);
	if (!contains_slash(path)){
		builtin_pwd(cwdbuf);
		// printf("in the get_filePath: %s\n", cwdbuf);
		char * temp_PATH = strcat(cwdbuf,"/");
		path = strcat(temp_PATH,path);
	}
	printf("%s\n", path);
	return path;
}



int findCharIndex(char* s, char e){
    int len = strlen(s);
    int i;
    for(i = 0; i < len; ++i){
        if(*s == e)
            return i;
        s = s + 1;
    }
    return -1;
}

bool contains_redirection(char* s){
    int len = strlen(s);
    int i;
    for(i = 0; i < len; ++i){
        if(*s == '>' || *s == '<' || *s == '|')
            return true;
        s = s + 1;
    }
    return false;
}

char* getPath(char* filename){
	char* envir_PATH = strdup(getenv("PATH"));
	pathList = calloc((sizeof(char*)),sizeof(char));
	// printf("%s\n", envir_PATH);
	char *delim = ":";
	pathList = parsePathevn(envir_PATH,pathList,delim);
	int i = 0;
	while(pathList[i]!=NULL){
		char* tempPath = concatPath(pathList[i],filename);
		if(fileExists(tempPath)==1){
			// printf("%s\n", "found path");
			// printf("%s\n", tempPath);
			return tempPath;
		}
		i++;
	}
	return NULL;
}

char* concatPath(char* path, char* filename) {
	char *c = strdup(path);
	strcat(c,"/");
	strcat(c,filename);
    *(c+strlen(c)) = '\0';
    return c;

}

int fileExists(const char* filename){
	// printf(">>>>>>>>%s\n", filename);
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1; //found
    else // -1
        return 0; //not found
}

char** parsePathevn(char *PATH, char** pathList, char* delim){
	// printf("%s\n", delim);
	char *p = strtok (PATH, delim);
	int n_spaces = 0;

	/* split string and append tokens to 'res' */
	while (p) {
	  pathList = realloc (pathList, sizeof (char*) * ++n_spaces);
	  if (pathList == NULL)
	  	fprintf(stderr,"%s\n", "Not found file or directory");
	    // exit (-1);  memory allocation failed
	  pathList[n_spaces-1] = p;
	  p = strtok (NULL, delim);
	}

	/* realloc one extra element for the last NULL */
	pathList = realloc (pathList, sizeof (char*) * (n_spaces+1));
	pathList[n_spaces] = 0;

	// /* print the result */
	int i;
	for (i = 0; i < (n_spaces+1); ++i)
	  printf ("parse>[%d] = %s\n", i, pathList[i]);

	return pathList;

}

bool contains_slash(char* s){
    int len = strlen(s);
    int i;
    for(i = 0; i < len; ++i){
        if(*s == '/')
            return true;
        s = s + 1;
    }
    return false;
}

int builtin_command(char **argv){
	if (strcmp(argv[0], "help")==0) {
		printInfo();
		return 1;
	}
	else if (strcmp(argv[0], "cd")==0){
		char* path = argv[1];
		builtin_cd(path);
		changePrompt(shellPrompt_1);
		return 1;
	}
	else if (strcmp(argv[0], "pwd")==0){
		builtin_pwd(cwdbuf);
		// getcwd(cwdbuf,1024);
		printf("%s\n", cwdbuf);
		return 1;
	}
	return 0; /* Not a builtin command */
}

void builtin_pwd(char* cwdbuf){
	pid_t pid;
	int child_status;
	if ((pid = Fork()) == 0){
		getcwd(cwdbuf,1024);
	}
	else{
        wait(&child_status);
        exit(0);
	}
}

void builtin_cd(char* path){
	/* change to home path*/
    if(path == NULL || strcmp(path, "")==0) {
		builtin_pwd(cwdbuf);
        oldCwd = realloc(oldCwd, sizeof(cwdbuf) + 1);
        strcpy(oldCwd, cwdbuf);
        chdir(getenv("HOME"));
    }
    /* change to previous working path */
	else if (strcmp(path, "-")==0){
        if(oldCwd == NULL) {
            fprintf(stderr,"Previous working directory not set.\n");
        }
        else {
		builtin_pwd(cwdbuf);
            chdir(oldCwd);
            oldCwd = realloc(oldCwd, sizeof(cwdbuf) + 1);
            strcpy(oldCwd, cwdbuf);
        }
    }
    /* change to exists path*/
	else{
		builtin_pwd(cwdbuf);
        oldCwd = realloc(oldCwd, sizeof(cwdbuf) + 1);
        strcpy(oldCwd, cwdbuf);
		if ((chdir(path)) == -1){
	        fprintf(stderr,"No such file or directory\n");
		}
	}
}

int parseLine(char *buf, char **argv){
	char *delim; /* Points to first space delimiter */
	int argc; 	 /* Number of args */
	int bg;      /* Background job? */

	buf[strlen(buf)] = ' '; /* Replace trailing ’\n’ with space */
	while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	 	buf++;

	 /* Build the argv list */
	 argc = 0;
	 while ((delim = strchr(buf, ' '))) {
		 argv[argc++] = buf;
		 *delim = '\0';
		 buf = delim + 1;
		 while (*buf && (*buf == ' ')) /* Ignore spaces */
		 	buf++;
	 }
	 argv[argc] = NULL;

	 if (argc == 0) /* Ignore blank line */
	 	return 1;

	 /* Should the job run in the background? */
	 if ((bg = (*argv[argc-1] == '&')) != 0)
	 	argv[--argc] = NULL;

	 return bg;
 }

void changePrompt(char* shellPrompt){
	char symbol[10];
	strcpy(shellPrompt,"<xinhan> : ");
		builtin_pwd(cwdbuf);
	strcpy(symbol,"<");
	strcat(shellPrompt,symbol);
	strcat(shellPrompt,cwdbuf);
	strcpy(symbol,">");
	strcat(shellPrompt,symbol);
	strcpy(symbol," $ ");
	strcat(shellPrompt,symbol);
}

void printInfo(){
	printf("%s\n","================================================================================");
	printf("%s\n","help: Print a list of all builtins and their basic usage.");
	printf("%s\n","exit: Exits the shell.");
	printf("%s\n", "cd: Changes the current working directory of the shell.");
	printf("%s\n", "     * cd - should change the working directory to the last directory the user was in.");
	printf("%s\n", "     * cd with no arguments should go to the user's home directory.");
	printf("%s\n", "     * cd . should change the working directory itself.");
	printf("%s\n", "     * cd .. should change the working directory to the parent directory in the directory hierarchy.");
	printf("%s\n", "pwd: prints the absolute path of the current working directory.");
	printf("%s\n","================================================================================");

}
