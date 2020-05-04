//
// Created by 010010 on 03/05/2020.
//

#include "header.h"
#include "ios.h"

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

int ios_makedir(char *absolutedirectory){
    char *com1 = "sshpass -p alpine ssh root@127.0.0.1 -p 2222 mkdir";
    char com2[100];
    strcpy(com2, absolutedirectory);
    char commout[200];
    sprintf(commout, "%s %s", com1, com2);

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


//currently only returns one line output command.. do not use pipes..
char *ios_runc(char *command){
    if (hasdeviceaccess()==1){
        printf("Device Access Not Availible\n");
        return 1;
    }
    else if (hasdeviceaccess() == 0){
        char *com1 = "sshpass -p alpine ssh root@127.0.0.1 -p 2222";
        char com2[800];
        strcpy(com2, command);
        char commout[800];
        sprintf(commout, "%s %s", com1, com2);
        char *com = commout;
        char out[2048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        return out;
    }
}

int ios_mountdisk(char *diskid, char *mntpnt){
    if (hasdeviceaccess()==1){
        printf("Device Access Not Availible\n");
        return 1;
    }
    else if (hasdeviceaccess()==0){
        char commout[500];
        sprintf(commout, "mount_apfs %s %s", diskid, mntpnt);
        ios_runc(commout);
        return 0;
    }


}