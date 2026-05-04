#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void xargs(int argc, char *argv[]) {
    char buf[512];
    int n;

    while ((n = read(0, buf, sizeof(buf))) > 0) {
        for (int i = 0; i <= n; i++) {
            if (buf[i] == '\n') {
                buf[i] = '\0';
                if (fork() == 0) {
                    char *args[32];
                    
                    for (int j = 1; j < argc; j++) {
                        args[j - 1] = argv[j];
                    }
                    args[argc - 1] = buf;
                    args[argc] = 0;

                    exec(argv[1], args);
                    exit(1);
                }
                wait(0);
                break;
            }
        }
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "usage: xargs <command> [args...]\n");
        exit(1);
    } else {
        xargs(argc, argv);
        exit(0);
    }
}