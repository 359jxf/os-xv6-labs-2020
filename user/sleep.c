#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Please provide one integer argument for sleep duration.\n");
        exit(1);
    }

    int sleep_duration = atoi(argv[1]);
    if (sleep_duration <= 0) {
        printf("Error: The argument must be a positive integer.\n");
        exit(1);
    }

    printf("Sleeping for %d seconds...\n", sleep_duration);
    sleep(sleep_duration);
    printf("Awake now!\n");

    exit(0);
}
