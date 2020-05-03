//
// Created by 010010 on 03/05/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "header.h"
#include "ver.h"
#include "ios.h"

const char *menuItems[] = {"Prerequisites",
                         "Generate System & Data Partitions -> iOS",
                         "Create Mount Points & Mount -> iOS",
                         "Prepare RootFS DMG -> macOS",
                         "Virtual iOS Install -> macOS",
                         "Extract Virtual Install -> iOS",
                         "Patch Boot & Configure SEP -> iOS",
                         "Prepare Data Partition -> iOS",
                         "Cleanup -> macOS"};

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

    if (opt <= ARRAY_SIZE(menuItems)){
        switch(opt){
            case 1:
                installer();
                break;
            case 2:
                makevol_apfs("SystemB");
                makevol_apfs("DataB");
                break;
            default:
                printf("\nOption Not Found\n");
                sleep(2);
                main();
        }
    } else{
        printf("Invalid ID\n");
        exit(1);
    }
    return 0;
}