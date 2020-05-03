//
// Created by 010010 on 03/05/2020.
//

#include "header.h"
#include "ver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *concatenate(const char *a, const char *b, const char *c) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    size_t clen = strlen(c);
    char *res = malloc(alen + blen + clen + 1);
    if (res) {
        memcpy(res, a, alen);
        memcpy(res + alen, b, blen);
        memcpy(res + alen + blen, c, clen + 1);
    }
    return res;
}

printmenu(){
    system("clear");
    printf("-----------------------\n");
    printf ("Welcome To iBootX Version %d\n", VER);
    printf ("Debug mode is %d\n", DEBUG);
    printf("-----------------------\n");
}