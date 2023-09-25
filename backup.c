#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define PATH_MAX 256

void checkNewPath(char* path) {
    int result = strcmp(path, "dummy_files_backup");
    assert(result == 0);
}

void listFilesAndDirectories(char* sourcePath) {
    struct dirent *dp;

    DIR *sourceDirectory = opendir(sourcePath);
    if (!sourceDirectory) {
        printf("Source Directory Doesn't Exist\n");
        return;
    }
    
    while ((dp = readdir(sourceDirectory)) != NULL) {
        //skipping . and .. directories
        if ((strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0)) {
            printf("%s/%s\n", sourcePath, dp->d_name);
        }

        if (dp->d_type == DT_DIR) {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", sourcePath, dp->d_name);
            listFilesAndDirectories(path);
        }
    }


}

int main(int argc, char *argv[]) {
    printf("Starting file backup...\n");
    if (argc < 2) {
        printf("Usage: %s <source_dir> <target_dir (optional)>\n", argv[0]);
    }
    const char* target_suffix = "_backup";
    char* sourceDirectoryName = argv[1];
    //char* targetDirectoryName = malloc((strlen(sourceDirectoryName)+1)*sizeof(char));
    //strcpy(targetDirectoryName, sourceDirectoryName);
    //strcat(targetDirectoryName, target_suffix);
    
    char* targetDirectoryName = strndup(sourceDirectoryName, strlen(sourceDirectoryName));
    strcat(targetDirectoryName, target_suffix);

    //TODO: can add an optional 
    //targetDirectoryName = (argc == 3) ? argv[2] : strcat(sourceDirectoryName, target_suffix);
    
    printf("Source directory: %s\n", sourceDirectoryName);
    printf("Target directory: %s\n", targetDirectoryName);

    listFilesAndDirectories(sourceDirectoryName);

    return 0;
}