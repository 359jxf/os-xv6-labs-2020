#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int p_left[2]) {
  int prime;
  close(p_left[1]);  // 关闭左边管道的写端
  if (read(p_left[0], &prime, sizeof(prime)) == 0) {
    close(p_left[0]);
    exit(0);
  }

  printf("prime %d\n", prime);

  int p_right[2];
  pipe(p_right);

  if (fork() == 0) {
    // 子进程
    close(p_left[0]);  // 关闭左边管道的读端
    sieve(p_right);    // 递归处理右边的管道
  } else {
    // 父进程
    close(p_right[0]);  // 关闭右边管道的读端

    int num;
    while (read(p_left[0], &num, sizeof(num)) > 0) {
      if (num % prime != 0) {
        write(p_right[1], &num, sizeof(num));
      }
    }

    close(p_left[0]);  // 关闭左边管道的读端
    close(p_right[1]); // 关闭右边管道的写端
    wait(0);           // 等待子进程结束
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  int p[2];
  pipe(p);

  if (fork() == 0) {
    // 子进程
    sieve(p);
  } else {
    // 父进程
    close(p[0]);  // 关闭左边管道的读端

    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(i));
    }

    close(p[1]);  // 关闭左边管道的写端
    wait(0);      // 等待子进程结束
  }

  exit(0);
}
