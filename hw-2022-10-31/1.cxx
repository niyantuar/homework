#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];

        pid_t pid = fork();
        if (pid == -1) {
            return 1;
        }

        if (pid == 0) {  // child
            char* passed_arguments[] {arg, nullptr};
            execvp(arg, passed_arguments);
            return 127;
        } else {  // parent
            int status = 0;
            waitpid(pid, &status, 0);
            if (status == 0) {
                return 0;
            }
        }
    }
    return 0;
}
