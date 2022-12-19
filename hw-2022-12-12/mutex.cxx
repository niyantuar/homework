#ifndef TASK_QUEUE_HXX
#define TASK_QUEUE_HXX

#include <cstddef>
#include <queue>
#include <functional>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>


class TaskQueue {
  public:
    TaskQueue(std::size_t thread_count) 
      : _thread_count{thread_count}
      , _threads(thread_count)
      , _semaphore{new sem_t{}}
      , _mutex{new pthread_mutex_t{}} {
        sem_init(_semaphore, 0, thread_count);
        pthread_mutex_init(_mutex, nullptr);
        for (std::size_t i{}; i < thread_count; ++i) {
          _free_threads.push(i);
        }
    }

    using function_t = std::function<void*(void*)>;
    using arg_t = void*;
    using task_t = std::pair<function_t, arg_t>;

    void push_task(function_t function, arg_t arg, void** output_arg) {
        _tasks.emplace(function, arg);
        _output_arguments.push(output_arg);
    }

    void run() {
      while (_tasks.empty()) {
          thread_create(
              _get_a_free_thread(),
              nullptr,
              _do_a_task,
              nullptr
          );
      }
    }

    ~TaskQueue() {
      sem_destroy(_semaphore);
      pthread_mutex_destroy(_mutex);
      delete _semaphore;
    }
  private:
    std::pair<task_t, void**> _get_a_new_task() {
        pthread_mutex_lock(_mutex);
        auto task = _tasks.front();
        _tasks.pop();
        auto output_argument = _output_arguments.front();
        _output_arguments.pop();
        pthread_mutex_unlock(_mutex);
        return {task, output_argument};
    }

    pthread_t* _get_a_free_thread() {
        pthread_mutex_lock(_mutex);
        std::size_t free_thread_index = _free_thread_indexes.front();
        _free_thread_indexes.pop();
        auto ret = &_threads[free_thread_index];
        pthread_mutex_unlock(_mutex);
        return ret;
    }

    void* _do_a_task(void*) {
      sem_wait(_semaphore);
      auto [task, output_arg] = _get_a_new_task();
      thread_create(
          _get_a_free_thread();
          nullptr,
          task.first,
          task.second
      );
      sem_post(_semaphore);
      
      return ret;
    }
  private:
    std::queue<task_t> _tasks;
    std::queue<void**> _argument_arguments;
    std::vector<pthread_t> _threads;
    sem_t* _semaphore;
    pthread_mutex_t* _mutex;
    std::size_t _thread_count;
    std::queue<std::size_t> _free_thread_indexes;
};


#endif // TASK_QUEUE_HXX
