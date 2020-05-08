//
// Created by 010010 on 03/05/2020.
//
#include "header.h"
#include "ios.h"

const char *menuItems[] = {"Prerequisites",  //ready for now..
                         "Generate System & Data Partitions -> iOS",  //ready
                         "Create Mount Points & Mount -> iOS", //ready
                         "Prepare RootFS DMG -> macOS", //ready
                         "Virtual iOS Install -> macOS", //ready
                         "Send Virtual Install -> iOS", //ready
                         "Extract Virtual Install -> iOS", //ready
                         "Patch Boot & Configure SEP -> iOS", //ready
                         "Prepare Data Partition -> iOS", //port
                         "Cleanup -> macOS", //to-do
                         "Retrieve SHSH2 -> macOS", //to-do
                         "Retrieve Bootchain For Target -> macOS", //to-do
                         //need to determine iOS version for correct method using systemversion.plist from rootfs.
                         //make sure to extract before cleanup process..
                         "Patch Bootchain Elements -> macOS", //to-do
                         "Prepare Device",
                         "Remount - TempFunc"}; //debug-ready

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
                } else{
                    printf("Great Success\n");
                    exit(0);
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
                printf("Generating iOS VDisk\n");
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
                                    printf("Unmount Complete\nInverting Disk\nEnter User Password:\n");
                                    if(macOS_apfs_invert(diskforfunc, "rootfsout.dmg")==0){
                                        printf("APFS_Invert Complete\n");
                                        if(macOS_runc("hdiutil attach iOS.dmg")==0){
                                            printf("Compressing VDisk - This Can Take A While..\n");
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
                    printf("Succesfully Sent VInstall To Device..\n");
                } else{
                    printf("Error Sending VInstall..\n");
                    exit(1);
                }
                break;
            case 7:
                printf("Extracting iOS VDisk\n");
                if (atoi(ios_runc("tar -xvf /mnt1/iOSout.tar.gz --strip-components=2 -C /mnt1 >/dev/null 2>/dev/null \; echo $?"))==0){
                    printf("Extract Successfull\n");
                }
                else{
                    printf("Error Extracting\n");
                    exit(1);
                }
                break;
            case 8:
                //patch fstab and configure SEP
                if(ios_rec_f("/private/etc/fstab","fstab-bak")==0){
                    printf("Device FSTAB Backed Up To iBootX Dir..\n");
                    if (macOS_runc("cp fstab-bak fstab")==0){
                        //original data collect
                        //origvar stores original var from fstab.
//                      char *origvar = macos_run_e("cat fstab-bak | grep '/private/var apfs' | grep -o disk0s1s.");
                        char *origvar = "disk0s1s2";
                        //origsys stores original sys from fstab.
                        char *origsys = "disk0s1s1";
                        printf("Original SYS DETECTED -> %s\n",origsys);
                        printf("Original Var DETECTED -> %s\n",origvar);
                        printf("DSX Support ENABLED\n");
                        //target data collect
                        char tvar[300];
                        char tsys[300];
                        strcpy(tsys,ios_runc("ls /dev \| cat \| grep -o disk0s1s. \| tail -2 \| head -1"));
                        strtok(tsys,"\n");
                        strcpy(tvar, ios_runc("ls /dev \| cat \| grep -o disk0s1s. \| tail -1"));
                        strtok(tvar,"\n");
                        printf("Target SYS DETECTED -> %s\n", tsys);
                        printf("Target Var DETECTED -> %s\n", tvar);
                        //patch
                        printf("Patching...\n");
                        if(ios_fstab_p("fstab",origsys,tsys)==0){
                            printf("Patch FSTAB Success!\n");
                        } else{
                            printf("PATCH FAIL");
                        }
                        if(ios_fstab_p("fstab",origvar,tvar)==0){
                            printf("Patch FSTAB Success!\n");
                        } else{
                            printf("PATCH FAIL");
                        }
                    }
                    else{
                        printf("Could Not Retrieve & Backup FSTAB\n");
                        exit(1);
                    }
                    }
                printf("SEP\n");
                if(ios_sep_c("/mnt1")==0){
                    printf("SEP Config Complete\n");
                    printf("BASEBAND\n");
                    if(ios_bb_c("/mnt1")==0){
                        printf("BB Config Complete\n");
                        //remove old fstab and send new fstab
                        printf("Sending FSTAB..\n");
                        if(atoi(ios_runc("rm /mnt1/private/etc/fstab \; echo $?"))==0){
                            if(ios_send_f("fstab","/mnt1/private/etc/fstab")==0){
                                printf("FSTAB Sent\n");
                            }
                            else{
                                printf("Couldnt Find FSTAB..\n");
                            }
                        }
                        else{
                            printf("Couldnt Find Old FSTAB\n");
                        }
                    }
                    else{
                        printf("BB FAIL\n");
                    }
                }
                else{
                    printf("SEP Config FAIL\n");
                }
                printf("%d\n", ios_ver_check());
                break;
                //prepare data partition
                printf("PRINTING NEWSYS\n");
                system("sshpass -p alpine ssh root@127.0.0.1 -p 2222 ls -l /");
                sleep(20);
                break;
            case 9:
                printf("Prepping Data Partition\n");
                //add extra checks to ensure disk is mounted, maybe grep df for this check..
                if(atoi(ios_runc("mv /mnt1/private/var/* /mnt2 >/dev/null 2>/dev/null \; echo $?"))==0){
                    printf("Operation Complete\nDEBUG copying KB\n");
                    if(atoi(ios_runc("cp -a /var/keybags /mnt2"))==0){
                        printf("KB success\n");
                    }
                    else{
                        printf("debug kb copy failed.. or already run");
                    }
                }
                else{
                    printf("Operation Has Already Been Run.. Or Some Other Error..\n");
                }


                //cleanup script cleans all files that are not necessary...
                break;
            case 10:
                //use tsschecker or something to download an shsh2 for use
                break;
            case 11:
                //use partial zip download to retrieve certain directory.
                break;
            case 12:
                //patch bootchain elements making sure correct tool is used
                break;
            case 13:
                //device prep
                break;
            case 15:
                //temp func for disk remount..
                if(1==1){
                    char SystemB[300];
                    char DataB[300];
                    char *mnt1 = "/mnt1";
                    char *mnt2 = "/mnt2";
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
                }

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