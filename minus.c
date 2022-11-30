#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

volatile int flag = 0;

void Input(double buf[2]){
    printf("Minus process: input %f, %f\n", buf[0], buf[1]);
}

void Output(double res){
    printf("Minus process: output %f\n", res);
}

void sigint_handler(int sig) {
        int f;
        double res, buf[2];

        f = open("./file", O_RDWR, 0);
        read(f, buf, sizeof(double)*2);

        Input(buf);

        res = buf[0] - buf[1];
        res = buf[0] - buf[1];

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

