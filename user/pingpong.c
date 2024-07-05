#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    int pipe_ftos[2];//father->son
    int pipe_stof[2];//son->father
    pipe(pipe_ftos);
    pipe(pipe_stof);
    int pid = fork();

    if(pid < 0){
        fprintf(2,"fork() error\n");
        close(pipe_ftos[0]);
        close(pipe_ftos[1]);
        close(pipe_stof[0]);
        close(pipe_stof[1]);
        exit(1);
    }
    else if (pid == 0) {
        // Child process
        char buf[1];
        close(pipe_ftos[1]);
        close(pipe_stof[0]);
        read(pipe_ftos[0], buf, 1); // Read from pipe
        fprintf(1, "%d: received ping\n", getpid()); // Print message
        close(pipe_ftos[0]);
        write(pipe_stof[1], buf, 1); // Write back to pipe
        close(pipe_stof[1]);
        exit(0);
    } else {
        // Parent process
        char buf[1] = {'1'};
        close(pipe_ftos[0]);
        close(pipe_stof[1]);
        write(pipe_ftos[1], buf, 1); // Write to pipe
        close(pipe_ftos[1]);
        read(pipe_stof[0], buf, 1); // Read from pipe
        close(pipe_stof[0]);
        fprintf(1, "%d: received pong\n", getpid()); // Print message
    }

    exit(0);
}
