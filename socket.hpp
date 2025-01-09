#pragma once

#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <charconv>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <poll.h>

class Socket
{
public:
    Socket(bool debugOutput)
        : debugOutput(debugOutput)
    {

    }

    Socket(bool debugOutput, int fd)
        : debugOutput(debugOutput), socketFD(fd)
    {

    }

    ~Socket()
    {
        closeSocket();
    }

    bool initializeServer(int port)
    {
        if (isInitialized)
            return false;

        struct addrinfo hints;
        struct addrinfo *rp, *result;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        char buf[4];
        std::to_chars(buf, buf + sizeof(buf), port);
        int err = getaddrinfo(NULL, buf, &hints, &result);
        if (err < 0)
        {
            std::cerr << "Error getting address" << std::endl;
            printError();
            exit(2);
        }

        for (rp = result; rp != nullptr; rp = rp->ai_next)
        {
            socketFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

            if (socketFD == -1)
                continue;
            if (bind(socketFD, rp->ai_addr, rp->ai_addrlen) == 0)
            {
                char host[NI_MAXHOST], service[NI_MAXSERV];
                break;
            }

            closeSocket();
        }

        freeaddrinfo(result);
        if (listen(socketFD, 5) < 0)
        {
            std::cerr << "Failed to prepare socket for accepting connections.";
            exit(1);
        }

        isInitialized = true;

        return true;
    }

    Socket acceptConnection()
    {
        if (debugOutput)
            std::cout << "Awaiting connection" << std::endl;
        int fd = accept(socketFD, NULL, NULL);

        if (fd == -1)
        {
            std::cerr << "Failed to accept connection." << std::endl;
            exit(1);
        }

        socketFD = fd;
        return Socket(debugOutput, fd);
    }

    void initializeClientAndConnectToServer(std::string const& address, int port)
    {
        if (isInitialized)
        {
            std::cerr << "Socket already initialized" << std::endl;
            exit(1);
        }

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
            printError();
            exit(1);
        }

        for (rp = result; rp; rp = rp->ai_next)
        {
            socketFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (socketFD == -1)
            {
                continue;
            }
            if (connect(socketFD, rp->ai_addr, rp->ai_addrlen) == 0)
            {
                break;
            }
            
            closeSocket();
        }

        freeaddrinfo(result);
        
        if (debugOutput)
            std::cout << "Connection success" << std::endl;
        isInitialized = true;
    }

    /// @brief Polls the socket and gets the message if there is one.
    /// @return The recieved message, if there is no message it will return empty.
    std::string pollAndRecieveMessage()
    {
        char buf[1024];
        struct pollfd fds[1];
        fds[0].fd = socketFD;
        fds[0].events = POLLIN;
        int N{};
        int polls = poll(fds, 1, -1);
        if (polls > 0)
        {
            N = recv(socketFD, buf, sizeof(buf), 0);
            if (N == -1)
            {
                std::cerr << "Socket failed when receiving message" << std::endl;
                closeSocket();
                exit(1);
            }
            if (N == 0)
            {
                std::cerr << "Peer socket not available, closing connection" << std::endl;
                closeSocket();
                exit(2);
            }
            std::string message {buf, N};
            if (debugOutput)
                std::cout << "Received: " << buf << " of size: " << N << std::endl;
            return message;
        }
        else if (polls < 0)
        {
            std::cerr << "Nothing to receive" << std::endl;
            printError();
            closeSocket();
            return "";
        }

        return "";
    }

    /// @brief Sends a message on the socket.
    /// @param message The message to send.
    void sendMessage(std::string const& message)
    {
        if (send(socketFD, message.c_str(), message.size(), 0) == -1)
        {
            printError();
            closeSocket();
            exit(1);
        }
    }

private:
    int socketFD;
    
    bool isInitialized{false};
    bool debugOutput;

    void printError()
    {
        std::cerr << "Error code: " << strerror(errno) << std::endl;
    }

    void closeSocket()
    {
        if (debugOutput)
            std::cout << "Closing socket with FD: " << socketFD << std::endl;
        shutdown(socketFD, SHUT_RDWR);
        close(socketFD);
    }
};