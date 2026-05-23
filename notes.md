# gdb
启动gdb：
1. 在一个终端窗口中运行 `make qemu-gdb`
2. 然后在另一个窗口中启动gdb：`gdb-multiarch`

退出gdb: `(gdb) q`
退出qemu: ctrl+a，松开后，再按x


设置一个断点，然后按 'c'（继续），xv6 就会运行直到到达断点

```bash
(GDB) layout src 
(GDB) backtrace 
```
layout src 该命令将 layout 窗口一分为二，显示 gdb 在源代码中的位置。 
backtrace 打印栈回溯。

通常你需要查看更多信息以追踪引发恐慌的问题。 例如，当内核恐慌时，要查找哪个用户进程正在运行，你可以打印进程名称：
```
(gdb) p p->name
```
打印PID：
```
(gdb) p p->pid
```


## syscall lab

判断num的第n个二进制位是不是1：if (num & (1 << n)) {}
