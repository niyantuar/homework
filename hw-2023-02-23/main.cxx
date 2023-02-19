#include <iostream>
#include <pthread.h>


pthread_mutex_t mutex;
pthread_mutex_t mutex_ostream;
pthread_cond_t cond;
std::size_t count;

void atomic_println(const std::string& str) {
    pthread_mutex_lock(&mutex_ostream);
    std::cout << str << std::endl;
    pthread_mutex_unlock(&mutex_ostream);
}

void* f(void*) {
    pthread_mutex_lock(&mutex);
    while (count <= 10) {
        pthread_cond_wait(&cond, &mutex);
    }
    atomic_println(">10");
    while (count <= 50) {
        pthread_cond_wait(&cond, &mutex);
    }
    atomic_println(">50");
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
    int n;
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

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}
