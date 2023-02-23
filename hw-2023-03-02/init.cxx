#include <iostream>
#include <sys/shm.h>
#include "consts.hxx"


int main() {
        key_t key = ftok(TOKEN_FILE_NAME.c_str(), 2);
        int* shmat_array;
        int index;
        int value;

        if (key == -1) {
                perror("ftok");
                return 1;
        }

        int shm_id;
        shm_id = shmget(key, MEMORY_SIZE, IPC_CREAT | 0600);
        if (shm_id  == -1) {
                perror("shmget");
                return 1;
        }

        shmat_array = (int*) shmat(shm_id, 0, 0);

        if (shmat_array == (void*)-1) {
                perror("shmat");
                return 1;
        }
        std::cout << shmat_array << std::endl;

        for (int i = 0; i < 4; i++) {
                std::cout << shmat_array[i] << " " << std::endl;
        }
        std::cin >> index >> value;

        shmat_array[index] = value;

        return 0;
}
