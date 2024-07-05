#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXSZ 512

const int CLEN = sizeof(char);

int
main(int argc, char *argv[]) {
    char buf[MAXSZ] = {0};
    char *xargs[MAXARG] = {0};
    int end_flag = 0;
    int j = 0;

    for (int i = 1; i < argc; i++) {
        xargs[i - 1] = argv[i];
    }
    int temp = argc - 1;

    while (!end_flag) {
        j = 0;
        while (1) {
            if (read(0, &buf[j], CLEN) == 0) {
                end_flag = 1;
                break;
            }
            if (buf[j] == '\n') {
                break;
            }
            j++;
        }
        buf[j] = 0; // Null terminate the string

        if (j > 0) {
            xargs[temp] = buf;
            xargs[temp + 1] = 0; // Null terminate the argument list

            if (fork() == 0) {
                exec(argv[1], xargs);
                fprintf(2, "xargs: exec %s failed\n", argv[1]);
                exit(1);
            } else {
                wait(0);
            }
        }
    }
    exit(0);
}
