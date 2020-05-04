//
// Created by 010010 on 03/05/2020.
//
#include "header.h"
#include "ios.h"

const char *menuItems[] = {"Prerequisites",
                         "Generate System & Data Partitions -> iOS",
                         "Create Mount Points & Mount -> iOS",
                         "Prepare RootFS DMG -> macOS",
                         "Virtual iOS Install -> macOS",
                         "Extract Virtual Install -> iOS",
                         "Patch Boot & Configure SEP -> iOS",
                         "Prepare Data Partition -> iOS",
                         "Cleanup -> macOS",
                         "Retrieve SHSH2 -> macOS",
                         "Retrieve Bootchain For Target -> macOS",
                         //need to determine iOS version for correct method using systemversion.plist from rootfs.
                         //make sure to extract before cleanup process..
                         "Patch Bootchain Elements -> macOS",
                         "Prepare Device"};

int main() {
    printmenu();
    printf("\nMain Menu:\n");
    printf("");
    for (int i = 0; i < ARRAY_SIZE(menuItems); i++) {
        printf("%d: %s\n", i+1, menuItems[i]);
    }
    char *optchar;
    printf("> ");
    fgets(&optchar, 50, stdin);
    int opt;
    opt = atoi(&optchar);

    if (opt <= ARRAY_SIZE(menuItems)){
        switch(opt){
            case 1:
                installer();
                break;
            case 2:
                if(ios_makevol_apfs("SystemB")!=0){
                    printf("An error occured..\n");
                    exit(1);
                }
                if(ios_makevol_apfs("DataB")!=0){
                    printf("An error occured..\n");
                    exit(1);
                }
                break;
            case 3:
                //Make mountpoints - Add check if folder exists, add to ibootxlib
                if(ios_makedir("/mnt1")!=0){
                    printf("Directory Already Exists.. Skipping\n");
                }
                if(ios_makedir("/mnt2")!=0){
                    printf("Directory Already Exists.. Skipping\n");
                }
                char SystemB[300];
                char DataB[300];
                char *mnt1 = "/mnt1";
                char *mnt2 = "/mnt2";
                //implement strncpy
                //could have issues with disk number above 10, experiement to see if .. works instead of . on low (below 10) partition count.
                //will be fine for dual boot.
                strcpy(SystemB,ios_runc("ls /dev \| cat \| grep -o disk0s1s. \| tail -2 \| head -1"));
                strtok(SystemB,"\n");
                strcpy(DataB, ios_runc("ls /dev \| cat \| grep -o disk0s1s. \| tail -1"));
                strtok(DataB,"\n");
                if(ios_mountdisk(SystemB,mnt1)!=0){
                    printf("Mounting Error.. Reboot iPhone w/Checkra1n\n");
                    exit(1);
                }
                if(ios_mountdisk(DataB,mnt2)!=0){
                    printf("Mounting Error.. Reboot iPhone w/Checkra1n\n");
                    exit(1);
                }
                break;
            case 4:
                break;
            default:
                printf("\nOption Not Found\n");
                sleep(2);
                main();
        }
    } else{
        printf("Invalid ID\n");
        exit(1);
    }
    return 0;
}