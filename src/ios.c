//
// Created by 010010 on 03/05/2020.
//

#include "header.h"
#include "ios.h"

//Pass a char array containing the volume name to create.
int ios_makevol_apfs(char *volname){
    char *com1 = "sshpass -p alpine ssh root@127.0.0.1 -p 2222 newfs_apfs -A -v";
//    char com2[50];
//    strcpy(com2, volname);
    char *com3 = "/dev/disk0s1 ";
    char commout[180]; //need defined array size.. large enough to hold input.
    sprintf(commout, "%s %s %s", com1, volname, com3);
    //Exit code 1 if device cannot connect
    if (hasdeviceaccess() == FALSE){
        printf("Device Access Not Availible\n");
        return FALSE;
    }
    //Exit code 0 from hasdeviceaccess if can connect
    else if (hasdeviceaccess() == TRUE){
//        printf("Connection Successful\n");
        system(commout);
        sleep(2); //sleep for 2secs to allow command to exec.
        return TRUE;
    }
}

//Make a directory.
int ios_makedir(char *absolutedirectory){
    //Exit code 1 if device cannot connect
    if (hasdeviceaccess() == FALSE){
        printf("Device Access Not Availible\n");
        return FALSE;
    }
        //Exit code 0 from hasdeviceaccess if can connect
    else if (hasdeviceaccess() == TRUE){
//        printf("Connection Successful\n");
        char *com1 = ("mkdir");
        char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
        char dircheck[500]; //set value for holding potential user input
        sprintf(dircheck, "%s %s %s", com1, absolutedirectory, com2);
        if(atoi(ios_runc(dircheck))==TRUE){
            return TRUE;
        }
        else{
            return FALSE;
        }
    }
}

//remember escape key for pipes and strtok for removing newlines to prevent multiple commands being executed.
//returns char array of output. redirect output to silent and add '; echo $?' to return exit code.
char *ios_runc(char *command){
    if (hasdeviceaccess()==FALSE){
        printf("Device Access Not Availible\n");
        return FALSE;
    }
    else if (hasdeviceaccess() == FALSE){
        char *com1 = "sshpass -p alpine ssh root@127.0.0.1 -p 2222";
        //com2 holds user input.
        char com2[800];
        //need to implement strncpy
        strcpy(com2, command);
        //commout to hold command to exec
        char commout[800];
        sprintf(commout, "%s %s", com1, com2);
        char *com = commout;
        //out char array to capture shell output
        char out[2048];
        FILE *shell = popen(com, "r");
        //fgets used to return shell output to out char array
        fgets(out, sizeof(out), shell);
        pclose(shell);
        return out;
    }
}

//returns 0 on success 1 if dir already exists
int ios_checkdirexists(char *dir){
    char *com1 = ("mkdir");
    char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
    //hold user input in large char array.
    char dircheck[800];
    sprintf(dircheck, "%s %s %s", com1, dir, com2);
    //return output as int.
    return atoi(ios_runc(dircheck));
}

//return 1 on error and 0 on success pass diskid in the form disk0s1s3 and mntpnt as absolute directory
//reference such as /mnt1
int ios_mountdisk(char *diskid, char *mntpnt){
    if (hasdeviceaccess()==FALSE){
        printf("Device Access Not Availible\n");
        return FALSE;
    }
    else if (hasdeviceaccess()==TRUE){
        //commout to hold command to exec.
        char commout[500];
        sprintf(commout, "mount_apfs %s %s >/dev/null 2>/dev/null \;echo $?", diskid, mntpnt);
        //return exit code as int. check if exit code is different if resource is busy or other errors.
        return atoi(ios_runc(commout));
    }
}

//need to improve process to return exit code
int ios_asr_process(char *rootfsdmg, char *rootfsoutdmg){
    //commout hold command to exec.
    char commout[500];
    sprintf(commout,"asr -source %s -target %s --embed -erase -noprompt --chunkchecksum --puppetstrings >/dev/null", rootfsdmg, rootfsoutdmg);
    FILE *file;
    //check rootfsdmg exists before processing. attempts access in read mode.
    if((file = fopen(rootfsdmg,"r"))!=NULL)
    {
        fclose(file);
        //exec's command if succesful after closing file access.
        system(commout);
        //check if output file now exists.
        FILE *fileout;
        if((fileout = fopen(rootfsoutdmg,"r"))!=NULL)
        {
            //close output file..
            fclose(fileout);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

//send file via scp to custom dir
int ios_send_f(char *filetosend, char *remotedir){

    if (hasdeviceaccess()==FALSE){
        printf("Device Access Not Availible\n");
        return FALSE;
    }
    else if (hasdeviceaccess() == TRUE){
        //verify file to send exists
        FILE *fileout;
        //checks if file to send exists before attempt..
        if((fileout = fopen(filetosend,"r"))!=NULL)
        {
            fclose(fileout);
            //commout holds command to exec..
            char commout[800];
            sprintf(commout, "sshpass -p alpine scp -P 2222 %s root@127.0.0.1:%s \; echo $?", filetosend, remotedir);
            char *com = commout;
            //holds exit code.
            char out[120];
            FILE *shell = popen(com, "r");
            //return exit code to out array.
            fgets(out, sizeof(out), shell);
            pclose(shell);
            //return exit code as int.
            return atoi(out);
        }
        else
        {
            return FALSE;
        }

    }

}

//returns major iOS version number as int.
int ios_ver_check(){
    //add checks to pull iOS version, maybe gotta use grep somewhere, uname?
    //remember to atoi the output when returning as int.
    //return should be 9,10,11,12,13 as examples.
    return atoi(ios_runc("sw_vers \| grep ProductVersion \| grep -o '[1][0-4]'"));
}

