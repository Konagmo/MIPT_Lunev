#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    struct stat sb1;
    struct stat sb2;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pathname>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[1], O_RDONLY);
    //fd2 = 3;

    if (fstat(fd1, &sb1) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if (fstat(fd2, &sb2) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("fd1 - %d, 1st I-node number: %ld,
     file: %d\n", fd1, (long) sb1.st_ino, sb1.st_dev);
    printf("fd2 - %d, 2nd I-node number: %ld,
     file: %d\n", fd2, (long) sb2.st_ino, sb2.st_dev);



    /*
    printf("File type:                ");

    switch (sb1.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");        break;
    case S_IFCHR:  printf("character device\n");    break;
    case S_IFDIR:  printf("directory\n");           break;
    case S_IFIFO:  printf("FIFO/pipe\n");           break;
    case S_IFLNK:  printf("symlink\n");             break;
    case S_IFREG:  printf("regular file\n");        break;
    case S_IFSOCK: printf("socket\n");              break;
    default:       printf("unknown?\n");            break;
    }

    printf("I-node number:            %ld\n",
     (long) sb1.st_ino);
    printf("Mode:                     %lo (octal)\n",
           (unsigned long) sb1.st_mode);

    printf("Link count:               %ld\n",
     (long) sb1.st_nlink);
    printf("Ownership:                UID=%ld  GID=%ld\n",
           (long) sb1.st_uid, (long) sb1.st_gid);

    printf("Preferred I/O block size: %ld bytes\n",
           (long) sb1.st_blksize);
    printf("File size:                %lld bytes\n",
           (long long) sb1.st_size);
    printf("Blocks allocated:         %lld\n",
           (long long) sb1.st_blocks);

    printf("Last status change:       %s",
     ctime(&sb1.st_ctime));
    printf("Last file access:         %s",
     ctime(&sb1.st_atime));
    printf("Last file modification:   %s",
     ctime(&sb1.st_mtime));
    */

    exit(EXIT_SUCCESS);
}

