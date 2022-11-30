#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

volatile int flag = 0;


void Input(double res){
    printf("Sqrt process: input %f\n", res);
}

void Output(double res){
    printf("Sqrt process: output %f\n", res);
}

void sigint_handler(int sig) {
        int f;
        double res;

        f = open("./file", O_RDWR, 0);
        read(f, &res, sizeof(double));

        Input(res);

        res = sqrt(res);

        Output(res);

        lseek(f, 0x00, SEEK_SET);
        write(f, &res, sizeof(double));
        close(f);

        flag = sig;
        kill(getppid(), SIGUSR1);
}

int main() {
    struct sigaction sa;
    sigset_t set;

    sigemptyset(&set); // Clear set of signals
    sigaddset(&set, SIGUSR1); // Add signal to set
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sa.sa_mask = set;
    sigaction(SIGUSR1, &sa, NULL);
    while(1)
        pause();
}

