#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sys/signal.h>


sockaddr_un socket_address;

int client_fd{};

std::string client_name{};

void sigint_handler(int signal_number) {
    close(client_fd);
    unlink(client_name.c_str());
    exit(signal_number);
}

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
    if (argc < 2) {
        throw std::logic_error{"client_name is not specified"};
    }
    client_name = argv[1];
    signal(SIGINT, sigint_handler);

    client_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("socket error");
        return 1;
    }

    socket_address.sun_family = AF_LOCAL;
    std::strcpy(socket_address.sun_path, client_name.c_str());
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
    unlink(client_name.c_str());
}
