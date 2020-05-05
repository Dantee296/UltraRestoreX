//
// Created by 010010 on 03/05/2020.
//

#ifndef SRC_HEADER_H
#define SRC_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>
#include "ver.h"

#include <fcntl.h>
#include <unistd.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#include <copyfile.h>
#else
#include <sys/sendfile.h>
#endif
#endif //SRC_HEADER_H

char *concatenate(const char *a, const char *b, const char *c);
install(const char *install);
void installer();
int hasdeviceaccess();
#define ARRAY_SIZE(x) ( sizeof(x) / sizeof((x)[0]) )
printmenu();
int macOS_runc(char *command);
char *macos_run_e(char *command);
int OSCopyFile(const char* source, const char* destination);