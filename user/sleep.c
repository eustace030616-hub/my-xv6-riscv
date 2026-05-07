#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2){
        write(2, "error, usage: sleep seconds\n", 30);
        exit(1);
    }
    int n = atoi(argv[1]);

    if(n < 0){
        write(2, "error, invalid input\n", 30);
        exit(1);
    }

    sleep(n);
    exit(0);
}
