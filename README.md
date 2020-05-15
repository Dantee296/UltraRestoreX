# UltraRestoreX. Control Your Device.

**We are releasing the Project iBootX Source Code, so the public can have a further insight into the process and contribute directly to the project. The code we are releasing has been migrated from one of our stable branches,and the code in this repository is deemed release-ready. We are going to add many extra comments to the code to fully explain the process, and will soon make a YouTube Video and extra YouTube Live sessions to explain the development process.
We'll continue working on it and publishing new beta's on a casual schedule.**

**To answer the incoming question:
Is this complete right now - yes and no. Our LimitedSupport branch, which can be found in this repository, has support for the iPhone 6, 6Plus, 5S, and some iPod Touch models. This support should actually include all devices that are vulnerable to Checkm8 and are currently running an iOS version below iOS 13. This is due to some changes in the boot process where the FSTAB file has been moved to a part of the bootchain called the DeviceTree, which involves some different patches**

**This release features a previously unreleased method for completing the majority of the iOS install off-device. The RamDisk creation process can now be effectively skipped, saving a lot of time.**

UltraRestoreX is designed to enable booting unsigned operating systems on modern iOS devices, for example the iPhone X. The document which will accompany this readme details the process of ‘dual-booting’ a modern iOS device, by patching the relevant parts of the iOS Boot-Chain and utilising some inbuilt macOS functions for disk management.

This will allow for security researchers to evaluate provided disk images on a live device, and enable users to boot custom firmware, otherwise known as 'CFW'.

As of writing there are a few tutorials/guides online partially detailing this process; however, I didn't feel like there was a convenient, one-click solution everybody can use in each situation (in an easy-to-approach way, at least). I also created a log of the research and development process so other users can work on and understand this process, which I will release soon

## Requirements
The software will be compiled to run on macOS Catalina and above, although may be backwards-compatible to a minimum macOS version 10.13 (i'll need to compile on a lower OS for this to be possible). Due to the unique nature of the application, It is not currently possible to port the tool to other operating systems, although it may be possible for us to implement the functionality into a portable live Linux USB, which we will work on as soon as we have everything in a stable form.

In a future release, I will recompile for a lower minimum operating system version in order to maximise the amount of compatible machines. The application does not utilise any API’s or features specific to newer macOS versions other than APFS_Invert, so there should not be any issues with this process.


## Full Guide Release To Follow Shortly.


## Credits
I would like to thank a few users, particularly @mcg_29 and @ralph0045 for their guide which inspired me and the team to start this journey to a full, automated, one click dual-boot and downgrade solution. Matthew Pierson for PyBoot, which allowed me to fix one of the bootchain patching steps. @lukew_0, @_JaccRob, @davewijk for developing the website and user interface for UltraRestoreX. And many, many more members of the community.
