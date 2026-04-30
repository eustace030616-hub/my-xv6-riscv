#include "kernel/types.h"
#include "user/user.h"

__attribute__((noreturn))
void sieve_primes(int left_pipe[2]) {
    int prime;
    int n;

    close(left_pipe[1]);

    if (read(left_pipe[0], &prime, sizeof(int)) != sizeof(int)) {
        close(left_pipe[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int right_pipe[2];
    pipe(right_pipe);

    if (fork() == 0) {
        close(left_pipe[0]);
        sieve_primes(right_pipe);
        exit(0);
    } else {
        close(right_pipe[0]);

        while (read(left_pipe[0], &n, sizeof(int)) == sizeof(int)) {
            if (n % prime != 0) {
                write(right_pipe[1], &n, sizeof(int));
            }
        }

        close(right_pipe[1]);
        close(left_pipe[0]);
        wait(0);
        exit(0);
    }
}


int main() {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        sieve_primes(p);
    } else {
        close(p[0]);
        for (int i = 2; i <= 35; i++ ) {
            write(p[1], &i, sizeof(int));
        }

        close(p[1]);
        wait(0);
    }

    exit(0);
}


