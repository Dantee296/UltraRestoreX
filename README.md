# iBootX. Control Your Device.

**We are releasing *part of the* iBootX Source Code, so the public can have a further insight into the process and contribute directly to the project. The code we are releasing has been migrated from one of our internal test branches and is currently incomplete. We are going to add many extra comments to the code to fully explain the process, and will soon make a YouTube Video on the process.
We'll continue working on it and publishing new beta's on a casual schedule.**

**To answer the incoming question:
Is this complete right now - no. We have moved code from one of our incomplete testing branches. We felt that this specific branch was suitable to release at this moment as we could share some unreleased methods with the public, and automate at least half of this process. BUT with that being said, the code is actively being worked on by the team, and now with the knowledge of the public, we hope we are able to fully complete this application.**

**This release features a previously unreleased method for completing the majority of the iOS install off-device. The RamDisk creation process can now be effectively skipped, saving a lot of time.**

![alt text](https://github.com/DuffyAPP-IT/iBootX/blob/master/Screenshot_2020-04-14_at_00.26.42.png?raw=true)

iBootX is a project, soon-to-be tool, that is designed to enable booting unsigned operating systems on modern iOS devices, for example the iPhone X. The document which will accompany this read-me details the process of ‘dual-booting’ a modern iOS device, by patching the relevant parts of the iOS Boot-Chain and utilising some inbuilt macOS functions for disk management.

This will allow for security researchers to evaluate provided disk images on a live device, and enable users to boot custom firmware, otherwise known as 'CFW'.

As of writing there are a few tutorials/guides online partially detailing this process; however, I didn't feel like there was a convenient, one-click solution everybody can use in each situation (in an easy-to-approach way, at least). I also created a log of the research and development process so other users can work on and understand this process.

## Requirements
The software will be compiled to run on macOS Catalina and above, although may be backwards-compatible to a minimum macOS version 10.13. Due to the unique nature of the application, It is not currently possible to port the tool to other operating systems.

The user will be expected to have a valid SHSH2 Blob for their device, ignoring the signing status of the specific iOS Version & Target Boot version (Any SHSH will work for this process).

In a future release, I will recompile for a lower minimum operating system version in order to maximise the amount of compatible machines. The application does not utilise any API’s or features specific to newer macOS versions other than APFS_Invert, so there should not be any issues with this process.


## Full Guide Release To Follow Shortly.


## Credits
I would like to thank a few users, particularly @mcg_29 and @ralph0045 for their guide which inspired me and the team to start this journey to a full, automated, one click dual-boot and downgrade solution.
