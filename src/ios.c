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
        exit(1);
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
        char com2[2400];
        strcpy(com2, command);
        char commout[2400];
        sprintf(commout, "%s %s", com1, com2);
        char *com = commout;
        char out[4048];
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

//need to improve process to return exit code
int ios_asr_process(char *rootfsdmg, char *rootfsoutdmg){
    char commout[500];
    sprintf(commout,"asr -source %s -target %s --embed -erase -noprompt --chunkchecksum --puppetstrings >/dev/null", rootfsdmg, rootfsoutdmg);
    FILE *file;
    if((file = fopen(rootfsdmg,"r"))!=NULL)
    {
        fclose(file);
        system(commout);
        FILE *fileout;
        if((fileout = fopen(rootfsoutdmg,"r"))!=NULL)
        {
            fclose(fileout);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

int ios_send_f(char *filetosend, char *remotedir){
    if (hasdeviceaccess()==1){
        printf("Device Access Not Availible\n");
        return 1;
    }
    else if (hasdeviceaccess() == 0){
        //verify file to send exists
        FILE *fileout;
        if((fileout = fopen(filetosend,"r"))!=NULL)
        {
            fclose(fileout);
            char commout[800];
            sprintf(commout, "sshpass -p alpine scp -P 2222 %s root@127.0.0.1:%s \; echo $?", filetosend, remotedir);
            char *com = commout;
            char out[2048];
            FILE *shell = popen(com, "r");
            fgets(out, sizeof(out), shell);
            pclose(shell);
            return atoi(out);
        }
        else
        {
            return 1;
        }

    }

}

//pull file from device using scp
int ios_rec_f(char *remotefiledir, char *localfname){
    if (hasdeviceaccess()==1){
        printf("Device Access Not Availible\n");
        return 1;
    }
    else if (hasdeviceaccess() == 0){
        char commout[800];
        sprintf(commout, "sshpass -p alpine scp -r -P 2222 root@127.0.0.1:%s %s \; echo $?", remotefiledir, localfname);
        char *com = commout;
        char out[2048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        return atoi(out);
    }
    else
    {
        return 1;
    }
    }

//patch fstab locally after using ios_rec_f to pull fstab
int ios_fstab_p(char *fstabloc,char *fromval,char *toval){
    //for command to patch concat
    char comm[900];
    sprintf(comm,"sed -i '' 's/%s/%s/g' %s",fromval, toval, fstabloc);
    return macOS_runc(comm);
}

//example ios_sep_mov("/mnt1");
int ios_sep_c(char *newmnt){
    char mntcheck[900];
    char comm[900];
    sprintf(mntcheck,"df \| grep mnt1 >/dev/null 2>/dev/null \; echo $?",newmnt);
    if (atoi(ios_runc(mntcheck))==0){
        sprintf(comm,"cp -a /usr/standalone/firmware/sep* %s/usr/standalone >/dev/null 2>/dev/null \; echo $?",newmnt);
        return atoi(ios_runc(comm));
    }
    else{
        return 1;
    }
}

//^
int ios_bb_c(char *newmnt){
    char mntcheck[900];
    char comm[900];
    sprintf(mntcheck,"df \| grep mnt1 >/dev/null 2>/dev/null \; echo $?",newmnt);
    if (atoi(ios_runc(mntcheck))==0){
        sprintf(comm,"cp -a /usr/local %s/usr/local >/dev/null 2>/dev/null \; echo $?",newmnt);
        return atoi(ios_runc(comm));
    }
    else{
        return 1;
    }
}

char *ios_ecid_grab(){
        if (macOS_runc("ideviceinfo | grep UniqueChipID | grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]'")==0){
            return macos_run_e("ideviceinfo | grep UniqueChipID | grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]'");
        }
        else{
            return 1;
        }
}

//returns major ios version as int
int ios_ver_check(){
    //add checks to pull iOS version, maybe gotta use grep somewhere, uname?
    //remember to atoi the output when returning as int.
    //return should be 9,10,11,12,13 as examples.
    return atoi(ios_runc("sw_vers \| grep ProductVersion \| grep -o '[1][0-4]'"));
}
