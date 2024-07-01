/****************************************************************************
 *   Project Name: strip3ds
 *   Description:  Tool for extracting resources from 3DS ROMs.
 *
 *   Copyright (C) 2024 Martin Louvel
 *
 *   This file is part of strip3ds.
 *
 *   Permission is hereby granted, free of charge, to any person obtaining 
 *   a copy of this software and associated documentation files (the 
 *   "Software"), to deal in the Software without restriction, including 
 *   without limitation other than credits to the original author
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom
 *   the Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be 
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 *   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 *   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

// PS: Also never attempt to translate batch file you wrote years ago into C code, it's such a pain in the ass.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 1024

char currentDirectoryName[MAX_PATH_LENGTH];
char currentDirectory[MAX_PATH_LENGTH];

const char* renCommandTemplate = "ren \"%s.bin\" \"%s.narc\"";

void clearConsole() {
    system("PowerShell.exe clear");
}

void getCurrentDirectoryName(char *buffer, char *pathBuffer, DWORD bufferSize) {
    char currentPath[MAX_PATH];
    DWORD length = GetCurrentDirectory(MAX_PATH, currentPath);

    if (length == 0 || length > MAX_PATH) {
        fprintf(stderr, "Error getting current directory\n");
        return;
    }

    // Extract the directory name from the path
    const char *lastSlash = strrchr(currentPath, '\\');
    if (lastSlash != NULL) {
        strncpy(buffer, lastSlash + 1, bufferSize - 1);
        buffer[bufferSize - 1] = '\0'; // Ensure null-termination
    } else {
        strncpy(buffer, currentPath, bufferSize - 1);
        buffer[bufferSize - 1] = '\0'; // Ensure null-termination
    }
    strncpy(pathBuffer, currentPath, bufferSize -1);
    pathBuffer[bufferSize -1] = '\0'; // Ensure null-termination
}

void printInfoLabel() {
    getCurrentDirectoryName(currentDirectoryName, currentDirectory, MAX_PATH_LENGTH);
    printf("I: [%s]: ", currentDirectory);
}

void printBanner() {
    printf("############################################\n");
    printf("#        _        _      _____     _       #\n");
    printf("#    ___| |___ __(_)_ __|___ /  __| |___   #\n");
    printf("#   / __| __| '__| | '_ \\ |_ \\ / _` / __|  #\n");
    printf("#   \\__ \\ |_| |  | | |_) |__) | (_| \\__ \\  #\n");
    printf("#   |___/\\__|_|  |_| .__/____/ \\__,_|___/  #\n");
    printf("#                  |_|                     #\n");
    printf("#                                          #\n");
    printf("############################################\n");
    printf("strip3ds, COPYRIGHT (C) 2024, official.pumpking.dev (Martin LOUVEL)\n");
    printf("using: 3dstool, COPYRIGHT (C) Dnasdw; Wiimms-szs-tools, COPYRIGHT (C) Wiimm (szs.wiimm.de); VGAudio, COPYRIGHT (C) Thealexbarney; NARChive, COPYRIGHT (C) nickworonekin; cgfx2obj, COPYRIGHT (C) smealum.\n");
    printf("WARNING: The extracted resources are property of Nintendo Entertainment. Consider using it for a non-commercial use only.\n\n");
}

void listFiles(const char* extension) {
    struct dirent* entry;
    DIR* dp = opendir(".");

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, extension)) {
            printf(" - [%s]\n", entry->d_name);
        }
    }

    closedir(dp);
}

int isDirectory(const char* path) {
    DWORD attr = GetFileAttributes(path);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return 0; // Invalid path
    }
    return (attr & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}

void ensureDirectoryExists(const char* path) {
    if (!isDirectory(path)) {
        if (_mkdir(path) != 0) {
            perror("mkdir");
        }
    }
}

void processFilesInFolder(const char* folder, const char* filePattern, const char* commandTemplate) {
    char command[MAX_PATH_LENGTH];
    snprintf(command, MAX_PATH_LENGTH, "dir \"%s\\%s\" /b /a-d", folder, filePattern);

    FILE* fp = _popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return;
    }

    char fileName[MAX_PATH_LENGTH];
    while (fgets(fileName, sizeof(fileName), fp)) {
        // Remove newline character
        fileName[strcspn(fileName, "\r\n")] = 0;

        if (commandTemplate[0] == 'r' && commandTemplate[1] == 'e' && commandTemplate[2] == 'n') {
            // Extract base filename without extension
            char baseName[MAX_PATH_LENGTH];
            snprintf(baseName, MAX_PATH_LENGTH, "%s\\%s", folder, fileName);
            char *dot = strrchr(baseName, '.');
            if (dot) {
                *dot = '\0';  // Remove extension
            }
            snprintf(command, MAX_PATH_LENGTH, commandTemplate, baseName);
            printInfoLabel();
            system(command);
            printf("   - Renamed %s -> %s.bin.narc!\n", fileName, baseName);
        } else {
            snprintf(command, MAX_PATH_LENGTH, commandTemplate, fileName);
            printInfoLabel();
            printf("Executing: %s\n", command);
            system(command);
            printInfoLabel();
            printf("Done.\n");
        }
    }

    _pclose(fp);
}

void processRom(const char* filename, const char* dstool, const char* wszst, const char* vgaudio, const char* narchive, const char* caesar) {
    char command[MAX_PATH_LENGTH];

    printf("I: file \"%s\" detected as a 3DS ROM! Extracting right now...\n", filename);

    printInfoLabel();
    printf("Running 3dstool.exe to extract the .cxi and .cfa files...\n");
    snprintf(command, MAX_PATH_LENGTH, "%s -xvtf cci \"%s\" --header \"%s-ncsdheader.bin\" -0 \"%s-game.cxi\" -1 \"%s-manual.cfa\" -2 \"%s-dlpchild.cfa\"",
             dstool, filename, filename, filename, filename, filename);
    system(command);
    printInfoLabel();
    printf("Done.\n");

    printInfoLabel();
    printf("Running 3dstool.exe to extract decrypted data from the .cxi file...\n");
    snprintf(command, MAX_PATH_LENGTH, "%s -xvtf cxi \"%s-game.cxi\" --header \"%s-ncchheader.bin\" --exh \"%s-exheader.bin\" --exefs \"%s-exefs.bin\" --romfs \"%s-romfs.bin\" --logo \"%s-logo.bcma.lz\" --plain \"%s-plain.bin\"",
             dstool, filename, filename, filename, filename, filename, filename, filename);
    system(command);
    printInfoLabel();
    printf("Done.\n");

    printInfoLabel();
    printf("Running 3dstool.exe to extract the cooked resource files from the romfs file and the exefs file...\n");
    snprintf(command, MAX_PATH_LENGTH, "%s -xvtf romfs \"%s-romfs.bin\" --romfs-dir \"%s-romfs\"",
             dstool, filename, filename);
    system(command);
    printInfoLabel();
    printf("Done 1/2...\n");
    snprintf(command, MAX_PATH_LENGTH, "%s -xvtf exefs \"%s-exefs.bin\" --header \"%s-exheader.bin\" --exefs-dir \"%s-exefs\"",
             dstool, filename, filename, filename);
    system(command);
    printInfoLabel();
    printf("Done 2/2...\n");
    printInfoLabel();
    printf("Done.\n");

    char subfolderPath[MAX_PATH_LENGTH];
    snprintf(subfolderPath, MAX_PATH_LENGTH, ".\\%s-romfs", filename);
    ensureDirectoryExists(subfolderPath);

    struct dirent* subentry;
    DIR* subdp = opendir(subfolderPath);

    if (subdp == NULL) {
        perror("opendir subfolder");
        return;
    }

    printInfoLabel();
    printf("Moved into .\\%s-romfs for future operations.\n", filename);

    printInfoLabel();
    printf("Opening each subfolder to extract resources...\n");

    while ((subentry = readdir(subdp))) {
        if (strcmp(subentry->d_name, ".") != 0 && strcmp(subentry->d_name, "..") != 0) {
            char subfolder[MAX_PATH_LENGTH];
            char processFolder[MAX_PATH_LENGTH];
            snprintf(subfolder, MAX_PATH_LENGTH, "%s\\%s", subfolderPath, subentry->d_name);

            if (isDirectory(subfolder)) {
                printInfoLabel();
                printf("Processing folder \"%s\"...\n", filename, subfolder);

                // Extract .bin files from .szs files
                printInfoLabel();
                printf("Running wszst.exe to extract .bin files from .szs files...\n");
                snprintf(command, MAX_PATH_LENGTH, "%s DECOMPRESS \"%s\\%%s\"", wszst, subfolder);
                processFilesInFolder(subfolder, "*.szs", command);

                // Rename .bin files to .narc
                printInfoLabel();
                printf("Renaming each .bin file to .narc...\n");
                snprintf(processFolder, MAX_PATH_LENGTH, subfolder);
                processFilesInFolder(processFolder, "*.bin", renCommandTemplate);

                // Extract files from .narc
                printInfoLabel();
                printf("Running Narchive.exe to extract the files from .narc files into components...\n");
                snprintf(command, MAX_PATH_LENGTH, "%s extract \"%s\\%%s\" -o \"%s\"", narchive, subfolder, subfolder);
                processFilesInFolder(subfolder, "*.narc", command);

                // Extract .wav files from .bcstm
                printInfoLabel();
                printf("Running VGAudioCli.exe to extract .wav files from .bcstm files...\n");
                snprintf(command, MAX_PATH_LENGTH, "%s \"%s\\%%s\" \"%s\\%%~nxs.wav\"", vgaudio, subfolder, subfolder);
                processFilesInFolder(subfolder, "*.bcstm", command);

                // Extract sounds from .bcsar
                printInfoLabel();
                printf("Running caesar.exe to extract sounds from .bcsar files...\n");
                snprintf(command, MAX_PATH_LENGTH, "%s \"%s\\%%s\"", caesar, subfolder);
                processFilesInFolder(subfolder, "*.bcsar", command);
            }
        }
    }
    closedir(subdp);
}

int main() {
    clearConsole();
    printBanner();
    printf("The following files will be extracted:\n");

    // List .3ds and .cci files
    listFiles(".3ds");
    listFiles(".cci");
    printf("\n");

    // Define paths to dependencies
    char dstool[MAX_PATH_LENGTH] = ".\\dependencies\\3dstool\\3dstool.exe";
    char wszst[MAX_PATH_LENGTH] = ".\\dependencies\\szs64\\bin\\wszst.exe";
    char vgaudio[MAX_PATH_LENGTH] = ".\\dependencies\\VGAudio\\VGAudioCli.exe";
    char narchive[MAX_PATH_LENGTH] = ".\\dependencies\\Narchive-1.0.2\\Narchive.exe";
    char caesar[MAX_PATH_LENGTH] = ".\\dependencies\\caesar\\caesar.exe";

    // Continue prompt
    printf("Continue? (y/n) ");
    char choice = getchar();
    getchar(); // to consume newline

    if (choice != 'y' && choice != 'Y') {
        return 0;
    }

    // Safe start prompt
    printf("Would you like to run the dependency's `safe-start` program? (This has to be done only once per user.) (y/n) ");
    choice = getchar();
    getchar(); // to consume newline

    if (choice == 'y' || choice == 'Y') {
        system(".\\dependencies\\szs64\\windows-install.exe");
    }

    // Process each .3ds and .cci file
    struct dirent* entry;
    DIR* dp = opendir(".");

    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".3ds") || strstr(entry->d_name, ".cci")) {
            processRom(entry->d_name, dstool, wszst, vgaudio, narchive, caesar);
            printInfoLabel();
            printf("Running cgfx2obj through automated process to extract obj files...\n");
            system(".\\dependencies\\python\\python.exe .\\dependencies\\cgfx2obj\\process.py");
            printInfoLabel();
            printf("Done.");
        }
    }

    closedir(dp);

    printf("\nThere are no more .3ds files to extract.\n");
    system("pause");

    return 0;
}
