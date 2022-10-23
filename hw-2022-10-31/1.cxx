#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    int exit_status = 0;
    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];

        pid_t pid = fork();
        if (pid == -1) {
            return 1;
        }

        if (pid == 0) {  // child
            char* passed_arguments[] {arg, nullptr};
            execvp(arg, passed_arguments);
            return 127;  // not found
        } else {  // parent
            int status;
            waitpid(pid, &status, 0);
            exit_status = WEXITSTATUS(status);
            if (exit_status == 0) {
                return 0;
            }
        }
    }
    return exit_status;
}
