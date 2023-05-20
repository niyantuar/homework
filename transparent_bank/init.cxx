#include "bank_storage.hxx"
#include "mutex.hxx"
#include <sstream>
#include <stdexcept>
#include <sys/shm.h>


static constexpr int FTOK_KEY{9};


int init_bank(char** argv) {
    std::size_t N;
    std::stringstream args{argv[2]};
    args >> N;
    if (not args) {
        throw std::invalid_argument{"argv[2] is not a number"};
    }

    auto token = ftok(argv[1], FTOK_KEY);
    Mutex mutex(argv[1], false);

    int shmid = shmget(
                    token,
                    sizeof(BankStorage) + sizeof(Customer) * N,
                    IPC_CREAT | IPC_EXCL | 0600
                );
    if (shmid == -1) {
        throw std::logic_error{"shmget"};
    }
    auto bank_storage = reinterpret_cast<BankStorage*>(
                            shmat(shmid, nullptr, 0)
                        );
    if (reinterpret_cast<void*>(bank_storage) == reinterpret_cast<void*>(-1)) {
        std::logic_error{"shmat error"};
    }
    bank_storage->user_count = N;
    for (std::size_t i{}; i < N; i++) {
        bank_storage->customers[i] = Customer{};
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        throw std::invalid_argument{"init must have 3 arguments"};
    }
    return init_bank(argv);
}
