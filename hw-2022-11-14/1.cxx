#include <iostream>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int first_to_second[2];
    int second_to_first[2];
    pipe(first_to_second);
    pipe(second_to_first);
    pid_t pid_1 = fork();
    if (pid_1 == 0) {  // child 1
        // writes into first_to_second, reads from second_to_first
        close(first_to_second[0]);
        close(second_to_first[1]);

        pid_t self_pid = getpid();
        write(first_to_second[1], &self_pid, sizeof(pid_t));
        close(first_to_second[1]);

        pid_t sibling_pid;
        read(second_to_first[0], &sibling_pid, sizeof(pid_t));
        close(second_to_first[0]);

        std::cout << "1: " << sibling_pid << std::endl;
        return 0;
    }

    pid_t pid_2 = fork();
    if (pid_2 == 0) {  // child 2
        // writes into second_to_first, reads from first_to_second
        close(second_to_first[0]);
        close(first_to_second[1]);

        pid_t sibling_pid;
        read(first_to_second[0], &sibling_pid, sizeof(pid_t));
        close(first_to_second[0]);

        pid_t self_pid = getpid();
        write(second_to_first[1], &self_pid, sizeof(pid_t));
        close(second_to_first[1]);

        std::cout << "2: " << sibling_pid << std::endl;
        return 0;
    }
    waitpid(pid_2, nullptr, 0);
    waitpid(pid_1, nullptr, 0);
    std::cout << "first process with pid " << pid_1 << " ended" << std::endl;
    std::cout << "second process with pid " << pid_2 << " ended" << std::endl;
    close(second_to_first[0]);
    close(second_to_first[1]);
    close(first_to_second[0]);
    close(first_to_second[1]);
}
