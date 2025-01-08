#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <charconv>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <poll.h>

#include <chrono>
#include <thread>


#include "conn.hpp"

int clientFD;

void connectToServer(std::string const& address, int port)
{
    struct addrinfo hints{};
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG;

    char buf[4];
    std::to_chars(buf, buf + sizeof(buf), port);
    int err;
    if (err = getaddrinfo(address.c_str(), buf, &hints, &result) < 0)
    {
        std::cout << "Error: " << errno << std::endl;
        return;
    }

    for (rp = result; rp; rp = rp->ai_next)
    {
        clientFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (clientFD == -1)
        {
            continue;
        }
        if (connect(clientFD, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            char host[NI_MAXHOST], service[NI_MAXSERV];
            int s = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
            break;
        }
        
        //close(clientFD);
    }

    freeaddrinfo(result);

    std::cout << "Socket FD: " << clientFD << std::endl;
}

int main(int argc, char *argv[])
{
    int client{};
    int port{};
    std::string address{};

    if (argc != 3)
    {
        std::cout << "Wrong amount of arguments";
        std::cout << "Arguments in order: [HOSTNAME] [PORT]" << std::endl;

        return -1;
    }
    
    address = argv[1]; // TODO: Check for correct format.
    port = std::stoi(argv[2]); // TODO: Check for correct format and port number.

    connectToServer(address, port);

    bool gameOver = false;
    while (gameOver == false)
    {
        char buf[1024];
        struct pollfd fds[1];
        fds[0].fd = clientFD;
        fds[0].events = POLLIN;
        int N{};
        if (poll(fds, 1, -1) > 0)
            N = recv(clientFD, buf, 1024, 0);
        else    
        {
            std::cout << "Something went wrong" << std::endl;
            return -1;
        }

        std::string rec {buf, N};
        if (N == -1)
        {
            std::cout << "Connection failed" << std::endl;
        }

        std::cout << "Recieved: " << N << " bytes" << std::endl;
        std::cout << rec;
        std::cout << std::endl;
    }
    close(clientFD);
    return 0;
}
