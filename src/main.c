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
                         "Prepare Data Partition -> iOS", //ready
                         "Cleanup -> macOS", //ready
                         "Retrieve SHSH2 -> macOS", //ready
                         "Prepare IPSW -> macOS", //ready
                         "Patch Bootchain Elements -> macOS", //to-do
                         "Prepare Device", //ready
                         "Remount - TempFunc"}; //debug-ready

int main() {
    //var declarations
    char pecid[500];
    char ptype[500];
    char boardconf[500];

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
                    printf("Directory Already Exists, Skipping\n");
                }
                if(ios_makedir("/mnt2")!=0){
                    printf("Directory Already Exists, Skipping\n");
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
                                                printf("Compression Failed.\n");
                                            }
                                        }
                                        else{
                                            printf("Failed Remounting Disk\n");
                                        }
                                    } else{
                                        printf("FAIL\n");
                                    }
                                }
                                else{
                                    printf("Failed To Unmount\n");
                                }
                            }
                            else{
                                printf("Failed To Copy.\n");
                            }
                        }
                        else{
                            printf("Couldn't Find rootfsout.dmg, does it exist?\n");
                            return 1;
                        }
                    }
                    else{
                        printf("Failed Attaching Disk..\n");
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
                        printf("Could not Retrieve & Backup FSTAB\n");
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
                                printf("Couldn't Find FSTAB..\n");
                            }
                        }
                        else{
                            printf("Couldn't Find Old FSTAB\n");
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
                        printf("Debug kb copy failed.. or already run");
                    }
                }
                else{
                    printf("Operation Has Already Ran.. Or Some Other Error..\n");
                }
                //cleanup script cleans all files that are not necessary...
                break;
            case 10:
                if(1==1){
                    FILE *fileout;
                    if((fileout = fopen("iOS.dmg","r"))!=NULL)
                    {
                        fclose(fileout);
                        if(macOS_runc("rm iOS.dmg")==0){
                            printf("Removed iOS.dmg\n");
                        }
                    }
                    if((fileout = fopen("rootfsout.dmg","r"))!=NULL)
                    {
                        fclose(fileout);
                        if(macOS_runc("rm rootfsout.dmg")==0){
                            printf("Removed rootfsout.dmg\n");
                        }
                    }
                    if((fileout = fopen("fstab","r"))!=NULL)
                    {
                        fclose(fileout);
                        if(macOS_runc("rm fstab")==0){
                            printf("Removed new FSTAB // Preserved Backup\n");
                        }
                    }
                    if((fileout = fopen("iOSout.tar.gz","r"))!=NULL)
                    {
                        fclose(fileout);
                        if(macOS_runc("rm iOSout.tar.gz")==0){
                            printf("Removed iOSout.tar.gz\n");
                        }
                    }
                    printf("Disk Space +1\n");
                }

                break;
            case 11:
                printf("Fetching Device ECID..\n");
                if(hasdeviceaccess()==0){
                    if(ios_ecid_grab()!=1){
                        printf("Found Valid ECID!\n");
                        //store valid product ecid
                        //pecid stored at top of main
                        //stop using strcpy
                        strcpy(pecid,ios_ecid_grab());
                        printf("B-STRTOK %s\n",pecid);
                        //remove newlines
                        strtok(pecid,"\n");
                        printf("A-STRTOK %s\n",pecid);
                    } else{
                        printf("Failed To Obtain ECID..\n");
                        exit(1);
                    }
                } else{
                    printf("Device Access Could Not Be Established, Check iPhone Tunnel\n");
                    exit(1);
                }


                printf("Fetching Product Type..\n");
                if(hasdeviceaccess()==0){
                    if(ios_ptype_grab()!=1){
                        printf("Found Valid Product Type!\n");
                        //stores found device type
                        //ptype defined at top of main
                        //stop using strcpy
                        strcpy(ptype, ios_ptype_grab());
                        //remove newlines
                        strtok(ptype,"\n");
                    } else{
                        printf("Failed To Find Valid Product Type\n");
                    }
                }


                printf("Fetching Board Config..\n");
                if(hasdeviceaccess()==0){
                    if(ios_bconf_grab()!=1){
                        printf("Found Valid Board Config!\n");
                        //stores found device type
                        //ptype defined at top of main
                        //stop using strcpy
                        strcpy(boardconf, ios_bconf_grab());
                        //remove newlines
                        strtok(boardconf,"\n");
                    } else{
                        printf("Failed To Find Valid Board Configuration\n");
                    }
                }
                if((ios_blob_fetch(ptype,pecid,boardconf)==0)){
                    printf("Save SHSH2 Success! Generating IM4M\n");
                    system("img4tool -e -s *.shsh2 -m IM4M");
                } else{
                    printf("TSSChecker ERROR\n");
                }
                break;

            case 12:
                printf("Extracting IPSW..");
                if (macOS_runc("mv *.ipsw ipsw.zip")==0){
                    macOS_runc("rm -rf fw; mkdir fw");
                    if (macOS_runc("cd fw; unzip ../ipsw.zip")==0){
                        printf("Extracted IPSW!\nFinding RootFS..\n");
                        char rootfs[500];
                        strcpy(rootfs, macos_run_e("find . -xdev -type f -size +1G | grep dmg"));
                        strtok(rootfs,"\n");
                        printf("ROOTFS ID'd as %s", rootfs);
                        //rootfs move command
                        char rrootfscom[800];
                        sprintf(rrootfscom,"mv %s rootfsin.dmg",rootfs);
                        if(macOS_runc(rrootfscom)==0){
                            printf("Prep Stage 1 Complete!\n");
                            //clean unnecessary data.. no verification necessary
                            macOS_runc("rm -rf ./fw/__MACOSX");

                            //find ibss
                            printf("Finding iBSS\n");
                            char ibss[500];
                            strcpy(ibss, macos_run_e("find . -xdev -type f -name '*.im4p' | grep iBSS"));
                            strtok(ibss,"\n");
                            printf("iBSS ID'd as %s\n", ibss);
                            //ibss move command
                            char ibsscom[800];
                            sprintf(ibsscom,"mv %s ibssin",ibss);
                            if(macOS_runc(ibsscom)==0){
                                printf("iBSS!\n");
                                //find ibec
                                printf("Finding iBEC\n");
                                char ibec[500];
                                strcpy(ibec, macos_run_e("find . -xdev -type f -name '*.im4p' | grep iBEC"));
                                strtok(ibec,"\n");
                                printf("iBEC ID'd as %s\n", ibec);
                                //ibec move command
                                char ibeccom[800];
                                sprintf(ibeccom,"mv %s ibecin",ibec);
                                if(macOS_runc(ibeccom)==0){
                                    printf("iBEC!\n");
                                    //find kcache
                                    printf("Finding kcache\n");
                                    char kcache[500];
                                    strcpy(kcache, macos_run_e("find . -xdev -type f -name 'kernelcache*'"));
                                    strtok(kcache,"\n");
                                    printf("kcache ID'd as %s\n", kcache);
                                    //kcache move command
                                    char kcachecom[800];
                                    sprintf(kcachecom,"mv %s kcachein",kcache);
                                    if(macOS_runc(kcachecom)==0){
                                        printf("kcache!\n");
                                        //find dtre
                                        printf("Finding DTRE\n");
                                        char dtre[500];
                                        strcpy(dtre, macos_run_e("find . -xdev -type f -name '*.im4p' | grep DeviceTree"));
                                        strtok(dtre,"\n");
                                        printf("dtre ID'd as %s\n", dtre);
                                        //dtre move command
                                        char dtrecom[800];
                                        sprintf(dtrecom,"mv %s dtrein",dtre);
                                        if(macOS_runc(dtrecom)==0){
                                            printf("DTRE!\n");
                                            //need to add trustcache code
                                        }
                                        else{
                                            printf("Unable to find DTRE..\n");
                                            exit(1);
                                        }
                                    }
                                    else{
                                        printf("Unable to find kcache..\n");
                                        exit(1);
                                    }
                                }
                                else{
                                    printf("Unable to find iBEC..\n");
                                    exit(1);
                                }
                            }
                            else{
                                printf("Unable to find iBSS..\n");
                                exit(1);
                            }
                            //find trustcache if target is 13,n,above ( take val from rootfs name )

                        } else{
                            printf("Something went wrong..");
                        }
                    }
                } else{
                    printf("IPSW not found, please insert IPSW into source directory and run again\n");
                }
                //use partial zip download to retrieve certain directory.
                //patch bootchain elements making sure correct tool is used
                break;
            case 13:
                //patch-bootchain
                if(1==1){
                    printf("Preparing...");
                    char tsys[300];
                    strcpy(tsys,ios_runc("ls /dev \| cat \| grep -o disk0s1s. \| tail -2 \| head -1"));
                    strtok(tsys,"\n");
                    char tprod[300];
                    strcpy(tprod,ios_ptype_grab());
                    printf("ConnectAgent Calling...\n");
                    char connect[1000];
                    sprintf(connect,"python3 connect.py -i %s 13.3 -d %s -b apl.png",tprod,tsys);
                    printf("%s",connect);
                    sleep(120);
                    system(connect);
                }
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
                        printf("Error/Already Mounted\n");
                        exit(1);
                    }
                    if(ios_mountdisk(DataB,mnt2)!=0){
                        printf("Error/Already Mounted\n");
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