#include <iostream>
#include <pthread.h>


pthread_mutex_t mutex;
pthread_mutex_t mutex_exit;
pthread_mutex_t mutex_ostream;
pthread_cond_t cond;
std::size_t count;
bool is_time_to_exit = false;

void atomic_println(const std::string& str) {
    pthread_mutex_lock(&mutex_ostream);
    std::cout << str << std::endl;
    pthread_mutex_unlock(&mutex_ostream);
}

void* f(void*) {
    pthread_mutex_lock(&mutex);
    while (count <= 10) {
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_lock(&mutex_exit);
        if (is_time_to_exit) {
            return nullptr;
        }
        pthread_mutex_unlock(&mutex_exit);
    }
    atomic_println(">10");
    while (count <= 50) {
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_lock(&mutex_exit);
        if (is_time_to_exit) {
            return nullptr;
        }
        pthread_mutex_unlock(&mutex_exit);
    }
    atomic_println(">50");
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
    char n;
    pthread_t thread;
    pthread_create(&thread, nullptr, f, nullptr);
    pthread_detach(thread);
    while (std::cin >> n) {
        pthread_mutex_lock(&mutex);
        count++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    atomic_println(std::to_string(count));
    pthread_mutex_lock(&mutex_exit);
    is_time_to_exit = true;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex_exit);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}
