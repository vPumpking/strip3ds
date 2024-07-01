# Welcome to **strip3ds**!

This tool was made for everyone who wants to extract Nintendo 3DS game resources, but who finds the process too hard or who wants to do it on a large batch of files.
This is a program using several other tools, one for each operation, at a same time, so you don't have to do it yourself.


# How to use
## Windows
Once downloaded the latest release, extract the **strip3ds** folder where you want and put as many **.3ds** or **.cci** game files as you want in this folder, next to the `strip3ds.exe` file. All of these will be extracted, but ensure they all have a different name, even if the extension is different. Then open PowerShell or a commandline prompt, navigate to the `strip3ds` folder with the
```powershell
cd [folder]
```
command and then run
```powershell
./strip3ds.exe
```
and press "y" when asked.
### Dependencies
All the external tools used by **strip3ds** are in the `dependencies` folder, next to the main script. In most of cases, **strip3ds** launches correctly, but you can need to run `./dependencies/szs64/windows-install.exe` (normally the main program asks for it every time it starts). Your antivirus also can recognize some dependencies as virus or trojans, but **they aren't**: they are just doing unusual operations that the antivirus is misunderstanding. If it's your case, just allow the execution anyway.
**Keep in mind that this folder always has to be in the same directory as the main program.**
## Linux
The Linux version is still in development and won't be available in a while, since I need to redo all the dependencies: they are windows programs! Some help would be appreciated if you find this tool useful.
# Features
Strip3ds is meant to extract the 3D models and resources from one or several 3DS rom(s), however the current version is lacking some file format conversions, and has only be tested with a Super Mario 3D Land ROM. It may lack some specific file formats, such as those from Luigi's Mansion 2 or Pokémon games.
# CONTRIBUTE
**Any** help is appreciated. As I'm not the best repository manager, though the one strip3ds is in can be missing some security settings. **If you find yourself able to merge a PR without my consent, please don't and wait for my approval.**
## Compile from source (Windows)
To compile the .c file, you can use whatever C compiler you want. I use gcc, you can have it on Windows by installing MINGW from https://www.mingw-w64.org/ then run
```powershell
cd [strip3ds folder]
gcc -o strip3ds strip3ds.exe
```
# License
As long as you credit me, you are free to make any use of the parts I made, which are the files `strip3ds.c` and `dependencies/cgfx2obj/process.py`. For the dependencies, check their repositories / websites to know more.