#include "tester.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

int compareStats(char *source, char *target) {
    struct stat source_stat, target_stat;

    if (stat(source, &source_stat) == -1 || stat(target, &target_stat) == -1) {
        perror("Error getting file stats");
        return -1; // Handle the error as needed
    }

    // Compare the relevant attributes (e.g., size, modification time, etc.)
    if (source_stat.st_size != target_stat.st_size) {
        return 0; // Sizes are different
    }

    if (source_stat.st_mtime != target_stat.st_mtime) {
        return 0; // Modification times are different
    }

    // Add more comparisons for other attributes as needed

    return 1; // Stats are the same
}

void test_copyFileToDirectory() {
    char* sourcePath = "source_directory";
    char* targetPath = "target_directory";
    mkdir(sourcePath, 0700);

    FILE* sourceFile = fopen("source_directory/source_file.txt", "w");
    fprintf(sourceFile, "Test file content");
    fclose(sourceFile);

    copyFilesAndDirectories(sourcePath, targetPath);

    // Check if the file was copied to the target directory
    assert(access("target_directory/source_file.txt", F_OK) != -1);

    // Make sure all stats are copied over as well
    //assert(compareStats(sourcePath, targetPath) == 1);
    //assert(compareStats("source_directory/source_file.txt", "target_directory/source_file.txt") == 1);

    // Clean up
    remove("target_directory/source_file.txt");
    remove("source_directory/source_file.txt");
    rmdir("source_directory");
    rmdir("target_directory");
}

void test_copyEmptyDirectory() {
    char* sourcePath = "empty_directory";
    char* targetPath = "target_directory";
    mkdir(sourcePath, 0700);

    copyFilesAndDirectories(sourcePath, targetPath);

    // Check if the directory was copied to the target directory
    assert(access("target_directory/empty_directory", F_OK) != -1);

    // Clean up
    rmdir("empty_directory");
    rmdir("target_directory/empty_directory");
}

void test_copyNestedDirectories() {
    char* sourcePath = "nested_source_directory";
    char* targetPath = "target_directory";
    mkdir(sourcePath, 0700);

    mkdir("nested_source_directory/subdir1", 0700);
    FILE* file = fopen("nested_source_directory/subdir1/file1.txt", "w");
    fprintf(file, "Test file content");
    fclose(file);

    copyFilesAndDirectories(sourcePath, targetPath);

    // Check if the directories and files were copied correctly
    assert(access("target_directory/subdir1", F_OK) != -1);
    assert(access("target_directory/subdir1/file1.txt", F_OK) != -1);

    // Make sure all stats are copied over as well
    //assert(compareStats(sourcePath, targetPath) == 1);
    //assert(compareStats("nested_source_directory/subdir1", "target_directory/subdir1") == 1);
    //assert(compareStats("nested_source_directory/subdir1/file1.txt", "target_directory/subdir1/file1.txt") == 1);

    // Clean up
    remove("nested_source_directory/subdir1/file1.txt");
    remove("target_directory/subdir1/file1.txt");
    rmdir("nested_source_directory/subdir1");
    rmdir("target_directory/subdir1");
    rmdir("nested_source_directory");
    rmdir("target_directory");
}

void test_copyNonexistentSourceDirectory() {
    char* sourcePath = "nonexistent_source_directory";
    char* targetPath = "target_directory";

    copyFilesAndDirectories(sourcePath, targetPath);

    // Check if the target directory remains unchanged
    assert(access("target_directory", F_OK) == -1);
}

void test_copyToInvalidTargetPath() {
    char* sourcePath = "source_directory";
    char* targetPath = "/invalid/target/directory";

    copyFilesAndDirectories(sourcePath, targetPath);

    // Check if the target directory remains unchanged
    assert(access("/invalid/target/directory", F_OK) == -1);
}

void run_tests() {
    // Run the tests
    test_copyFileToDirectory();
    //test_copyEmptyDirectory();
    test_copyNestedDirectories();
    test_copyNonexistentSourceDirectory();
    test_copyToInvalidTargetPath();

    printf("All tests passed!\n");
}
