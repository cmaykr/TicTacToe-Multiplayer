#include "conn.hpp"

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
	
void Connection::connectToServer(std::string const& address, int port)
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
        return;
    }

    for (rp = result; rp; rp = rp->ai_next)
    {
        socketFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socketFD == -1)
        {
            continue;
        }
        if (connect(socketFD, rp->ai_addr, rp->ai_addrlen))
        {
            char host[NI_MAXHOST], service[NI_MAXSERV];
            int s = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
            break;
        }
        
        //close(sock);
    }

    freeaddrinfo(result);
}

void Connection::closeConnection() const
{
    int err = close(socketFD);

    if (err == -1)
    {
        throw std::logic_error("Socket failed to close");
    }
}

void Connection::sendMessage(std::string const& message)
{
    std::cout << message.size() << std::endl;
    send(socketFD, message.c_str(), message.size(), 0);
}