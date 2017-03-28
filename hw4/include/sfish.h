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

#endif

void eval (char* cmd, char* shellPrompt);
void executable_command(char* cmd, char** tokens);
char* getPath(char* filename);
bool contains_slash(char* s);
void builtin_cd(char* path);
void builtin_pwd();
int fileExists(const char* file);
int builtin_command(char **argv);
int parseLine(char* cmd, char** argv);
void changePrompt(char* shellPrompt);
void printInfo();