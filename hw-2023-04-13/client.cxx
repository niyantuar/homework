#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>


struct SocketAddress {
    sa_family_t sa_family;
    char sa_data[1024];
} socket_address;

int client_fd{};

void* send(void*) {
    std::string receiver_name;
    std::string message;
    std::cin >> receiver_name >> message;
    sockaddr_un address{};
    address.sun_family = AF_LOCAL;
    std::strcpy(address.sun_path, receiver_name.c_str());
    sendto(
        client_fd,
        message.c_str(),
        message.size() + 1,
        0,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );
    return nullptr;
}

void* receive(void*) {
    while (true) {
        char buffer[1024] {};
        sockaddr_un address{};
        address.sun_family = AF_LOCAL;
        socklen_t address_length = sizeof(address);
        recvfrom(
            client_fd,
            buffer,
            sizeof(buffer),
            0,
            reinterpret_cast<sockaddr*>(&address),
            &address_length
        );
        std::cout << "received message " << buffer
                  << " from " << address.sun_path << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 1) {
        throw std::logic_error{"client_name is not specified"};
    }
    std::string client_name{argv[1]};

    client_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket error");
        return 1;
    }

    socket_address.sa_family = AF_LOCAL;
    std::strcpy(socket_address.sa_data, client_name.c_str());
    if (
        bind(
            client_fd,
            reinterpret_cast<sockaddr*>(&socket_address),
            sizeof(socket_address)
        ) == -1
    ) {
        perror("bind error");
        return 1;
    }

    pthread_t sender{};
    pthread_create(&sender, nullptr, send, nullptr);

    pthread_t receiver{};
    pthread_create(&receiver, nullptr, receive, nullptr);

    pthread_detach(receiver);
    pthread_join(sender, nullptr);

    close(client_fd);
}
