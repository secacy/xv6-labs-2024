#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int p[2];
    int *ptr;
    char buf[512];
    pipe(p);

    if(fork() == 0) {

        //free(ptr); // 释放内存 //需要释放内存不 //这里我没释放
        ptr = (int*) malloc(1);
        if (ptr == 0) {
            fprintf(2, "Memory allocation failed!\n");
            exit(1);
        }
        write(p[1], ptr, 1);
        fprintf(1, "%d: received ping\n", getpid());
    } else {
        wait(0);
        read(p[0], buf, 1);
        fprintf(1, "%d: received pong\n", getpid());
    }

    exit(0);
}
