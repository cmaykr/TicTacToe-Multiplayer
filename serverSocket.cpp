#include "serverSocket.hpp"

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

void ServerSocket::createServer(int port)
{
        struct addrinfo hints;
    struct addrinfo *rp, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int err = getaddrinfo(NULL, "8080", &hints, &result);
    if (err < 0)
    {
        std::cout << "Error getting address" << std::endl;
    }

    for (rp = result; rp != nullptr; rp = rp->ai_next)
    {
        socketFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (socketFD == -1)
            continue;
        if (bind(socketFD, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            char host[NI_MAXHOST], service[NI_MAXSERV];
            int s = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
            if (s == 0)
            {
                std::cout << host << " dd " << service << std::endl;
            }
            break;
        }
        else
        {
            std::cout << "Fail" << std::endl;
        }

        close(socketFD);
    }

    freeaddrinfo(result);
    std::cout << socketFD << std::endl;
    listen(socketFD, 5);
}

std::string ServerSocket::recieveMessage()
{
    std::cout << "Waiting to recieve" << std::endl;
    int clientFD = accept(socketFD, NULL, NULL);
    this->clientFD = clientFD;
    if (clientFD == -1)
     {
        std::cout << "Client failed" << std::endl;
     }

    char buf[1024];
    int numRead = recv(clientFD, buf, sizeof(buf), 0);
    std::cout << "Message: " << buf[3] << numRead << std::endl;

    std::string message{};

    for (int i{}; i < numRead; i++)
    {
        message.push_back(buf[i]);
    }
    return message;
}

void ServerSocket::closeServer() const
{
    close(clientFD);
    close(socketFD);
}