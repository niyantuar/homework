#include <iostream>
#include <cstring>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>


static constexpr std::size_t BUF_SIZE{4};
static constexpr const char* TOKEN_FILE{"token_file"};

struct Buffer {
    int write_index;
    int read_index;
    int data[BUF_SIZE];
};


int main(int argc, char** argv) {
    int N = std::atoi(argv[1]);
    int M = std::atoi(argv[2]);

    if (creat(TOKEN_FILE, 0600) == -1) {
        throw std::logic_error{"creat"};
    }

    int token = ftok(TOKEN_FILE, 7);
    if (token == -1) {
        throw std::logic_error{"ftok"};
    }

    int shm_id = shmget(
                     token,
                     BUF_SIZE * sizeof(int),
                     IPC_CREAT | IPC_EXCL | 0600
                 );
    if (shm_id == -1) {
        throw std::logic_error{"shmget"};
    }

    for (int i{}; i < N; i++) {
        if (fork() == 0) {  // producer

            break;
        }
    }

    for (int i{}; i < M; i++) {
        if (fork() == 0) {  // consumer

            break;
        }
    }

    for (int i{}; i < N + M; i++) {
        wait(nullptr);
    }

    shmctl(shm_id, IPC_RMID, nullptr);
    if (unlink(TOKEN_FILE) == -1) {
        throw std::logic_error{"unlink"};
    }
}
