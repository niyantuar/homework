#include "mutex.hxx"

struct sembuf Mutex::_up {
    0, 1, 0
};
struct sembuf Mutex::_down {
    0, -1, 0
};
void Mutex::_connect_mutex() {
    int token = ftok(_file_path, 7);
    if (token == -1) {
        throw std::logic_error{"ftok"};
    }
    _sem_id = semget(token, 1, 0600);
    if (_sem_id == -1) {
        throw std::logic_error{"semget"};
    }
}
void Mutex::_create_mutex() {
    creat(_file_path, 0600);
    int token = ftok(_file_path, 7);
    if (token == -1) {
        throw std::logic_error{"ftok"};
    }
    _sem_id = semget(token, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (_sem_id == -1) {
        throw std::logic_error{"semget"};
    }
    unlock();
}
void Mutex::unlock() {
    semop(_sem_id, &_up, 1);
}
Mutex::Mutex(const char* file_path, bool is_created) : _file_path{file_path} {
    if (not is_created) {
        _create_mutex();
    } else {
        _connect_mutex();
    }
}
void Mutex::lock() {
    semop(_sem_id, &_down, 1);
}
void Mutex::destroy_mutex() {
    semctl(_sem_id, 0, IPC_RMID, nullptr);
}
