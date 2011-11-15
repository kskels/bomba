
#include <connection.h>
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


void Connection::connect(const std::string &address, unsigned port) {
    struct sockaddr_in serv_addr;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0) 
        std::cout << "Failed to open the socket" << std::endl;
    struct hostent *server = gethostbyname(address.c_str());
    if (!server) {
        std::cout << "Failet to get host by name" << std::endl;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if (::connect(_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        std::cout << "Failed to connect" << std::endl;
    std::cout << "Done" << std::endl;
}

void Connection::disconnect() {

}

