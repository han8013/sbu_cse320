#include "sfish.h"

#define MAXARGS 128

void eval(char* cmd){

	char *tokens[MAXARGS];
	parseLine(cmd,tokens);
	printf ("tokens[%d] = %s\n", 0, tokens[0]);
	printf ("tokens[%d] = %s\n", 1, tokens[1]);
	printf ("tokens[%d] = %s\n", 2, tokens[2]);


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

// int parseLine(char* cmd, char** argv){
// 	char **res  = NULL;
// 	char *p = strtok (cmd, " ");
// 	int n_spaces = 0;
// 	/* split string and append tokens to 'res' */
// 	while (p) {
// 	  res = realloc (res, sizeof (char*) * ++n_spaces);
// 	  if (res == NULL)
// 	    exit (-1); /* memory allocation failed */
// 	  res[n_spaces-1] = p;
// 	  p = strtok (NULL, " ");
// 	}

// 	/* realloc one extra element for the last NULL */
// 	res = realloc (res, sizeof (char*) * (n_spaces+1));
// 	res[n_spaces] = 0;

// 	return 0;
// }

void changePrompt(char* shellPrompt){
	char cwd [512];
	char symbol[10];
	strcpy(shellPrompt,"<xinhan> : ");
	getcwd(cwd,512);
	strcpy(symbol,"<");
	strcat(shellPrompt,symbol);
	strcat(shellPrompt,cwd);
	strcpy(symbol,">");
	strcat(shellPrompt,symbol);
	strcpy(symbol," $ ");
	strcat(shellPrompt,symbol);
}

