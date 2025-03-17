/* xarg的参数是要运行的命令，它从标准输入读取行，然后对每一行运行命令，并将该行附加到命令的参数中。 */

//tips:
//使用 fork 和 exec 在每一行输入中调用命令。在父进程中使用 wait 等待子进程完成命令。
//要读取输入的各行，请一次读取一个字符，直到出现换行符（“\n”）。
//kernel/param.h 声明了 MAXARG


#include "kernel/types.h" // uint... <- uint strlen
#include "kernel/param.h" // MAXARG -- max exec arguments
#include "user/user.h"  //系统调用: fork ...  ulib: strlen... umalloc: malloc ...

void
copy(char **dest, char *org)
{
    *dest = malloc(strlen(org) + 1);
    strcpy(*dest, org);
}

int
readline(int i, char *argv[])
{
    int d = 1024;
    char buf[d];
    int j = 0;
    // 读取一行
    while (read(0, buf + j, 1)) {
        if (buf[j] == '\n') {
            buf[j] = 0;
            break;
        }
        j++;
        if (j == d) {
            fprintf(2, "Parameters are too long!\n");
            exit(1);
        }
    }

    // 没有读取到内容
    if (j == 0) {
        return -1;
    }

    // 遍历行
    int k = 0;
    while (k < j) {
        if (i > MAXARG) {
            fprintf(2, "Too many parameters!\n");
            exit(1);
        }

        //跳过空格
        while ((k < j) && (buf[k] == ' ')) {
            k++;
        }

        int l = k;
        while ((k < j) && buf[k] != ' ') {
            k++;
        }
        buf[k] = 0;
        // copy(&argv[i], buf + l);  //如果只传argv[i]，传的是这个地址，但出了函数没有改变这个地址，我们希望地址是malloc的地址
        argv[i] = malloc(strlen(buf + l) + 1);
        strcpy(argv[i], buf+l);
        i++;
    }

    return i;
}


int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(2, "usage: xargs command [args ...]\n");
        exit(1);
    }

    char *my_argv[MAXARG];
    for (int i = 1; i < argc; i++) {
        my_argv[i-1] = argv[i];
    }

    //读取行并解析行这个参考了博客：
    //https://doraemonzzz.com/2021/07/20/2021-7-20-MIT-6.S081-Operating-System-Enginerring-Lab1-Utilities/#xargs-moderate
    int end;
    while ((end= readline(argc-1, my_argv)) != -1) {
        my_argv[end] = 0;
        if (fork() == 0) {
            exec(my_argv[0], my_argv);
            exit(1);
        } else {
            wait(0);
        }
    }
    exit(0);
}
