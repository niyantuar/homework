#include <iostream>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    pid_t pid = fork();
    if (pid == 0) {
        if (kill(getppid(), SIGKILL) != 0) {
            std::cout << "couldn't kill:(" << std::endl;
        } else {
            sleep(1);
            std::cout << "yay, done" << std::endl;
            std::cout << "new father is " << getppid() << std::endl;
        }
    } else {
        wait(nullptr);
    }
}
