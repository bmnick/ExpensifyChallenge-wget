//
//  main.cpp
//  mywget
//
//  Created by Ben Nicholas on 11/17/11.
//  Copyright (c) 2011 Oracle Corporation. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void usage(void);
std::string get_url(const char* url);
bool write_file(std::string outfile_name, std::string contents);

int main (int argc, const char * argv[])
{
    if (argc < 2) {
        usage();
        return 1;
    }

    std::string site_contents = get_url(argv[1]);
    
    write_file("wget.out", site_contents);
    
    return 0;
}

void usage(void) {
    std::cout << "Usage: mywget url" << std::endl;
}

std::string get_url(const char* url) {
    int sockfd;
    int num_chars;
    struct hostent * server;
    struct sockaddr_in sock_addr;
    char buffer[1024];
    std::string response;
    
    // create and check socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: could not create socket";
        exit(2);
    }
    
    // create hostent for fetching against
    server = gethostbyname(url);
    if (server == NULL) {
        std::cerr << "Error: server not found";
        exit(3);
    }
    
    // Build socket address
    bzero((void *)&sock_addr, sizeof(sockaddr_in));
    sock_addr.sin_family = PF_INET;
    sock_addr.sin_len = server->h_length;
    bcopy((char *)server->h_addr, (char *)&sock_addr.sin_addr.s_addr, server->h_length);
    sock_addr.sin_port = htons(80);
    
    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        std::cerr << "Error: trouble connecting to server";
        close(sockfd);
        exit(4);
    }
    
    // build command to send to server
    std::string get_command = "GET ";
    get_command.append("/\r\n\r\n");

    num_chars = write(sockfd, get_command.c_str(), get_command.length()+1);
    if (num_chars < 0) {
        std::cerr << "Error: trouble sending command to server";
        close(sockfd);
        exit(5);
    }
    
    while ((num_chars = recv(sockfd, buffer, 1023, 0)) > 0) {
        std::cout << "recieved " << num_chars << "bytes from server" << std::endl;
        response.append(buffer);
    }
    if (num_chars < 0) {
        std::cerr << "Error: trouble reading from server";
        close(sockfd);
        exit(6);
    }
    
    return response;
}

bool write_file(std::string outfile_name, std::string contents) {
    std::ofstream out_file(outfile_name.c_str());
    
    if (out_file.is_open()) {
        out_file << contents;
    } else {
        return false;
    }
    
    out_file.close();
    
    return true;
}
