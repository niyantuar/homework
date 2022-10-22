#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char** argv) {
    if (argc != 3) {
        throw std::logic_error{"Arguments count must be 3"};
    }
    const std::string number_string = argv[1];
    const unsigned long long n = stoull(number_string);
    const std::string text = argv[2];
    for (unsigned long long i = 1; i <= n; ++i) {
        const std::string file_name = "file_" + std::to_string(i) + ".txt";
        const int file_desc = open(file_name.c_str(), O_TRUNC | O_CREAT | O_WRONLY,
                                   0644);
        const std::string content = text + " " + std::to_string(i) + "\n";
        write(file_desc, content.c_str(), content.size());
        close(file_desc);
    }
}
