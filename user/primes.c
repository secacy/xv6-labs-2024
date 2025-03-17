/* 为 xv6 编写一个并发的素数筛程序 */

// 对于每个素数，您将安排创建一个进程，该进程通过管道从其左侧邻居读取，并通过另一个管道写入其右侧邻居。

// tips:
// 由于 xv6 的文件描述符和进程数量有限，因此第一个进程可以在 280 处停止。
// 请小心关闭进程不需要的文件描述符，否则您的程序将在第一个进程达到 280 之前耗尽 xv6 的资源。

#include "kernel/types.h"  //uint64
#include "user/user.h"  //struct stat //system calls //ulib.c //umalloc.c

void
sieve(int p[2]) 
{
    // 获取第一个数据
    int first;
    int n = read(p[0], &first, sizeof(first));

    // 如果接收不到数据时结束进程
    if (n == 0) {
        close(p[0]);
        exit(0);
    }
    if (n < 0) {
        fprintf(2, "read error\n");
        exit(1);
    }

    // 打印第一个数据
    printf("prime %d\n", first);

    // 创建新的管道
    int new_p[2];
    pipe(new_p);

    // 创建新的进程，传递给下一个
    if (fork() > 0) {
        close(new_p[0]); //父进程关闭读取端
        int num;
        while ((n = read(p[0], &num, sizeof(num))) > 0) {
            if (num % first != 0) write(new_p[1], &num, sizeof(num));
        }
        if(n < 0) {
            fprintf(2, "read error\n");
            exit(1);
        }

        // 在进程完成所有的写入后关闭读写
        close(p[0]);
        close(new_p[1]);
        wait(0);
    } else {
        close(p[0]); //子进程关闭父进程的读取端
        close(new_p[1]); //子进程关闭写入端
        sieve(new_p);
    }
}

int 
main(void)
{
    //创建管道
    int p[2];
    pipe(p);

    if (fork() > 0) {
        close(p[0]); //父进程关闭读取端
        // 第一个进程将数字 2 到 280 输入管道
        for (int i = 2; i <= 280; i++) {
            int k = i;
            write(p[1], &k, sizeof(k));
        }
        close(p[1]);  //关闭写入端表示文件结束
        wait(0); //父进程结束时应该等待子进程终止
    } else {
        //fork()时，文件描述符的表单也从父进程拷贝到子进程，所以这个pipe事实上有4个文件描述符
        close(p[1]); //子进程关闭写入端
        // 来自deepseek的建议：利用递归
        sieve(p);
    }

    exit(0);
}