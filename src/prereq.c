//
// Created by 010010 on 03/05/2020.
//

#include "header.h"

printmenu(){
    system("clear");
    printf("-----------------------\n");
    printf ("Welcome To iBootX Version %d\n", VER);
    printf ("Debug mode is %d\n", DEBUG);
    printf("-----------------------\n");
}

//verify device access function
int hasdeviceaccess(){
    //if device access = false
    //      return 1

    //if device access = true
    //      return 0

    char *ping = "nc -z 127.0.0.1 2222 >/dev/null 2>/dev/null";

    FILE * shell;
    shell = popen(ping, "r");
    int r = pclose(shell);
    if(WEXITSTATUS(r)==0)
    {
        return 0;
    }
    else if(WEXITSTATUS(r)==1)
    {
        return 1;
    } else {
        //unexpected
        exit(1);
    }
}