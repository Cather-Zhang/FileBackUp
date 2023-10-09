#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <pwd.h>
#include <grp.h>

#include "tester.h"


int copyFilesAndDirectories(char* sourcePath, char* targetPath) {
    struct dirent *dp;
    const char* pathSlash = "/";
    char newSourcePath[256];
    char newTargetPath[256];

    DIR *sourceDirectory = opendir(sourcePath);
    if (!sourceDirectory) {
        printf("ERROR: Source Directory %s Doesn't Exist\n", sourcePath);
        return 1;
    }

    DIR* targetDirectory = opendir(targetPath);
    if (!targetDirectory) {
        perror("Unable to open target directory");
        return 1;
    }

    while ((dp = readdir(sourceDirectory)) != NULL) {
        //skipping . and .. directories
        if ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0)) {
            continue;
        }

        //printf("%s/%s\n", sourcePath, dp->d_name);

        snprintf(newSourcePath, sizeof(newSourcePath), "%s/%s", sourcePath, dp->d_name);
        snprintf(newTargetPath, sizeof(newTargetPath), "%s/%s", targetPath, dp->d_name);
        //printf("NEW PATHS: %s to %s\n", newSourcePath, newTargetPath);

        // If it is a folder, we recursively copy sub-directories
        if (dp->d_type == DT_DIR) {
            //printf("%s is a directory, recursively calling the function\n", newSourcePath);
            copyFolderStats(newSourcePath, newTargetPath);
            copyFilesAndDirectories(newSourcePath, newTargetPath);
        }
        else {
            //printf("It is a file\n");
            FILE* sourceFile = fopen(newSourcePath, "rb");
            if (sourceFile == NULL) {
                printf("Error opening source file to be copied!\n");
                return 1;
                //continue;
            }

            // Get the source file's stats
            struct stat sourceStat;
            if (stat(newSourcePath, &sourceStat) == -1) {
                perror("Failed to get source file stats");
                fclose(sourceFile);
                return 1;
            }

            FILE* targetFile = fopen(newTargetPath, "wb");
            
            if (targetFile == NULL) {
                printf("Error opening or creating target file\n");
                return 1;
                //continue;
            }

            //printf("copying");
            char buffer[4096];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
                fwrite(buffer, 1, bytes_read, targetFile);
                //printf(".");
            }

            //printf("File %s copied successfully\n", newSourcePath);


            // Copy file ownership and permission
            //copyFileStats(newSourcePath, newTargetPath);

            fclose(sourceFile);
            fclose(targetFile);
        }
    }

    closedir(sourceDirectory);
    closedir(targetDirectory);

    return 0;
}

int copyFileStats(char* source, char* target) {
    if (chown(newTargetPath, sourceStat.st_uid, sourceStat.st_gid) == -1) {
        perror("Failed to copy file ownership");
        return 1;
    }
    if (fchmod(targetFile, sourceStat.st_mode) == -1) {
        perror("Failed to copy file permissions");
        return 1;
    }

    // Copy file timestamps
    struct utimbuf utime_buf;
    utime_buf.actime = sourceStat.st_atime;
    utime_buf.modtime = sourceStat.st_mtime;
    if (utime(targetFile, &utime_buf) == -1) {
        perror("Failed to copy file timestamps");
        return 1;
    }
    return 0;
}

int copyFolderStats(char* source, char* target) {
    struct stat sourceStat;
    struct utimbuf utimeBuf;

    if (stat(source, &sourceStat) == -1) {
        perror("Error getting folder stats");
        return -1; // Handle the error as needed
    }

    // Create the destination folder with the same permissions as the source folder
    if (mkdir(target, sourceStat.st_mode) == -1) {
        perror("Error creating destination folder");
        return -1; // Handle the error as needed
    }

    // Set ownership of the destination folder to match the source folder
    if (chown(target, sourceStat.st_uid, sourceStat.st_gid) == -1) {
        perror("Error setting folder ownership");
        return -1; // Handle the error as needed
    }

    // Set timestamps (atime and mtime) of the destination folder to match the source folder
    utimeBuf.actime = sourceStat.st_atime;
    utimeBuf.modtime = sourceStat.st_mtime;
    if (utime(target, &utimeBuf) == -1) {
        perror("Error setting folder timestamps");
        return -1; // Handle the error as needed
    }
    return 0;
}

int main(int argc, char *argv[]) {
    run_tests();

    printf("Starting file backup...\n");
    if (argc < 2) {
        printf("Usage: %s <source_dir> <target_dir (optional)>\n", argv[0]);
    }
    const char* target_suffix = "_backup";
    char* sourceDirectoryName = argv[1];

    char* targetDirectoryName = strndup(sourceDirectoryName, strlen(sourceDirectoryName)+1);
    strcat(targetDirectoryName, target_suffix);

    //TODO: can add an optional 
    //targetDirectoryName = (argc == 3) ? argv[2] : strcat(sourceDirectoryName, target_suffix);
    
    printf("Source directory: %s\n", sourceDirectoryName);
    printf("Target directory: %s\n", targetDirectoryName);

    // Create target folder first
    if (copyFolderStats(sourceDirectoryName, targetDirectoryName) != 0) {
        perror("Unable to create backup folder with same stats as source!");
        return 1;
    }

    if (copyFilesAndDirectories(sourceDirectoryName, targetDirectoryName) != 0) {
        perror("Unable to backup folder!");
        return 1;
    }

    return 0;
}