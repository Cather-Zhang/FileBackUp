#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

void testAddition() {
    int i = 5;
    assert(i == 5);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return -1;
    }

    printf("Hello World\n");

    const char* sourceDirectory = (char*)malloc(strlen(argv[1]) + 1);
    struct stat dirStat;
    int result = stat(sourceDirectory, *dirStat);

    if (result == 0) {
        if (S_ISDIR(dirStat.st_mode)) {
            printf("The directory '%s' exists.\n", sourceDirectory);
            return 0;
        } else {
            printf("'%s' is not a directory.\n", sourceDirectory);
            return -1;
        }
    }
    else {
        printf("%s is not a directory", sourceDirectory);
        return -1;
    }


    testAddition();

    return 0;
}