#include <iostream>
#include <stdexcept>
#include <sys/shm.h>
#include <fcntl.h>
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

    std::size_t index{};
    char input;
    std::cin >> std::noskipws;
    while (std::cin >> shmat_array[index]) {
        index++;
        shmat_array[index] = 0;
    }
    return 0;
}
