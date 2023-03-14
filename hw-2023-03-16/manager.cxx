#include "manager.hxx"

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/shm.h>
#include <sys/sem.h>


#define MUTEX 0
#define DB 1

void Manager::init(const char* file_path) {
    int token = ftok(file_path, 7);
    if (token == -1) {
        throw std::logic_error{"ftok"};
    }
    int shm_id = shmget(token, 33 * sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
    if (shm_id == -1) {
        throw std::logic_error{"shmget"};
    }
    // need to init the shared memory

    int sem_id = semget(token, 2, IPC_CREAT | IPC_EXCL | 0600);
    if (sem_id == -1) {
        throw std::logic_error{"semget"};
    }
    struct sembuf sops[] {
        {0, 1, 0}, { 1, 1, 0 }
    };
    if (semop(sem_id, sops, sizeof(sops) / sizeof(struct sembuf)) == -1) {
        throw std::logic_error{"semget"};
    }
}
void Manager::destroy(const char* file_path) {
    throw std::logic_error{"not implemented"};
}

Manager::Manager(const char* file_path) {
    int token = ftok(file_path, 7);
    if (token == -1) {
        throw std::logic_error{"ftok"};
    }
    _shm_id = shmget(token, 33 * sizeof(int), 0600);
    if (_shm_id == -1) {
        throw std::logic_error{"shmget"};
    }
    _rc_address = reinterpret_cast<int*>(shmat(_shm_id, nullptr, 0));
    _a_address = _rc_address + 1;
    _sem_id = semget(token, 2, 0600);

    if (_sem_id == -1) {
        throw std::logic_error{"semget"};
    }
}

Manager::~Manager() {
    shmdt(_rc_address);
}

void Manager::reader() {
    int b[32] = {};
    struct sembuf mutex_down = {MUTEX, -1, 0};
    struct sembuf mutex_up = {MUTEX, 1, 0};
    struct sembuf db_down = {DB, -1, 0};
    struct sembuf db_up = {DB, 1, 0};

    semop(_sem_id, &mutex_down, 1);

    *_rc_address = *_rc_address + 1;

    if (*_rc_address == 1) {
        semop(_sem_id, &db_down, 1);
    }

    semop(_sem_id, &mutex_up, 1);

    for (int i = 0; i < 32; i++) {
        b[i] = _a_address[i];
    }

    semop(_sem_id, &mutex_down, 1);
    *_rc_address = *_rc_address - 1;
    if (*_rc_address == 0) {
        semop(_sem_id, &db_up, 1);
    }
    semop(_sem_id, &mutex_up, 1);

    for (int i = 0; i < 32; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
}
void Manager::writer(int value) {
    struct sembuf db_down = {DB, -1, 0};
    struct sembuf db_up = {DB, 1, 0};
    semop(_sem_id, &db_down, 1);
    for (int i = 0; i < 32; ++i) {
        _a_address[i] = value;
    }

    semop(_sem_id, &db_up, 1);
}
