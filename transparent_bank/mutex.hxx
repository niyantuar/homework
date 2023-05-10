#ifndef MUTEX_HXX
#define MUTEX_HXX
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

class Mutex {
  public:
    explicit Mutex(const char* file_path, bool is_created = true);
    void lock();
    void unlock();
    void destroy_mutex();

  private:
    void _create_mutex();
    void _connect_mutex();
    int _sem_id;
    static struct sembuf _up;
    static struct sembuf _down;
    const char* _file_path;
};

#endif // MUTEX_HXX
