#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    char buf[1];
    int pid;

    // 创建两个管道
    pipe(p1); // p1[0] 为读端，p1[1] 为写端
    pipe(p2); // p2[0] 为读端，p2[1] 为写端

    pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error\n");
        exit(1);
    }

    if (pid == 0) { // 子进程
        close(p1[1]); // 关闭父进程到子进程的写端
        close(p2[0]); // 关闭子进程到父进程的读端

        // 读取父进程发送的字节
        read(p1[0], buf, 1);
        printf("%d: received ping\n", getpid());

        // 向父进程发送字节
        write(p2[1], buf, 1);

        // 关闭剩余的文件描述符
        close(p1[0]);
        close(p2[1]);

        exit(0);
    } else { // 父进程
        close(p1[0]); // 关闭父进程到子进程的读端
        close(p2[1]); // 关闭子进程到父进程的写端

        // 向子进程发送一个字节
        buf[0] = 'A';
        write(p1[1], buf, 1);

        // 读取子进程返回的字节
        read(p2[0], buf, 1);
        printf("%d: received pong\n", getpid());

        // 关闭剩余的文件描述符
        close(p1[1]);
        close(p2[0]);

        // 等待子进程结束
        wait(0);

        exit(0);
    }
}
