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
        } else {  // parent
            int status;
            waitpid(pid, &status, 0);
            std::cout << "Process " << arg
                      << " with pid " << pid
                      << " finished with status " << status << std::endl;
        }
    }
    return 0;
}
