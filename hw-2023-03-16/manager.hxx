#ifndef MANAGER_H
#define MANAGER_H


class Manager {
  private:
    int _shm_id;
    int* _rc_address;
    int* _a_address;
    int _sem_id;

  public:
    static void init(const char* file_path);

    static void destroy(const char* file_path);

    Manager(const char* file_path);

    ~Manager();

    void reader();

    void writer(int value);
};

#endif // MANAGER_H
