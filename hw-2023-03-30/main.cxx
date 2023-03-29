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

static constexpr std::size_t BUF_SIZE{4};
static constexpr const char* TOKEN_FILE{"token_file"};

struct Buffer {
    int write_index;
    int read_index;
    int data[BUF_SIZE];
};

void put_item(
    int value,
    int* write_index_address,
    int* read_index_address,
    int* data
) {
    data[*write_index_address] = value;
    *write_index_address = (*write_index_address + 1) % BUF_SIZE;
}

int get_item(
    int* write_index_address,
    int* read_index_address,
    int* data
) {
    int res = data[*read_index_address];
    *read_index_address = (*read_index_address + 1) % BUF_SIZE;
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
                     sizeof(Buffer),
                     IPC_CREAT | IPC_EXCL | 0600
                 );
    if (shm_id == -1) {
        throw std::logic_error{"shmget"};
    }

    int* write_index_address = reinterpret_cast<int*>(shmat(shm_id, nullptr, 0));
    int* read_index_address = write_index_address + 1;
    int* data = read_index_address + 1;
    *write_index_address = 0;
    *read_index_address = 0;
    for (std::size_t i = 0; i < BUF_SIZE; ++i) {
        data[i] = 0;
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
        struct sembuf reserve {
            0, -1, 0
        };
        struct sembuf add_item {
            1, 1, 0
        };
        if (fork() == 0) {  // producer
            int* write_index_address = reinterpret_cast<int*>(shmat(shm_id, nullptr, 0));
            int* read_index_address = write_index_address + 1;
            int* data = read_index_address + 1;

            if (semop(sem_id, &reserve, 1) == -1) {
                throw std::logic_error{"couldn't reserve"};
            }

            if (semop(sem_id, &mutex_lock, 1) == -1) {
                throw std::logic_error{"mutex lock"};
            }
            put_item(i, write_index_address, read_index_address, data);
            if (semop(sem_id, &mutex_unlock, 1) == -1) {
                throw std::logic_error{"mutex unlock"};
            }
            if (semop(sem_id, &add_item, 1) == -1) {
                throw std::logic_error{"couldn't add item"};
            }
            return 0;
        }
    }

    for (int i{}; i < M; i++) {
        struct sembuf reliese {
            0, 1, 0
        };
        struct sembuf remove_item {
            1, -1, 0
        };
        if (fork() == 0) {  // consumer
            int* write_index_address = reinterpret_cast<int*>(shmat(shm_id, nullptr, 0));
            int* read_index_address = write_index_address + 1;
            int* data = read_index_address + 1;

            if (semop(sem_id, &remove_item, 1) == -1) {
                throw std::logic_error{"couldn't remove item"};
            }

            if (semop(sem_id, &mutex_lock, 1) == -1) {
                throw std::logic_error{"mutex lock"};
            }
            int item = get_item(write_index_address, read_index_address, data);
            std::cout << item << std::endl;
            if (semop(sem_id, &mutex_unlock, 1) == -1) {
                throw std::logic_error{"mutex unlock"};
            }
            if (semop(sem_id, &reliese, 1) == -1) {
                throw std::logic_error{"couldn't reliese"};
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

