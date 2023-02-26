#include "consts.hxx"
#include <cstdio>
#include <pthread.h>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    try {
        int shm_id = get_shm_id();
        shmctl(shm_id, IPC_RMID, nullptr);
        if (unlink(TOKEN_FILE_NAME) == -1) {
            perror("unlink");
            throw std::runtime_error{"unlink"};
        }
    } catch (const std::runtime_error& e) {
        return 1;
    }
}
