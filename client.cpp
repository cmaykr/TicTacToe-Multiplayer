#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <charconv>
#include <unistd.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
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

    struct addrinfo hints{};
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG;

    std::cout << port << " " << address << std::endl;
    char buf[4];
    std::to_chars(buf, buf + sizeof(buf), port);
    int err;
    if (err = getaddrinfo("127.0.0.1", "8080", &hints, &result) < 0)
    {
        std::cout << "Failed getting address" << std::endl;
        return -2;
    }

    int sock;
    for (rp = result; rp; rp = rp->ai_next)
    {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock == -1)
        {
            continue;
        }
        if (connect(sock, rp->ai_addr, rp->ai_addrlen))
        {
            char host[NI_MAXHOST], service[NI_MAXSERV];
            int s = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
            if (s == 0)
            {
                std::cout << host << " " << service << std::endl;
            }
            std::cout << "Success" << std::endl;
            break;
        }
        
        //close(sock);
    }

    freeaddrinfo(result);
    std::string message{};

    std::cin >> message;

    std::cout << "Sending message " << message << std::endl;
    if (send(sock, message.c_str(), 1024, 0) == -1)
    {
        std::cout << "Failed" << std::endl;
    }
    else
    {
        std::cout << "Sent" << std::endl;
    }

    close(sock);
    return 0;
}
