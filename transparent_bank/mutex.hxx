#ifndef MUTEX_HXX
#define MUTEX_HXX
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

class Mutex {
public:
  explicit Mutex(const char *file_path, bool is_created = true);
  void lock();
  void unlock();

private:
  void _create_mutex(const char *file_path);
  void _connect_mutex(const char *file_path);
  void _destroy_mutex(const char *file_path);
  int _sem_id;
  static struct sembuf _up;
  static struct sembuf _down;
};

#endif // MUTEX_HXX
