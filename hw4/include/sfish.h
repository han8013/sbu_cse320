#ifndef SFISH_H
#define SFISH_H
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string.h>

#endif

void eval (char* cmd);
int parseLine(char* cmd, char** argv);
void changePrompt(char* shellPrompt);
void printHelpInfo();