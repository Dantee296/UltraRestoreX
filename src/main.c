//
// Created by 010010 on 03/05/2020.
//
#include "header.h"
#include "ios.h"

const char *menuItems[] = {"Prerequisites",  //done for the most-part
                         "Generate System & Data Partitions -> iOS",  //ready
                         "Create Mount Points & Mount -> iOS", //ready
                         "Prepare RootFS DMG -> macOS", //ready
                         "Virtual iOS Install -> macOS", //ready
                         "Extract Virtual Install -> iOS", //ready
                         "Patch Boot & Configure SEP -> iOS", //port
                         "Prepare Data Partition -> iOS", //port
                         "Cleanup -> macOS", //to-do
                         "Retrieve SHSH2 -> macOS", //to-do
                         "Retrieve Bootchain For Target -> macOS", //to-do
                         //need to determine iOS version for correct method using systemversion.plist from rootfs.
                         //make sure to extract before cleanup process..
                         "Patch Bootchain Elements -> macOS", //to-do
                         "Prepare Device"}; //port

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
                printf("Preparing RootFS DMG...\n");
                if(ios_asr_process("rootfsin.dmg","rootfsout.dmg")==0){
                    printf("Great Success\n");
                }
                else{
                    printf("Error Opening RootFS..\n");
                }
                break;
            case 5:
                if(macOS_runc("hdiutil create -size 10GB -fs APFS -volname iOS iOS.dmg")==0){
                    printf("iOS VInstall Disk Created...\n");
                    if(macOS_runc("hdiutil attach iOS.dmg")==0){
                        printf("Attached Disk\n");
                        const char *diskid = macos_run_e("diskutil list | grep -o disk.s. | tail -1");
                        FILE *file;
                        if((file = fopen("rootfsout.dmg","r"))!=NULL){
                            fclose(file);
                            if(OSCopyFile("rootfsout.dmg","/Volumes/iOS/rootfsout.dmg")==0){
                                char diskforfunc[50];
                                strcpy(diskforfunc,diskid);
                                strtok(diskforfunc,"\n");
                                printf("Copy Successful\n");
                                if(macOS_runc("umount /Volumes/iOS")==0){
                                    printf("Unmount Complete\nInverting Disk\n");
                                    if(macOS_apfs_invert(diskforfunc, "rootfsout.dmg")==0){
                                        printf("APFS_Invert Complete\n");
                                        if(macOS_runc("hdiutil attach iOS.dmg")==0){
                                            printf("Compressing VDisk\n");
                                            if(macOS_runc("tar -zcf  iOSout.tar.gz /Volumes/iOS >/dev/null 2>/dev/null")==0){
                                                printf("Disk Compressed Successfully\nVirtual Install Process Complete\n");
                                                exit(0);
                                            }
                                            else{
                                                printf("Compression Failed..\n");
                                            }
                                        }
                                        else{
                                            printf("Failed To Remount Disk");
                                        }
                                    } else{
                                        printf("FAIL\n");
                                    }
                                }
                                else{
                                    printf("Failed To Unmount");
                                }
                            }
                            else{
                                printf("Failed To Copy...\n");
                            }
                        }
                        else{
                            printf("Couldn't Find rootfsout.dmg");
                            return 1;
                        }
                    }
                    else{
                        printf("Failed To Attach Disk..\n");
                        exit(1);
                    }
                }
                else{
                    printf("Disk Could Not Be Created\n");
                    exit(1);
                }
                break;
            case 6:
                //send file to device.
                printf("Sending...\nPlease Wait\n");
                if((ios_send_f("iOSout.tar.gz","/mnt1")==0)){
                    printf("Succesfully Sent VInstall To Device..\nExtracting..\n");
                    if (atoi(ios_runc("cd /mnt1 \; tar -xvf iOSout.tar.gz —-strip-components=2 >/dev/null 2>/dev/null \; echo $?"))==0){
                        printf("Extract Process Complete!\n");
                    }
                } else{
                    printf("Error Sending VInstall..\n");
                }
                break;
            case 7:
                //patch fstab and configure SEP
                break;
            case 8:
                //prepare data partition
                break;
            case 9:
                //cleanup script cleans all files that are not necessary...
                break;
            case 10:
                //use tsschecker or something to download an shsh2 for use
                break;
            case 11:
                //use partial zip download to retrieve certain directory.
                break;
            case 12:
                //patch bootchain elemements making sure correct tool is used
                break;
            case 13:
                //device prep
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