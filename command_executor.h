#pragma once
#include <errno.h>

void createCommandProcess(char** arguments);
void createFileProcess(char** arguments, char* fileName);
char** prepareArguments(char** arguments);
