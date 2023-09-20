# FileBackUp

Project Title: Simple File Backup Utility

Project Description:

Create a simple file backup utility that allows users to easily back up specified directories to a designated backup location. The utility will consist of three main components: a C program, a Makefile for building the program, and a Bash script for automating backup tasks.

Requirements:

    C Program (backup.c):
        Write a C program that accepts command-line arguments to specify the source directory and the destination directory for the backup.
        The program should create a backup of all files and directories in the source directory and its subdirectories into the destination directory.
        Use system calls like mkdir, open, read, write, and close to perform file operations.
        Implement error handling to deal with cases where the source directory doesn't exist or there are permission issues.

    Makefile (Makefile):
        Create a Makefile that compiles the C program and generates an executable file.
        Include rules for compiling and linking the program.
        Define variables for compiler options and flags.
        Create a "clean" target to remove generated files.

    Bash Script (backup.sh):
        Write a Bash script that automates the backup process using the C program.
        The script should take user input for source and destination directories.
        Ensure that the destination directory is created if it doesn't exist.
        Call the C program with the provided source and destination directories as arguments.

Optional Enhancements:

    Implement backup rotation: Keep a specified number of backup copies and delete older ones.
    Add options to exclude certain file types or directories from the backup.
    Create a log file that records backup operations with timestamps.
    Add multi-threading to enhance the efficiency of this process.

Example Usage:
```
$ ./backup.sh
Enter source directory: /path/to/source
Enter destination directory: /path/to/backup
Backup completed successfully!
```

Note: Be cautious when working with file operations in C, especially when handling user input and system calls. Always validate user input and handle errors gracefully.

This project combines C for low-level file operations, a Makefile for building the program, and a Bash script for automation. It provides a practical solution for backing up files and directories.

Docker Command (https://youtu.be/pr-pYhMDhgM):
```
docker run -h <username> -v "<PathToFolder>":/root/host --rm -it ubuntu:20.04
```

Install make within docker container:
```
apt-get update && apt-get install -y gcc
```

Install make within docker container:
```
apt-get update && apt-get install make
```

Set the backup bash script to correct execute permission:
```
chmod +x backup.sh
```
