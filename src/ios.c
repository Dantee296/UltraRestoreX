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

//Pass a char array containing the volume name to create. The function will connect to the device usin
int ios_makevol_apfs(char *volname){
    char *com1 = "sshpass -p alpine ssh root@127.0.0.1 -p 2222 newfs_apfs -A -v";
    char com2[50];
    strcpy(com2, volname);
    char *com3 = "/dev/disk0s1 ";
    char commout[120];
    sprintf(commout, "%s %s %s", com1, com2, com3);

    //Exit code 1 if device cannot connect
    if (hasdeviceaccess() == 1){
        printf("Device Access Not Availible\n");
        return 1;
    }
    //Exit code 0 from hasdeviceaccess if can connect
    else if (hasdeviceaccess() == 0){
//        printf("Connection Successful\n");
        system(commout);
        sleep(2);
        return 0;
    }
}

int ios_mountdisk(char *diskid){
        //need code to determine what the final 2 disks are. Maybe use popen and read the output of grep.
        //or try to implement regex in the output..?
}

