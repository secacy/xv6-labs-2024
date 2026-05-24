# gdb
启动gdb：
1. 在一个终端窗口中运行 `make qemu-gdb`
2. 然后在另一个窗口中启动gdb：`gdb-multiarch`

退出gdb: `(gdb) q`
退出qemu: ctrl+a，松开后，再按x

设置断点：
```
(gdb) b user/secret.c:20
```

设置一个断点，然后按 'c'（继续），xv6 就会运行直到到达断点。

将 layout 窗口一分为二，显示 gdb 在源代码中的位置：
```bash
(GDB) layout src 
```

打印栈回溯：
```bash
(gdb) backtrace 
```

打印进程名称：
```bash
(gdb) p p->name
```
打印PID：
```
(gdb) p p->pid
```


# syscall lab

## system call tracing

判断num的第n个二进制位是不是1：if (num & (1 << n)) {}


## attack xv6

此处实验参考了：https://github.com/rinevard/MIT-6.828-2024/blob/main/notes/lab2-syscall.md

keys: 
1. 进程退出时，os会回收内存；执行 pipe() 和 fork() 都要分配内存 --> 我们请求一些物理页时，要在这些物理页中找到字符串，但不确定字符串所在页表是哪一个，因此需要遍历寻找
2. 应该先把内存分配和内存释放的代码看看(kernel/kalloc.c) -> 释放页面时会创建run链表节点，这里空闲页本身是被当成链表节点使用的，没有额外分配链表节点，然后加锁并插入空闲链表。 -> 因此不能通过匹配开头找到字符串

```c
  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
```

也就是说，一页空闲内存长这样：
+--------------------+
| struct run { next }|
|                    |
| 剩下的空间没用      |
|                    |
+--------------------+
其中指针占8个字节。
