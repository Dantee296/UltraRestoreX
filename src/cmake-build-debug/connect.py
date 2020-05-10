#!/usr/bin/env python3

import subprocess
from subprocess import check_output
import sys

try:

    import argparse
    import os
    import platform
    import plistlib
    import re
    import shutil
    import time
    from argparse import RawTextHelpFormatter
    from resources import img4, pwn, ipsw
    from resources.iospythontools import iphonewiki, ipswapi, utils

except:
    print("Failed to import dependencies, running pip to install them...")
    subprocess.check_call([sys.executable, "-m", "pip", "install", "-r", "requirements.txt"])
    try:
        import argparse
        import os
        import platform
        import plistlib
        import re
        import shutil
        import time
        from subprocess import check_output
        from argparse import RawTextHelpFormatter
        from resources import img4, pwn, ipsw
        from resources.iospythontools import iphonewiki, ipswapi, utils
        
    except:
        print("\n\nFailed to install dependencies\n")
        exit(0)
    print("\n\nSuccessfully installed dependencies!\n")



tool_version = '\033[92m' + "iBootX_Connect" + '\033[0m'


def main():
    removeFiles = [
        'resources/devicetree.im4p',
        'resources/devicetree.img4',
        'resources/ibec.im4p',
        'resources/ibec.img4',
        'resources/ibec.raw',
        'resources/ibec.pwn',
        'resources/ibec.patched',
        'resources/ibss.im4p',
        'resources/ibss.img4',
        'resources/ibss.raw',
        'resources/ibss.pwn',
        'resources/ibss.patched',
        'resources/kernel.im4p',
        'resources/kernel.img4',
        'resources/manifest.plist',
        'resources/shsh.shsh',
        'resources/trustcache.im4p',
        'resources/trustcache.img4',
        'resources/bootlogo.im4p',
        'resources/bootlogo.ibootim',
        "resources/bootlogo.img4"
    ]

    for item in removeFiles: # removes files from above list
        if os.path.isfile(item):
            os.remove(item)

    utils.clean() # removes potentially out of date JSON files 

    text = 'iBootX Connect - Integration..'
    parser = argparse.ArgumentParser(description=text, formatter_class=RawTextHelpFormatter, usage=f"./connect.py -i 'DEVICE IOS'\n\nE.G './connect.py -i iPhone9,2 13.2.3 -b ~/Downloads/bootlogo.png'\n\nCurrent Connect version is: {tool_version}", epilog="EXAMPLE USAGE: ./connect.py -i iPhone8,1 13.4.1 -d disk0s1s6\n\nOR ./connect -i iPhone9,4 13.1.3 -b ~/Downloads/bootlogo.png\n\nOR ./connect -q ~/Downloads/13.2.3.iPhone7.ipsw iPhone9,1 -a")
    parser.add_argument("-i", "--ios", help="\n", nargs=2, metavar=('\b', '\b'))
    parser.add_argument("-q", "--ipsw", help="\n", nargs=2, metavar=('\b', '\b'))
    parser.add_argument("-b", "--bootlogo", help="\n", nargs=1, metavar=("\b"))
    parser.add_argument('-p', '--pwn', help='\n', action='store_true')
    parser.add_argument("-d", "--dualboot", help="\n", nargs=1, metavar=("\b"))
    parser.add_argument("-a", "--bootargs", help="\n", action='store_true')
    parser.add_argument("-v", "--version", help="\n", action="store_true")

    if platform.system() == 'Darwin':  # If not MacOS then exit basically
        pass
    elif platform.system() == "Linux":
        sys.exit('\033[91m' + "\nSorry this OS is not currently supported!\n\nOnly MacOS machines (Hackintosh or a legitimate Apple computer) are support as of now.\n" + '\033[0m')
    elif platform.system() == "Windows":
        sys.exit('\033[91m' + "\nERROR!\n\n" + '\033[0m')
    else:
        sys.exit("")

    args = parser.parse_args()

    if args.pwn:
        pwn.pwndfumode()
        exit(22)
        
    elif args.ipsw:
        if args.bootlogo:
            useCustomLogo = True
            logopath = args.bootlogo[0]
        else:
            useCustomLogo = False
            logopath = "null"
        if args.dualboot:
            bootOtherOS = True
            sysPartName = args.dualboot[0]
            bootArgs = f"rd={sysPartName} -v"
            if args.bootargs:
                print("ERR")
        else:
            bootOtherOS = False
            if args.bootargs:
                bootArgs = input("Please enter the boot-args you want to use then press enter: ")
            else:
                bootArgs = "-v"
        
            
        print('\033[95m' + "iBootX Connect - Integration..\n" + '\033[0m')
        print("Current version is: " + tool_version)
        print("User chose to use a locally stored IPSW, running some checks...")
        if os.path.exists("IPSW"):
            shutil.rmtree("IPSW")
        ipsw.unzipIPSW(args.ipsw[0])
        version = False
        supportedModels = str(ipsw.readmanifest("IPSW/BuildManifest.plist", version))
        if args.ipsw[1] in supportedModels:
            print("IPSW is for given device!")
        else:
            print("Sorry this IPSW is not valid for the given device ERR")
            exit(0)
        version = True
        iosVersion = str(ipsw.readmanifest("IPSW/BuildManifest.plist", version))
        print(f"iOS version is: {iosVersion} and device model is: {args.ipsw[1]}")
        time.sleep(5)

        arewelocal = True
        img4.img4stuff(args.ipsw[1], iosVersion, useCustomLogo, logopath, arewelocal, bootOtherOS, bootArgs)

        # now to pwn device
        print("Exploiting")
        pwn.pwndfumode()

        # Send files to device and boot =)
        img4.sendImages(iosVersion, useCustomLogo)

        print("Device should be booting!")
        exit(0)
    elif args.ios:
        print('\033[95m' + "iBootX Connect - Integration..\n" + '\033[0m')
        print("Current version is: " + tool_version)
        if args.bootlogo:
            useCustomLogo = True
            logopath = args.bootlogo[0]
        else:
            useCustomLogo = False
            logopath = "null"
        if args.dualboot:
            bootOtherOS = True
            sysPartName = args.dualboot[0]
            bootArgs = f"rd={sysPartName} -v"
            print(f"User choose to boot {args.ios[1]} from /dev/{sysPartName}.")
            if args.bootargs:
                print(f"\n" + '\033[93m' + "WARNING:" + '\033[0m' + f"'-a' was specified indicating the user wanted to set custom boot-args, but '-d' was also set which currently doesn't support custom boot-args...\nIgnoring '-a' and continuing with '{bootArgs}' as the set boot-args.\n")
        else:
            bootOtherOS = False
            if args.bootargs:
                bootArgs = input("Please enter the boot-args you want to use then press enter: ")
            else:
                bootArgs = "-v"

        if "10." in (str(args.ios))[2:-2]:
            print("\nWARNING.Press.Enter\n")
            choice = input("")
            if choice == "":
                pass
            else:
                print("Exiting...")
                exit(2)

        print("Make sure your device is connected in DFU mode")
        time.sleep(5)
        arewelocal = False
        img4.img4stuff(args.ios[0], args.ios[1], useCustomLogo, logopath, arewelocal, bootOtherOS, bootArgs)

        # now to pwn device
        print("Exploiting device with checkm8")
        pwn.pwndfumode()

        # Send files to device and boot =)
        img4.sendImages(args.ios[1], useCustomLogo)

        print("Device should be booting!")
        exit(2)

    elif args.version:
        sys.exit(f"Current version is: {tool_version}")

    else:
        sys.exit(parser.print_help(sys.stderr))


if __name__ == "__main__":
    main()
