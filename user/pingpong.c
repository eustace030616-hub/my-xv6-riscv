#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p1[2];
    int p2[2];
    char buf[1];

    pipe(p1);
    pipe(p2);

    if (fork() == 0) {

        int pid = getpid();

        close(p1[1]);
        close(p2[0]);
        
        read(p1[0], buf, 1);
        printf("%d: received ping\n", pid);

        write(p2[1], "b", 1);
        close(p2[1]);
        close(p1[0]);

        exit(0);
    } else {

        int pid = getpid();

        close(p1[0]);
        close(p2[1]);

        write(p1[1], "a", 1);
        close(p1[1]);

        read(p2[0], buf, 1);
        printf("%d: received pong\n", pid);
        close(p2[0]);

        wait(0);
        exit(0);
    }
}

