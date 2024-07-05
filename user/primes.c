#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

const int LEN = sizeof(int);


void right_transmit(int rpipe_wr, int number, int first){
    
    if(number % first){
        write(rpipe_wr, &number, LEN);
    }
}

void primes(int lpipe[2]){

    int p[2];
    int number;
    int first;

    close(lpipe[WR]);//用不到

    if(read(lpipe[RD], &first, LEN)){
        fprintf(1,"prime %d\n", first);
        pipe(p);

        if(fork() == 0){
            primes(p);
        }
        else{
            while(read(lpipe[RD], &number, LEN)){
                right_transmit(p[WR], number, first);
            }
            close(lpipe[RD]);
            close(p[WR]);
            close(p[RD]);
            wait(0);
        }
    }
    exit(0);
}

int
main(int argc, char* argv[]){
    int i;
    int p[2];
    pipe(p);

    if(argc != 1){
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    for(i = 2;i <= 35;i++){
        write(p[WR],&i,LEN);
    }

    if(fork() == 0){
        primes(p);
    }
    else{
        close(p[RD]);
        close(p[WR]);
        wait(0);
    }
    exit(0);
}