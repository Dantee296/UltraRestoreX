//
// Created by 010010 on 03/05/2020.
//

#ifndef SRC_IOS_H
#define SRC_IOS_H

#endif //SRC_IOS_H

int ios_makevol_apfs(char *volname);
int ios_makedir(char *absolutedirectory);
int ios_mountdisk(char *diskid, char *mntpnt);
int ios_checkdirexists(char *dir);
char *ios_runc(char *command);
int ios_asr_process(char *rootfsdmg, char *rootfsoutdmg);
int ios_send_f(char *filetosend, char *remotedir);
int ios_ver_check();