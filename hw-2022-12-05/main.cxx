#include <iostream>
#include <cstring>
#include <cstddef>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "shat vata" << std::endl;
        return 1;
    }
    std::size_t N = std::atoi(argv[1]);
    std::size_t M = std::atoi(argv[2]);

    std::vector<int> random_numbers(N);
    std::srand(std::time(nullptr));
    for (auto& el : random_numbers) {
        el = std::rand() % 10;  // vor shat gesh tver chlinen
        std::cout << el << ' ';
    }
    std::cout << std::endl;


    pid_t child_pid = -1;
    std::vector<std::array<int, 2>> pipes(M, std::array<int, 2>{-1, -1});

    const std::size_t step = std::ceil(static_cast<double>(N) / M);
    
    for (std::size_t i{}; i < M; ++i) {
        if (child_pid != 0) {
            int current_pipe[2];
            if (pipe(current_pipe) == -1) {
                std::cout << "shat vata pipe" << std::endl;
                return -1;  // shat vata
            }
            pipes[i] = {current_pipe[0], current_pipe[1]};

            child_pid = fork();
            if (child_pid == -1) {
                std::cout << "shat vata fork" << std::endl;
                return -1;  // doesn't kill children
            }

            close(pipes[i][1]);
        }
        if (child_pid == 0) {
            close(pipes[i][0]);
            int restricted_sum = std::accumulate(
                random_numbers.begin() + step * i,
                std::min(random_numbers.begin() + step * (i + 1), random_numbers.end()),
                0
            );
            std::cout << restricted_sum << std::endl;
            write(pipes[i][1], &restricted_sum, sizeof(restricted_sum));
            close(pipes[i][1]);
            return 0;
        }
    }

    int res{};
    for (auto [in, out] : pipes) {  // out is closed
        wait(nullptr);
        int restricted_sum;
        read(in, &restricted_sum, sizeof(restricted_sum));
        res += restricted_sum;
        close(in);
    }
    std::cout << res << std::endl;
}
