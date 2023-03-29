#include <iostream>
#include <cstring>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

pthread_mutex_t mutex_ostream;
void atomic_println(const std::string& str) {
    pthread_mutex_lock(&mutex_ostream);
    std::cout << str << std::endl;
    pthread_mutex_unlock(&mutex_ostream);
}

static constexpr std::size_t BUF_SIZE{4};
static constexpr const char* TOKEN_FILE{"token_file"};

struct Buffer {
    int write_index;
    int read_index;
    int data[BUF_SIZE];
};

static Buffer buffer{};

void put_item(int value) {
    buffer.data[buffer.write_index] = value;
    buffer.write_index = (buffer.write_index + 1) % BUF_SIZE;
}

int get_item() {
    int res = buffer.data[buffer.read_index];
    buffer.read_index = (buffer.read_index + 1) % BUF_SIZE;
    return res;
}

int main(int argc, char** argv) {
    int N = std::atoi(argv[1]);
    int M = std::atoi(argv[2]);

    if (creat(TOKEN_FILE, 0600) == -1) {
        throw std::logic_error{"creat"};
    }

    int token = ftok(TOKEN_FILE, 7);
    if (token == -1) {
        throw std::logic_error{"ftok"};
    }

    int shm_id = shmget(
                     token,
                     BUF_SIZE * sizeof(int),
                     IPC_CREAT | IPC_EXCL | 0600
                 );
    if (shm_id == -1) {
        throw std::logic_error{"shmget"};
    }

    int sem_id = semget(token, 3, IPC_CREAT | IPC_EXCL | 0600);
    // sem[0]: shows the count of the free slots in the buffer
    // sem[1]: shows the count of the unused products
    // sem[2]: mutex for the buffer

    if (sem_id == -1) {
        throw std::logic_error{"semget"};
    }

    struct sembuf init[] {
        {0, BUF_SIZE, 0},
        {2, 1, 0}
    };
    if (semop(sem_id, init, sizeof(init) / sizeof(*init)) == -1) {
        throw std::logic_error{"semaphore init failed"};
    }

    struct sembuf mutex_lock {
        2, -1, 0
    };
    struct sembuf mutex_unlock {
        2, 1, 0
    };

    for (int i{}; i < N; i++) {
        struct sembuf produce[] {
            {0, -1, 0},
            {1, 1, 0},
        };
        if (fork() == 0) {  // producer
            if (semop(sem_id, &mutex_lock, 1) == -1) {
                throw std::logic_error{"mutex lock"};
            }

            put_item(i);
            if (semop(sem_id, produce, sizeof(produce) / sizeof(*produce)) == -1) {
                throw std::logic_error{"couldn't produce"};
            }

            if (semop(sem_id, &mutex_unlock, 1) == -1) {
                throw std::logic_error{"mutex unlock"};
            }
            return 0;
        }
    }

    for (int i{}; i < M; i++) {
        struct sembuf consume[] {
            {0, 1, 0},
            {1, -1, 0}
        };
        if (fork() == 0) {  // consumer
            if (semop(sem_id, &mutex_lock, 1) == -1) {
                throw std::logic_error{"mutex lock"};
            }

            int item = get_item();
            if (semop(sem_id, consume, sizeof(consume) / sizeof(*consume)) == -1) {
                throw std::logic_error{"couldn't produce"};
            }
            atomic_println(std::to_string(item));

            if (semop(sem_id, &mutex_unlock, 1) == -1) {
                throw std::logic_error{"mutex unlock"};
            }
            return 0;
        }
    }

    for (int i{}; i < N + M; i++) {
        wait(nullptr);
    }

    shmctl(shm_id, IPC_RMID, nullptr);
    semctl(sem_id, 3, IPC_RMID, nullptr);
    if (unlink(TOKEN_FILE) == -1) {
        throw std::logic_error{"unlink"};
    }
}

