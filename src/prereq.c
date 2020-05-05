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

int macOS_runc(char *command){
    char *com1 = ">/dev/null 2>/dev/null;echo $?";
    char com2[800];
    strcpy(com2, command);
    char commout[800];
    sprintf(commout, "%s %s", com2, com1);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    printf("%s",out);
    pclose(shell);
    return atoi(out);
}

char *macos_run_e(char *command){
        char com2[800];
        strcpy(com2, command);
        char commout[800];
        sprintf(commout, "%s", com2);
        char *com = commout;
        char out[2048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        return out;
}

int OSCopyFile(const char* source, const char* destination){
    int input, output;
    if ((input = open(source, O_RDONLY)) == -1)
    {
        return -1;
    }
    if ((output = creat(destination, 0660)) == -1)
    {
        close(input);
        return -1;
    }

    //Here we use kernel-space copying for performance reasons
#if defined(__APPLE__) || defined(__FreeBSD__)
    //fcopyfile works on FreeBSD and OS X 10.5+
    int result = fcopyfile(input, output, 0, COPYFILE_ALL);
#else
    //sendfile will work with non-socket output (i.e. regular file) on Linux 2.6.33+
    off_t bytesCopied = 0;
    struct stat fileinfo = {0};
    fstat(input, &fileinfo);
    int result = sendfile(output, input, &bytesCopied, fileinfo.st_size);
#endif

    close(input);
    close(output);

    return result;
}

int macOS_apfs_invert(char *diskid, char *dmg){
    char commout[800];
    //add check for if apfs_invert binary is present
    sprintf(commout, "sudo /System/Library/Filesystems/apfs.fs/Contents/Resources/apfs_invert -d %s -s 1 -n %s", diskid, dmg);
    char *com = commout;
    if (macOS_runc(com)==0){
        return 0;
    }
    else{
        return 1;
    }
}