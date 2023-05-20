#include "mutex.hxx"
#include <stdexcept>
#include <sys/shm.h>

static constexpr int FTOK_KEY{9};

int destroy_bank(char** argv) {
    auto token = ftok(argv[1], FTOK_KEY);
    Mutex mutex(argv[1]);
    mutex.destroy_mutex();

    int shmid = shmget(token, 0, 0600);
    if (shmid == -1) {
        throw std::logic_error{"shmget"};
    }
    shmctl(shmid, IPC_RMID, nullptr);
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::invalid_argument{"destroy must have 2 arguments"};
    }
    return destroy_bank(argv);
}
