#include <iostream>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    pid_t pid = fork();
    if (pid == 0) {
        kill(getppid(), SIGKILL);
    } else {
        wait(nullptr);
    }
}
