#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tester.h"


void checkNewPath(char* path) {
    int result = strcmp(path, "dummy_files_backup");
    assert(result == 0);
}

void copyFilesAndDirectories(char* sourcePath, char* targetPath) {
    struct dirent *dp;
    const char* pathSlash = "/";
    char newSourcePath[256];
    char newTargetPath[256];
    printf("Input: %s and %s\n", sourcePath, targetPath);

    DIR *sourceDirectory = opendir(sourcePath);
    if (!sourceDirectory) {
        printf("ERROR: Source Directory %s Doesn't Exist\n", sourcePath);
        return;
    }
    
    struct stat info;

    if (stat(targetPath, &info) != 0) {
        printf("Target Directory %s Doesn't Exist, creating one now\n", targetPath);
        mkdir(targetPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        // Try to open the target dir again
        
    }
    DIR* targetDirectory = opendir(targetPath);
    if (!targetDirectory) {
        printf("ERROR: Target Directory %s Doesn't Exist\n", targetPath);
        return;
    }
    
    
    while ((dp = readdir(sourceDirectory)) != NULL) {
        //skipping . and .. directories
        if ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0)) {
            continue;
        }

        printf("%s/%s\n", sourcePath, dp->d_name);

/*
        char* newSourcePath = strndup(sourcePath, strlen(sourcePath)+1);
        strcat(newSourcePath, pathSlash);
        strcat(newSourcePath, dp->d_name);

        char* newTargetPath = strndup(targetPath, strlen(targetPath)+1);
        strcat(newTargetPath, pathSlash);
        strcat(newTargetPath, dp->d_name);
*/
        snprintf(newSourcePath, sizeof(newSourcePath), "%s/%s", sourcePath, dp->d_name);
        snprintf(newTargetPath, sizeof(newTargetPath), "%s/%s", targetPath, dp->d_name);
        printf("NEW PATHS: %s to %s\n", newSourcePath, newTargetPath);

        // If it is a folder, we recursively copy sub-directories
        if (dp->d_type == DT_DIR) {
            printf("%s is a directory, recursively calling the function\n", newSourcePath);
            copyFilesAndDirectories(newSourcePath, newTargetPath);
        }
        else {
            printf("It is a file\n");
            FILE* sourceFile = fopen(newSourcePath, "rb");
            if (sourceFile == NULL) {
                printf("Error opening source file to be copied!\n");
                //continue;
            }
            
            FILE* targetFile = fopen(newTargetPath, "wb");
            
            if (targetFile == NULL) {
                printf("Error opening or creating target file\n");
                //continue;
            }

            printf("copying");
            char buffer[4096];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
                fwrite(buffer, 1, bytes_read, targetFile);
                //printf(".");
            }

            printf("File %s copied successfully\n", newSourcePath);

            fclose(sourceFile);
            fclose(targetFile);
        }
    }

    closedir(sourceDirectory);
    closedir(targetDirectory);

}

int main(int argc, char *argv[]) {
    run_tests();

    printf("Starting file backup...\n");
    if (argc < 2) {
        printf("Usage: %s <source_dir> <target_dir (optional)>\n", argv[0]);
    }
    const char* target_suffix = "_backup";
    char* sourceDirectoryName = argv[1];
    //char* targetDirectoryName = malloc((strlen(sourceDirectoryName)+1)*sizeof(char));
    //strcpy(targetDirectoryName, sourceDirectoryName);
    //strcat(targetDirectoryName, target_suffix);
    
    char* targetDirectoryName = strndup(sourceDirectoryName, strlen(sourceDirectoryName)+1);
    strcat(targetDirectoryName, target_suffix);

    //TODO: can add an optional 
    //targetDirectoryName = (argc == 3) ? argv[2] : strcat(sourceDirectoryName, target_suffix);
    
    printf("Source directory: %s\n", sourceDirectoryName);
    printf("Target directory: %s\n", targetDirectoryName);

    copyFilesAndDirectories(sourceDirectoryName, targetDirectoryName);

    return 0;
}