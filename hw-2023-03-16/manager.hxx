#ifndef MANAGER_H
#define MANAGER_H

#include <pthread.h>

class Manager {
  private:
    int _shm_id;
    int* _rc_address;
    int* _a_address;
    int _sem_id;
    static pthread_mutex_t _ostream_mutex;

  public:
    static void init(const char* file_path);

    static void destroy(const char* file_path);

    Manager(const char* file_path);

    ~Manager();

    void reader();

    void writer(int value);
};

#endif // MANAGER_H
