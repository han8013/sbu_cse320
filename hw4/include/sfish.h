#ifndef SFISH_H
#define SFISH_H
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#endif


int eval (char* cmd, char* shellPrompt);
void executable_command(char* cmd, char** tokens);
void execute(char* cmd, char** tokens);

void alarmHandler(int s);
void builtin_alarm(char* s);
void killHandler();
void blockHandler();
void sig_child();


pid_t Fork();
void spawn_proc (int in, int out, char** command);
void fork_pipes (int n, char** commands[]);

char* get_filePath(char* filename);
void Close(int fd, char* File);

bool contains_redirection(char* s);
void redirection(char* cmd, char** tokens);
int findCharIndex(char* string, char e);

char* getPath(char* filename);
char* concatPath(char* path, char* filename);
bool contains_slash(char* s);

void builtin_cd(char* path);
void builtin_pwd(char* cwdbuf);
int fileExists(const char* file);
int builtin_command(char **argv);

char** parsePathevn(char *PATH, char** pathList, char* delim);
int parseLine(char* cmd, char** argv);
void changePrompt(char* shellPrompt);
void printInfo();

