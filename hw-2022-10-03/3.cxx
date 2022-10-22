#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>


static const std::size_t FILE_MAX_SIZE = 1000000;

bool is_directory(DIR* dir) {
    return not(dir == NULL);
}

void print_file_content(const std::string& file_name,
                        const std::string& pattern) {
    const int file_descriptor = open(file_name.c_str(), O_RDONLY);
    char file_content_buffer[FILE_MAX_SIZE + 1] {};
    read(file_descriptor, file_content_buffer, FILE_MAX_SIZE);
    std::string file_content{file_content_buffer};

    std::size_t pattern_size = pattern.size();
    std::size_t line_number = 1;
    std::string line{};
    for (std::size_t i{}; i <= file_content.size() - pattern_size; ++i) {
        if (file_content[i] == '\n') {
            line_number++;
            line = "";
            continue;
        }
        line += file_content[i];
        if (file_content.substr(i, pattern_size) == pattern) {
            std::cout << file_name << ":";
            std::cout << line_number << ": " << line;
            std::size_t j = i + 1;
            while (true) {
                if (j == file_content.size()) {
                    break;
                }
                char current_character = file_content[j];
                if (current_character == '\n') {
                    break;
                }
                std::cout << current_character;
                j++;
            }
            std::cout << std::endl;
        }
    }
    close(file_descriptor);
}

bool is_trivial_directory(const std::string& director_name) {
    return ((director_name == ".") or (director_name == ".."));
}

void parse_file(const std::string& file_name, const std::string& pattern);

void print_directory(const std::string& directory_name, DIR* dir,
                     const std::string& pattern) {
    while (true) {
        dirent* directory_entry = readdir(dir);
        if (directory_entry == NULL) {
            break;
        }
        std::string child_name = directory_entry->d_name;
        if (is_trivial_directory(child_name)) {
            continue;
        }
        std::string child_full_name = directory_name + "/" + child_name;
        parse_file(child_full_name, pattern);
    }
}

void parse_file(const std::string& file_name, const std::string& pattern) {
    DIR* dir = opendir(file_name.c_str());
    if (is_directory(dir)) {
        print_directory(file_name, dir, pattern);
    } else {
        print_file_content(file_name, pattern);
    }
    closedir(dir);
}

int main(int argc, char** argv) {
    if (argc == 1) {
        throw std::logic_error{"missing positional argument"};
    }
    std::string pattern = argv[1];
    std::string directory_name = ".";
    if (argc == 3) {
        directory_name = argv[2];
    }
    parse_file(directory_name, pattern);
}
