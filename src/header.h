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

#endif //SRC_HEADER_H

char *concatenate(const char *a, const char *b, const char *c);
install(const char *install);
void installer();
int hasdeviceaccess();
#define ARRAY_SIZE(x) ( sizeof(x) / sizeof((x)[0]) )
printmenu();