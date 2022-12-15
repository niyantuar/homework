#include <iostream>
#include <cstddef>
#include <vector>
#include <fstream>
#include <chrono>
#include <pthread.h>


class Integer {
  private:
    int _value;
    pthread_mutex_t* _mutex;
  public:
    Integer(int value): _value{value}, _mutex(new pthread_mutex_t{}) {
        pthread_mutex_init(_mutex, nullptr);
    }   
    
    int get_value() const {
        return _value;
    }

    void set_value(int value) {
        _value = value;
    }

    void operator++(int) {
        pthread_mutex_lock(_mutex);
        _value++;
        pthread_mutex_unlock(_mutex);
    }

    ~Integer() {
        pthread_mutex_destroy(_mutex);
        delete _mutex;
    }
};


void* increment(void* arg) {
    Integer* integer = static_cast<Integer*>(arg);
    const std::size_t count = 100000;
    for (std::size_t i{}; i < count; i++){
       integer++;
    }
    return nullptr;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::size_t thread_count = 100;
    Integer* integer = new Integer{0};
    std::vector<pthread_t*> threads(thread_count);
    
    for (int i = 0; i < thread_count; i++){
        if (pthread_create(
                threads[i],
                nullptr,
                increment,
                static_cast<void*>(integer)) != 0
        ){
            return 1;
        }
    }
    
    for (std::size_t i = 0; i < thread_count; i++){
        if (pthread_join(*threads[i], nullptr) != 0){
            return 2;
        }
    }
    std::cout << integer->get_value() << std::endl;
    
    delete integer;
    
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
        stop - start
    );

    std::ofstream time_file{"time.txt", std::ios_base::app};
    time_file << "mutex: " << duration.count() << " seconds" << std::endl;
}
