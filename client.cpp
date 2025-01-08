#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <charconv>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <poll.h>
#include <algorithm>

#include <chrono>
#include <thread>

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
        if (poll(fds, 1, -1) > 0) // Currently blocks program completely until something happens, can probably stall program.
            N = recv(clientFD, buf, 1024, 0);
        else    
        {
            std::cout << "Something went wrong" << std::endl;
            return -1;
        }

        if (N == -1 || N == 0)
        {
            std::cout << "Connection failed" << std::endl;
        }
        std::string rec {buf, (long unsigned int)N};

        std::cout << "Recieved: " << N << " bytes" << std::endl;
        std::for_each(rec.begin(), rec.end() - 1, [](char c)
        {
            std::cout << c;
        });
        std::cout << std::endl;

        if (rec.at(N - 1) == '2')
        {
            std::cout << "Your turn, choose an empty spot to place your o. 1-9" << std::endl;
            int spot{};
            std::cin >> spot;
            std::cout << "Sending message: " << spot << std::endl;
            char message[1];
            message[0] = spot;
            send(clientFD, message, sizeof(message), 0);
            char buf[1024];
            int N = recv(clientFD, buf, sizeof(buf), 0);

            std::string ans{buf, (long unsigned int)N};
            while (ans == "invalid" && ans != "valid")
            {
                std::cin >> spot;
                send(clientFD, &spot, 1, 0);

                N = recv(clientFD, buf, sizeof(buf), 0);
                ans = std::string(buf, N);
            }
        }
        else if (rec.at(N - 1) == '3')
        {
            std::cout << "Game is over, player one won the game." << std::endl;
            gameOver = true;
        }
        else if (rec.at(N - 1) == '4')
        {
            std::cout << "Game is over, player two (you) won the game." << std::endl;
            gameOver = true;
        }
        else if (rec.at(N - 1) == '1')
        {
            std::cout << "Other players turn" << std::endl;
        }
        else if (rec.at(N - 1) == '5')
        {
            std::cout << "Game is a draw, tough luck!" << std::endl;
            gameOver = true;
        }
        else
        {
            std::cout << "Invalid message, ending session." << std::endl;
            gameOver = true;
        }
    }

    close(clientFD);
    return 0;
}
