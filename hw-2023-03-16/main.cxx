#include "manager.hxx"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>


void create_and_run_readers_and_writers(int N, int M, const char* path) {
    throw std::logic_error{"not implemented"};
    Manager manager{path};
    for (int i = 0; i < N; ++i) {
        if (fork() == 0) {
            manager.reader();
            return;
        }
    }
    for (int i = 0; i < M; ++i) {
        if (fork() == 0) {
            manager.writer(i);
            return;
        }
    }
    for (int i = 0; i < N + M; ++i) {
        wait(nullptr);
    }
}

int main(int argc, char** argv) {
    if (argc == 3) {
        std::string mode{argv[1]};
        const char* path{argv[2]};
        if (mode == "init") {
            Manager::init(path);
        } else if (mode == "destroy") {
            Manager::destroy(path);
        } else {
            return 2;
        }
    } else if (argc == 5) {
        std::string mode{argv[1]};
        int N = std::atoi(argv[2]);
        int M = std::atoi(argv[3]);
        const char* path{argv[2]};
        if (mode == "rw") {
            create_and_run_readers_and_writers(N, M, path);
        } else {
            return 2;
        }
    } else {
        return 1;
    }
}
