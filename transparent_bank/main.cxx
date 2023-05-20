#include "bank.hxx"
#include "bank_storage.hxx"
#include "customer.hxx"
#include "mutex.hxx"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>


constexpr int FTOK_KEY{9};
const std::string INIT{"init"};
const std::string DESTROY{"destroy"};
const std::string CLIENT{"client"};


int client(char** argv) {
    auto token = ftok(argv[0], FTOK_KEY);
    Mutex mutex(argv[0]);
    int shmid = shmget(token, 0, 0600);
    if (shmid == -1) {
        throw std::logic_error{"shmget"};
    }
    auto bank_storage = reinterpret_cast<BankStorage*>(
                            shmat(shmid, nullptr, 0)
                        );
    if (reinterpret_cast<void*>(bank_storage) == reinterpret_cast<void*>(-1)) {
        std::logic_error{"shmat error"};
    }

    Bank bank{bank_storage, mutex};

    const std::string FREEZE{"freeze"};
    const std::string UNFREEZE{"unfreeze"};
    const std::string SEND_MONEY{"send_money"};
    const std::string TRANSACTION_TO_EVERY_VALID_CUSTOMER{
        "transaction_to_every_valid_customer"
    };
    const std::string SET_MINIMUM_ALLOWED{"set_minimum_allowed"};
    const std::string SET_MAXIMUM_ALLOWED{"set_maximum_allowed"};

    while (true) {
        bank.print();
        std::string command;
        std::cin >> command;
        if (command == FREEZE) {
            std::size_t index;
            std::cin >> index;
            bank.freeze(index);
        } else if (command == UNFREEZE) {
            std::size_t index;
            std::cin >> index;
            bank.freeze(index);
        } else if (command == SEND_MONEY) {
            std::size_t from, to;
            Customer::balance_t value;
            std::cin >> from >> to >> value;
            if (value < 0) {
                std::cerr << "no negative money\n";
                continue;
            }
            bank.send_money(from, to, value);
        } else if (command == TRANSACTION_TO_EVERY_VALID_CUSTOMER) {
            Customer::balance_t value;
            std::cin >> value;
            if (value < 0) {
                std::cerr << "no negative money\n";
                continue;
            }
            bank.transaction_to_every_valid_customer(value);
        } else if (command == SET_MINIMUM_ALLOWED) {
            Customer::balance_t minimum_allowed;
            std::size_t index;
            std::cin >> minimum_allowed >> index;
            bank.set_minimum_allowed(minimum_allowed, index);
        } else if (command == SET_MAXIMUM_ALLOWED) {
            Customer::balance_t maximum_allowed;
            std::size_t index;
            std::cin >> maximum_allowed >> index;
            bank.set_maximum_allowed(maximum_allowed, index);
        } else {
            std::logic_error{"not command `" + command + "` was found"};
        }
    }

    return 0;
}


int init_bank(char** argv) {
    std::size_t N;
    std::stringstream args{argv[2]};
    args >> N;
    if (not args) {
        throw std::invalid_argument{"argv[2] is not a number"};
    }

    auto token = ftok(argv[0], FTOK_KEY);
    Mutex mutex(argv[0], false);

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

int destroy_bank(char** argv) {
    auto token = ftok(argv[0], FTOK_KEY);
    Mutex mutex(argv[0]);
    mutex.destroy_mutex();

    int shmid = shmget(token, 0, 0600);
    if (shmid == -1) {
        throw std::logic_error{"shmget"};
    }
    shmctl(shmid, IPC_RMID, nullptr);
    return 0;
}

int main(int argc, char** argv) {
    if (argv[1] == INIT) {
        if (argc != 3) {
            throw std::invalid_argument{"init must have 3 arguments"};
        }
        return init_bank(argv);
    }
    if (argv[1] == DESTROY) {
        if (argc != 2) {
            throw std::invalid_argument{"destroy must have 2 arguments"};
        }
        return destroy_bank(argv);
    }
    if (argv[1] == CLIENT) {
        if (argc != 2) {
            throw std::invalid_argument{"client must have 2 arguments"};
        }
        return client(argv);
    }
}
