#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>


static const std::size_t FILE_MAX_SIZE = 1000000;

bool is_directory(DIR* dir) {
    return not(dir == NULL);
}

void print_file_content(const std::string& file_name) {
    const int file_descriptor = open(file_name.c_str(), O_RDONLY);
    char file_content[FILE_MAX_SIZE] {};
    read(file_descriptor, file_content, FILE_MAX_SIZE);
    std::cout << file_content << std::endl;
    close(file_descriptor);
}

bool is_trivial_directory(const std::string& director_name) {
    return ((director_name == ".") or (director_name == ".."));
}

void parse_file(const std::string& file_name);

void print_directory(const std::string& directory_name, DIR* dir) {
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
        std::cout << "|> " << child_full_name << std::endl;
        parse_file(child_full_name);
        std::cout << std::endl;
    }
}

void parse_file(const std::string& file_name) {
    DIR* dir = opendir(file_name.c_str());
    if (is_directory(dir)) {
        print_directory(file_name, dir);
    } else {
        print_file_content(file_name);
    }
    closedir(dir);
}

int main(int argc, char** argv) {
    std::string directory_name = ".";
    if (argc == 2) {
        directory_name = argv[1];
    }
    parse_file(directory_name);
}
