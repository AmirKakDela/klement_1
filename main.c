#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

pid_t plus, minus, mul, div, sqr;

volatile int flag = 0;

void sigint_handler(int sig) {
    flag = sig;
}

void Output(double x1, double x2){
    printf("X1= %f, X2= %f", x1, x2);
}

double SendRecv(pid_t child, double x, double y){
    double buf[2];
    double res;

    buf[0] = x;
    buf[1] = y;

    int f;

    f = open("./file", O_RDWR, 0);
    write(f, &buf, sizeof(buf));

    close(f);

    kill(child, SIGUSR1);
    pause();

    f = open("./file", O_RDWR, 0);
    read(f, &res, sizeof(double));
    close(f);

    return res;
}

void MainWork(){

    double inputValue_1, inputValue_2, inputValue_3;
    double a, b, c, add, x1, x2, post;

    printf("Enter a, b, c: ");
    scanf("%lf", &inputValue_1);
    scanf("%lf", &inputValue_2);
    scanf("%lf", &inputValue_3);

    a = inputValue_1;
    b = inputValue_2; 
    c = inputValue_3;

    x2 = SendRecv(mul, b, b);
    post = 4;
    add = SendRecv(mul, post, a);
    add = SendRecv(mul, add, c);
    x2 = SendRecv(minus, x2, add);
    x2 = SendRecv(sqr, x2, x2);
    post = 0;
    add = SendRecv(minus, post, b);
    x1 = SendRecv(plus, add, x2);
    x2 = SendRecv(minus, add, x2);
    post = 2;
    add = SendRecv(mul, post, a);
    x1 = SendRecv(div, x1, add);
    x2 = SendRecv(div, x2, add);
    Output(x1, x2);
}

void End(){
   kill(plus, SIGINT);
   kill(minus, SIGINT);
   kill(mul, SIGINT);
   kill(div, SIGINT);
   kill(sqr, SIGINT);
}

void creatProcess(){
    plus = fork();
    if (plus){
        minus = fork();
        if (minus){
            mul = fork();
            if (mul){
                div = fork();
                if (div){
                    sqr = fork();
                    if (sqr){
                        MainWork();
                        End();
                    }
                    else{
                        execl("./sqrt", "", NULL);
                        perror("Error to start sqrt");
                    }
                }
                else{
                    execl("./division", "", NULL);
                    perror("Error to start division");
                }
            }
            else{
                execl("./multiplication", "", NULL);
                perror("Error to start multiplication");
            }
        }
        else{
            execl("./minus", "", NULL);
            perror("Error to start minus");
        }
    }
    else{
        execl("./plus", "", NULL);
        perror("Error to start plus");
    }
}


int main() {
    struct sigaction sa;
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sa.sa_mask = set;
    sigaction(SIGUSR1, &sa, NULL);
    creatProcess();
}
