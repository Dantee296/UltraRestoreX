//
// Created by 010010 on 03/05/2020.
//

#include "header.h"
#include "ver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//need to move the order around ....
const char *appName[] = {"XCode CLI Tools", //working verified
                         "Brew", //needs fix
                         "LibUSB", //working verified
                         "SSHPass", //working verified
                         "iPhone Tunnel", //working verified , need manual setup
                         "libimobiledevice", //working verified
                         "ideviceinstaller", //working verified
                        };

const char *installComm[] = {"sudo xcode-select --install",
                             "echo Install from Brew.SH (this opt has an issue sorry! progress with\nthe rest of the prereqs\nAfter installing from https://brew.sh)",
                             "brew install libusb",
                             "brew install https://raw.githubusercontent.com/kadwanev/bigboybrew/master/Library/Formula/sshpass.rb",
                             "curl -o iPhoneTunnel.zip https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/iphonetunnel-mac/iPhoneTunnel2.3-beta1.zip",
                             "brew install libimobiledevice",
                             "brew install ideviceinstaller",
                             //add silencing after full tests...
                              };

//takes install arguement with App Name
install(const char *install) {
    for (int i = 0; i < ARRAY_SIZE(appName); i++) {
        //printf("%s", install);
        //!strcmp(string, strings[i])
        if (!strcmp(install, appName[i])) {
            if (DEBUG == 1) {
                printf("Installing %s In Debug Mode\n", appName[i]);
                system(installComm[i]);
                sleep(4); // user can read output before recursive func call.
                installer();
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
    char *installindexc;
    printf("> ");
    fgets(&installindexc, 50, stdin);
    int installindex = atoi(&installindexc)-1;
    if (installindex < ARRAY_SIZE(appName)){
        install(appName[installindex]);
    } else{
        printf("Invalid ID\n");
        exit(1);
    }
}