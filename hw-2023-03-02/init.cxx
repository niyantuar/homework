#include <iostream>
#include <sys/shm.h>
#include <fcntl.h>
#include "consts.hxx"


int main() {
    creat(TOKEN_FILE_NAME, 0600);
    key_t shm_key = ftok(TOKEN_FILE_NAME, PROJ_ID);
    if (shm_key == -1) {
        perror("ftok");
        return 1;
    }

    int shm_id;
    shm_id = shmget(shm_key, MEMORY_SIZE, IPC_CREAT | IPC_EXCL | 0600);
    if (shm_id  == -1) {
        perror("shmget");
        return 1;
    }
}
