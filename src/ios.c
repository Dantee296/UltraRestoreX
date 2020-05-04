//
// Created by 010010 on 03/05/2020.
//

#include "header.h"
#include "ios.h"

//Pass a char array containing the volume name to create.
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

//Make a directory.
int ios_makedir(char *absolutedirectory){
    //Exit code 1 if device cannot connect
    if (hasdeviceaccess() == 1){
        printf("Device Access Not Availible\n");
        return 1;
    }
        //Exit code 0 from hasdeviceaccess if can connect
    else if (hasdeviceaccess() == 0){
//        printf("Connection Successful\n");
        char *com1 = ("mkdir");
        char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
        char dircheck[500];
        sprintf(dircheck, "%s %s %s", com1, absolutedirectory, com2);
        char *exitcodec = ios_runc(dircheck);
        int exitcode = atoi(exitcodec);
        if(exitcode==0){
            return 0;
        }
        else{
            return 1;
        }

        sleep(2);
        return 0;
    }
}

//remember escape key for pipes and strtok for removing newlines to prevent multiple commands being executed.
//returns char array of output. redirect output to silent and add '; echo $?' to return exit code.
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

//returns 0 on success 1 if dir already exists
int ios_checkdirexists(char *dir){
    char *com1 = ("mkdir");
    char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
    char dircheck[500];
    sprintf(dircheck, "%s %s %s", com1, dir, com2);
    return atoi(ios_runc(dircheck));
}

//return 1 on error and 0 on success pass diskid in the form disk0s1s3 and mntpnt as absolute directory
//reference such as /mnt1
int ios_mountdisk(char *diskid, char *mntpnt){
    if (hasdeviceaccess()==1){
        printf("Device Access Not Availible\n");
        return 1;
    }
    else if (hasdeviceaccess()==0){
        char commout[500];
        sprintf(commout, "mount_apfs %s %s >/dev/null 2>/dev/null \;echo $?", diskid, mntpnt);
        return atoi(ios_runc(commout));
    }
}