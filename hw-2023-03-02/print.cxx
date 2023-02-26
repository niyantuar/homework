#include <iostream>
#include <stdexcept>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include "consts.hxx"

int main() {
    int shm_id;
    try {
        shm_id = get_shm_id();
    } catch (const std::runtime_error& e) {
        return 1;
    }
    char* shmat_array;
    shmat_array = static_cast<char*>(shmat(shm_id, 0, 0));

    if (shmat_array == reinterpret_cast<void*>(-1)) {
        perror("shmat");
        return 1;
    }

    while (true) {
        std::size_t index{};
        while (shmat_array[index] != 0) {
            std::cout << shmat_array[index];
            index++;
        }
        std::cout << std::endl;
        sleep(5);
    }

    return 0;
}
