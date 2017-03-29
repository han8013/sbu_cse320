#include "sfish.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#define MAXARGS 128
char *shellPrompt_1;
char cwdbuf[512];
char *oldCwd;
char **pathList;
char **redireList;
char **pro1List;
char **pro2List;
char **pro3List;

void eval(char* cmd, char* shellPrompt){
	char *tokens[MAXARGS];  /* Argument list execve() */
	//int bg = 0; 			/* Should the job run in bg or fg? */
	//pid_t pid;			    /* Process id */
	shellPrompt_1 = shellPrompt;
	char *cmd_copy = strdup(cmd);
	parseLine(cmd_copy,tokens);
	if (tokens[0] == NULL)
		return; /* Ignore empty lines */
	if (builtin_command(tokens)==0) {
		executable_command(cmd,tokens);
	}

	return;
}

void executable_command(char* cmd, char** tokens){

	if (contains_redirection(cmd)){
		redirection(cmd,tokens);
	}
	else{
		execute(cmd,tokens);
	}
}

void execute(char* cmd, char** tokens){
	/* check contain slash first*/
	char *path = strdup(cmd);
	if (contains_slash(tokens[0])){
		if (fileExists(tokens[0])==0){
			printf("No such file or directory%s\n", tokens[0]);
            return;
		}
	}
	else{
		path = getPath(tokens[0]);
		free(pathList);
        if(path == NULL) {
            printf("command not found: %s\n", tokens[0]);
            return;
        }
	}
	/* execute cmd */
	/* fork first */
	pid_t pid_execu;
	int child_status;
	if ((pid_execu=fork())==0){
		execv(path,tokens);
	}
	else{
        wait(&child_status);
	}
}

void redirection(char* cmd, char** tokens){
	/* fork first */
	pid_t pid_redire;
	int child_status;
	int findChar;
	if ((pid_redire=fork())==0){
		printf("%s\n", cmd);

		if ((findChar = findCharIndex(cmd,'<'))!=-1)
		{
			printf("%s\n", "redirection input");

			// Pro1 < input
			redireList = malloc(sizeof(char*));
			char *delim = "<";
			redireList = parsePathevn(cmd,redireList,delim);
			char* progArgv = redireList[0];
			char* inputFile = redireList[1];
			char *space = " ";
			pro1List = malloc(sizeof(char*));

			pro1List = parsePathevn(progArgv,pro1List,space);
			printf("%s\n", inputFile);
			inputFile = inputFile+1;
			printf("%s\n", inputFile);

			builtin_pwd();
			char * temp_PATH = strcat(cwdbuf,"/");
			inputFile = strcat(temp_PATH,inputFile);

			printf("PATH IS %s\n", inputFile);
    		int input_fd = open(inputFile, O_RDONLY);
    		if (input_fd<0){
		        // exit process on invalid file
		        fprintf(stderr,"sfish: %s No such file or directory\n", inputFile);
		        exit(EXIT_SUCCESS);
            }
    		dup2(input_fd, STDIN_FILENO);
    		int fc;
    		if ((fc = close(input_fd))<0)
    		{
		        fprintf(stderr,"sfish: %s File close error\n", inputFile);
		        exit(EXIT_SUCCESS);
		    }
    		execute(progArgv,pro1List);
    		free(redireList);
    		free(pro1List);

			if ((findChar = findCharIndex(cmd,'>'))!=-1) // Pro1 < input > output
			{
				/* code */
			}
		}
		else if ((findChar = findCharIndex(cmd,'>'))!=-1) // Pro1 > output
		{
			/* code */
		}
		else if ((findChar = findCharIndex(cmd,'|'))!=-1)
		{
			/* code */
		}

	}
	else{
        wait(&child_status);
	}
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
			printf("%s\n", "found path");
			printf("%s\n", tempPath);
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
	printf("%s\n", delim);
	char *p = strtok (PATH, delim);
	int n_spaces = 0,i;

	/* split string and append tokens to 'res' */
	while (p) {
	  pathList = realloc (pathList, sizeof (char*) * ++n_spaces);
	  if (pathList == NULL)
	    exit (-1); /* memory allocation failed */
	  pathList[n_spaces-1] = p;
	  p = strtok (NULL, delim);
	}

	/* realloc one extra element for the last NULL */
	pathList = realloc (pathList, sizeof (char*) * (n_spaces+1));
	pathList[n_spaces] = 0;

	// /* print the result */
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
		builtin_pwd();
		printf("%s\n", cwdbuf);
		return 1;
	}
	return 0; /* Not a builtin command */
}

void builtin_pwd(){
	pid_t pid;
	int child_status;
	if ((pid = fork()) == 0){
		getcwd(cwdbuf,512);
	}
	else{
        wait(&child_status);
	}
}

void builtin_cd(char* path){
	/* change to home path*/
    if(path == NULL || strcmp(path, "")==0) {
		getcwd(cwdbuf,512);
        oldCwd = realloc(oldCwd, sizeof(cwdbuf) + 1);
        strcpy(oldCwd, cwdbuf);
        chdir(getenv("HOME"));
    }
    /* change to previous working path */
	else if (strcmp(path, "-")==0){
        if(oldCwd == NULL) {
            printf("Previous working directory not set.\n");
        }
        else {
			getcwd(cwdbuf,512);
            chdir(oldCwd);
            oldCwd = realloc(oldCwd, sizeof(cwdbuf) + 1);
            strcpy(oldCwd, cwdbuf);
        }
    }
    /* change to exists path*/
	else{
		getcwd(cwdbuf,512);
        oldCwd = realloc(oldCwd, sizeof(cwdbuf) + 1);
        strcpy(oldCwd, cwdbuf);
		if ((chdir(path)) == -1){
	        printf("No such file or directory\n");
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
	getcwd(cwdbuf,512);
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
