//
// Created by 010010 on 03/05/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "ver.h"

const char *menuItems[] = {"Prerequesites",
                         "Option 2",
                         "Option 3",
                         "Option 4"};

int main() {
    printmenu();
    printf("\nMain Menu:\n");
    printf("");
    for (int i = 0; i < ARRAY_SIZE(menuItems); i++) {
        printf("%d: %s\n", i+1, menuItems[i]);
    }
    char *optchar;
    printf("> ");
    fgets(&optchar, 50, stdin);
    int opt;
    opt = atoi(&optchar);

    if (opt < ARRAY_SIZE(menuItems)){
        switch(opt){
            case 1:
                installer();
                break;
            default:
                printf("Option Invalid");
                exit(1);
        }
    } else{
        printf("Invalid ID\n");
        exit(1);
    }
    return 0;
}