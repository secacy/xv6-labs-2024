// /* find all the files in a directory tree with a specific name. */

// // tips:
// // use strcmp() to compare strings
// // Use recursion to allow find to descend into sub-directories.
// // 没注意到的坑：在递归搜索子目录时，注意排除 .和 ..。不然可能导致无限递归或重复搜索。

// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/fs.h"
// #include "kernel/fcntl.h"

// void 
// find(char *path, char *file)
// {

//     char buf[512], *p;
//     int fd;
//     struct dirent de;
//     struct stat st;

//     if ((fd = open(path, O_RDONLY)) < 0) {
//         fprintf(2, "find: cannot open %s\n", path);
//         return;
//     }

//     if (fstat(fd, &st) < 0) {
//         fprintf(2, "find: cannot stat %s\n", path);
//         close(fd);
//         return;
//     }

//     if (st.type != T_DIR) {
//         fprintf(2, "find: %s is not dir\n", path);
//         return;
//     }

//     //path is dir
//     if (strlen(path) + 1 +DIRSIZ + 1 > sizeof buf) {
//         printf("find: path too long\n");
//         return;
//     }

//     strcpy(buf, path);
//     p = buf+strlen(buf);
//     *p++ = '/';
//     while (read(fd, &de, sizeof(de)) == sizeof(de)) {
//         if (de.inum == 0)
//             continue;
//         if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
//             continue;
//         }
//         memmove(p, de.name, DIRSIZ);
//         p[DIRSIZ] = 0;
//         if (stat(buf, &st) < 0) {
//             printf("find: cannot stat %s\n", buf);
//             continue;
//         }

//         if (st.type == T_DIR) {
//             find(buf, file);
//         }
//         if (st.type == T_FILE) {
//             if (strcmp(de.name, file) == 0 || strcmp(file, "") == 0)
//                 printf("%s\n", buf);
//         }
//     }
//     close(fd);

// }

// int
// main(int argc, char *argv[])
// {
//     int i;

//     if (argc <= 1) {
//         fprintf(2, "usage: find path [file ...]\n");
//         exit(1);
//     }

//     if (argc <= 2) {
//         find(argv[1], "");
//         exit(0);
//     }

//     for(i = 2; i < argc; i++)
//         find(argv[1], argv[i]);
//     exit(0);
// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void match(const char* path, const char* name){
    //printf("%s %s", path, name);
    int pp = 0;
    int pa = 0;
    while(path[pp] != 0){
        pa = 0;
        int np = pp;
        while(name[pa] != 0){
            if (name[pa] == path[np]){
                pa++;
                np++;
            }
            else
                break;
        }
        if(name[pa] == 0){
            printf("%s\n", path);
            return;
        }
        pp++;
    }
}

void find(char *path, char *name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    
    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){
        case T_FILE:
            // printf("%s %d %d %l\n", path, st.type, st.ino, st.size);
            match(path, name);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                if(de.name[0] == '.' && de.name[1] == 0) continue;
                if(de.name[0] == '.' && de.name[1] == '.' && de.name[2] == 0) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                find(buf, name);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[]){
    if (argc < 3){
        printf("Usage: find [path] [filename]\n");
        exit(-1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
