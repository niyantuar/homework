#include <cstddef>
#include <iostream>
#include <pthread.h>
#include <stdexcept>
#include <string>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>


pthread_mutex_t mutex_ostream;
void atomic_println(const std::string& str) {
    pthread_mutex_lock(&mutex_ostream);
    std::cout << str << std::endl;
    pthread_mutex_unlock(&mutex_ostream);
}


int main() {
    pthread_mutex_init(&mutex_ostream, nullptr);
    const int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem_id == -1) {
        throw std::logic_error{"semget"};
    }
    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        throw std::logic_error{"semctl"};
    }
    int n;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        struct sembuf minus = {0, static_cast<short>(-i), 0};
        struct sembuf plus = {0, static_cast<short>(i + 1), 0};
        int pid = fork();
        if (pid == -1) {
            throw std::logic_error{"pid"};
        }
        if (pid == 0) {  // child process
            if (semop(sem_id, &minus, 1) == -1) {
                throw std::logic_error{"semop"};
            }
            atomic_println(std::to_string(i));
            if (semop(sem_id, &plus, 1) == -1) {
                throw std::logic_error{"semop"};
            }
            return 0;
        }
    }
    for (int i = 1; i <= n; i++) {
        wait(nullptr);
    }
    pthread_mutex_destroy(&mutex);
    semctl(sem_id, 1, IPC_RMID);
}
