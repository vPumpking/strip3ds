# Welcome to **strip3ds**!

This tool was made for everyone who wants to extract Nintendo 3DS game resources, but who finds the process too hard or who wants to do it on a large batch of files.
This is a program using several other tools, one for each operation, at a same time, so you don't have to do it yourself.


# How to use
## Windows
Once downloaded the latest release, extract the **strip3ds** folder where you want and put as many **.3ds** or **.cci** game files as you want in this folder, next to the `strip3ds.bat` script. All of these will be extracted, but ensure they all have a different name, even if the extension is different. Then just double-click on the `strip3ds.bat` file and press "y" when asked.
### Dependencies
All the external tools used by **strip3ds** are in the `dependencies` folder, next to the main script. In most of cases, **strip3ds** launches correctly, but you can need to run `./dependencies/szs64/windows-install.exe` (normally the main program does it itself). Your antivirus also can recognize some dependencies as virus or trojans, but **they ar'nt**: the are just doing unusual operations that the antivirus is misunderstanding. If it's your case, just allow the execution anyway.
**Keep in mind that this folder always has to be in the same directory as the main program.**
## Linux
The Linux version is still in development and isn't available yet.
# Features
Strip3ds is meant to extract the 3D models of one or several 3DS rom(s), however the current version is lacking the final conversion, from 