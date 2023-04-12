#include <iostream>
#include <unistd.h>
#include <pthread.h>

void* send(void*) {
    std::string receiver_name;
    std::string message;
    std::cin >> receiver_name >> message;

}

void* receive(void*) {
    while (true) {

    }
}

int main(int argc, char** argv) {
    if (argc < 1) {
        throw std::logic_error{"client_name is not specified"};
    }
    std::string client_name{argv[1]};

    // create socket

    pthread_t sender{};
    pthread_create(&sender, nullptr, send, nullptr);

    pthread_t receiver{};
    pthread_create(&receiver, nullptr, receive, nullptr);

    pthread_detach(receiver);
    pthread_join(sender, nullptr);

    // release socket
}
