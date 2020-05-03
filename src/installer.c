//
// Created by 010010 on 03/05/2020.
//

#include "header.h"
#include "ver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

const char *appName[] = {"Brew",
                         "libusb",
                         "SSHPass",
                         "iPhone Tunnel",
                         "testcom"};

const char *installComm[] = {"/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)",
                             "libusb",
                             "brew install https://raw.githubusercontent.com/kadwanev/bigboybrew/master/Library/Formula/sshpass.rb",
                             "curl -o iPhoneTunnel.zip https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/iphonetunnel-mac/iPhoneTunnel2.3-beta1.zip",
                             "ls -l /"};

//takes install arguement with App Name
install(char *install) {
    for (int i = 0; i < ARRAY_SIZE(appName); i++) {
        //printf("%s", install);
        //!strcmp(string, strings[i])
        if (!strcmp(install, appName[i])) {
            if (DEBUG == 1) {
                printf("Installing %s In Debug Mode\n", appName[i]);
                char *com1 = installComm[i];
                system(installComm[i]);
                exit(0);
            }
            if (DEBUG == 0) {
                printf("Installing %s\n", appName[i]);
                char *com1 = installComm[i];
                execlp(com1, com1, NULL);
            }
        }
    }
    printf("invalid id\n");
}

void installer(){
    printmenu();
    printf("Prerequesite Installer - Please Choose From The List Below:\n");
    for (int i = 0; i < ARRAY_SIZE(appName); i++) {
        printf("%d: %s\n", i+1, appName[i]);
    }
    char *installindexc;
    printf("> ");
    fgets(&installindexc, 50, stdin);
    printf("%s is the index", &installindexc);
    int installindex = atoi(&installindexc)-1;

    install(appName[installindex]);
}