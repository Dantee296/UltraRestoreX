//
// Created by 010010 on 03/05/2020.
//

#include "header.h"
#include "ver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Prerequisite Applications. Install command in array directly below in same order.
//Each option will generate a new menu listing.
const char *appName[] = {"Brew",
                         "LibUSB",
                         "SSHPass",
                         "iPhone Tunnel",
                         "ls"};


//Referenced in system call to exec the correct installer command.
const char *installComm[] = {"/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)",
                             "brew install libusb",
                             "brew install https://raw.githubusercontent.com/kadwanev/bigboybrew/master/Library/Formula/sshpass.rb",
                             "curl -o iPhoneTunnel.zip https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/iphonetunnel-mac/iPhoneTunnel2.3-beta1.zip",
                             "ls -l /"};

//takes install arguement as a char array w/App Name from appName array.
install(const char *install) {
    for (int i = 0; i < ARRAY_SIZE(appName); i++) {
        //printf("%s", install);
        //!strcmp(string, strings[i])
        if (!strcmp(install, appName[i])) {
            if (DEBUG) {
                printf("Installing %s In Debug Mode\n", appName[i]);
                system(installComm[i]);
                exit(0);
            }
        }
    }
}

void installer(){
    printmenu();
    printf("Prerequesite Installer - Please Choose From The List Below:\n");
    for (int i = 0; i < ARRAY_SIZE(appName); i++) {
        printf("%d: %s\n", i+1, appName[i]);
    }
    //Used with fgets to capture user input.
    char *installindexc;
    printf("> ");
    fgets(&installindexc, 50, stdin);
    //installindex holds int value of the users input -1 (user can type 1 for technically index 0)
    int installindex = atoi(&installindexc)-1;
    if (installindex < ARRAY_SIZE(appName)){
        install(appName[installindex]);
    } else{
        printf("Invalid ID\n");
        exit(1);
    }
}