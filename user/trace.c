  // user/trace.c                                                               
#include "user.h"                                                             

int main(int argc, char *argv[]) {
    if ( argc < 3) {
        fprintf(2, "wrong input; usage: <int mask> <argument>");
        exit(1);
    }

    trace(atoi(argv[1]));

    if (fork() == 0) {
        exec(argv[2], &argv[2]);
        exit(1);
    } else {
        wait(0);
    }
    exit(0);
}