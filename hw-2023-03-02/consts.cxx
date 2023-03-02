#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "consts.hxx"


int get_shm_id() {
    key_t shm_key = ftok(TOKEN_FILE_NAME, PROJ_ID);
    if (shm_key == -1) {
        perror("ftok");
        throw std::runtime_error{"ftok"};
    }
    int shm_id = shmget(shm_key, MEMORY_SIZE, 0600);
    if (shm_id == -1) {
        perror("shmget");
        throw std::runtime_error{"shmget"};
    }
    return shm_id;
}
