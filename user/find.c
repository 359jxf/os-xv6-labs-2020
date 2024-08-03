#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // 打开目录
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  // 获取目录状态
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // 根据文件类型处理
  switch (st.type) {
  case T_FILE:
    // 如果是文件，检查文件名是否匹配
    if (strcmp(path + strlen(path) - strlen(filename), filename) == 0) {
      printf("%s\n", path);
    }
    break;

  case T_DIR:
    // 如果是目录，准备递归搜索子目录
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    // 读取目录内容
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      // 跳过 "." 和 ".."
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      find(buf, filename);  // 递归调用
    }
    break;
  }
  close(fd);  // 关闭目录
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "Usage: find <directory> <filename>\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
