#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define MAX_BUFFER 512

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs <command> [args...]\n");
        exit(1);
    }

    char buf[MAX_BUFFER];
    char *new_argv[MAXARG];
    int i, n;
    int pid;

    // Copy original arguments to new_argv
    for (i = 0; i < argc - 1; i++) {
        new_argv[i] = argv[i + 1];
    }

    while (1) {
        // Read a line from standard input
        i = 0;
        while (1) {
            n = read(0, &buf[i], 1);
            if (n < 0) {
                fprintf(2, "xargs: read error\n");
                exit(1);
            }
            if (n == 0 || buf[i] == '\n') {
                break;
            }
            i++;
        }
        if (n == 0 && i == 0) {
            // End of input
            break;
        }
        buf[i] = 0; // Null terminate the string

        // Add the line to new_argv
        new_argv[argc - 1] = buf;
        new_argv[argc] = 0; // Null terminate the argument list

        // Fork and exec
        pid = fork();
        if (pid < 0) {
            fprintf(2, "xargs: fork failed\n");
            exit(1);
        }
        if (pid == 0) {
            exec(new_argv[0], new_argv);
            fprintf(2, "xargs: exec %s failed\n", new_argv[0]);
            exit(1);
        } else {
            wait(0);
        }
    }
    exit(0);
}
