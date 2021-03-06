//
// Created by 010010 on 03/05/2020.
//

#ifndef SRC_IOS_H
#define SRC_IOS_H

#endif //SRC_IOS_H

int ios_makevol_apfs(char *volname, char *role);
int ios_makedir(char *absolutedirectory);
int ios_mountdisk(char *diskid, char *mntpnt);
int ios_checkdirexists(char *dir);
char *ios_runc(char *command);
int ios_asr_process(char *rootfsdmg, char *rootfsoutdmg);
int ios_send_f(char *filetosend, char *remotedir);
int ios_rec_f(char *remotefiledir, char *localfname);
int ios_fstab_p(char *fstabloc,char *fromval,char *toval);
int ios_sep_c(char *newmnt);
int ios_bb_c(char *newmnt);
char *ios_ecid_grab();
char *ios_ptype_grab();
char *ios_bconf_grab();
int *ios_blob_fetch(char *ptype, char *eciddec, char *boardconf);
int ios_dtre_patch(int iosver,char *dtrein, char *dtreout, char *IM4M);
int ios_ver_check();