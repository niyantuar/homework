#ifndef TASK_QUEUE_HXX
#define TASK_QUEUE_HXX

#include <cstddef>
#include <queue>
#include <functional>
#include <signal.h>


class TaskQueue {
  public:
    TaskQueue(std::size_t thread_count) : _thread_count{thread_count} {
        // struct sigaction action{};
        // action.sa_handler = [](int signum) {
        //     if (signum == SIGUSR1) {
        //         this->_take_a_new_task();
        //     }
        // };
        // sigaction(SIGUSR1, &action, nullptr);
    }

    using function_t = std::function<void*(void*)>;
    using arg_t = void*;
    using task_t = std::pair<function_t, arg_t>;

    void run(function_t function, arg_t arg) {
        _tasks.emplace(function, arg);
        // loop
    }
  private:
    void _take_a_new_task() {
        auto task = _tasks.front();
        _tasks.pop();
    }
  private:
    std::queue<task_t> _tasks;
    std::size_t _thread_count;
};


#endif // TASK_QUEUE_HXX
