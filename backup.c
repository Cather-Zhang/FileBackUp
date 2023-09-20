#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>

void checkNewPath(char *path) {
    int result = strcmp(path, "dummy_files_copy");
    assert(result == 0);
}

int main(int argc, char *argv[]) {
    char path[100] = "dummy_files";
    DIR *directory;
    directory = opendir(path);
    if (directory == NULL) {
        printf("Directory doesn't exist");
    }

    strcat(path, "_copy");
    printf("%s\n", path);

    checkNewPath(path);

    return 0;
}