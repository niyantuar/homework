#ifndef CONSTS_HXX
#define CONSTS_HXX
#include <string>
#include <cstddef>

constexpr const char* TOKEN_FILE_NAME{"token_file"};
constexpr int PROJ_ID{7};
constexpr std::size_t MEMORY_SIZE{1024};

int get_shm_id();

#endif // CONSTS_HXX
