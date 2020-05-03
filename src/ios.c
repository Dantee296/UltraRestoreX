//
// Created by 010010 on 03/05/2020.
//

#include "ios.h"
#include "header.h"
#include "ver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>


void makevol_apfs(char *volname){
    char *com1 = "sshpass -p alpine ssh root@127.0.0.1 -p 2222 newfs_apfs -A -v";
    char com2[50];
    strcpy(com2, volname);
    char *com3 = "/dev/disk0s1 ";
    char commout[120];
    sprintf(commout, "%s %s %s", com1, com2, com3);

    //Exit code 1 if device cannot connect
    if (hasdeviceaccess() == 1){
        printf("Device Access Not Availible\nMake sure iPhone Tunnel is configured\n");
        exit(1);
    }
    //Exit code 0 from hasdeviceaccess if can connect
    else if (hasdeviceaccess() == 0){
        printf("Connection Successful\n");
        system(commout);
    }
}