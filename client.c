#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFFER_SIZE 10000 // size for holding console output

// error handling, take string and print as perror and exit
void printError(const char *msg) { perror(msg); exit(1); }
// if in terminal args number does not match required amount
void printClientUsage() { fprintf(stderr, "Usage: ./client <IPv4 address> <port number>\n"); exit(1); }

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE]; // stores console output
    char response; // play/quit option
    const char *ipAddress = argv[1]; // 2nd arg is ip address
    int portNumber = atoi(argv[2]); // 3rd arg is port number
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // creating sequential ipv4 client socket
    struct sockaddr_in socketAddress;

    if (argc != 3) printClientUsage(); // print usage if under 3 arguments given
    if (clientSocket < 0) printError("Error opening socket"); // if client sockets creation fails

    memset((char *) &socketAddress, 0, sizeof(socketAddress)); // initialise struct with 0 values
    socketAddress.sin_family = AF_INET; // socket address must be ipv4
    socketAddress.sin_addr.s_addr = inet_addr(ipAddress); // convert ip string to binary
    socketAddress.sin_port = htons(portNumber); // sets port number using htons converting

    // create connection to address, if error occurs, print error
    if (connect(clientSocket, (struct sockaddr *) &socketAddress, sizeof(socketAddress)) < 0) printError("Error connecting");
    // read data from socket, if error occurs, print error
    if (read(clientSocket, buffer, BUFFER_SIZE) < 0) printError("Error reading from socket");

    printf("%s", buffer); // print welcome message to console
    memset(buffer, 0, BUFFER_SIZE); // reset buffer to 0
    scanf(" %c", &response); // scan response char, space present for handling new line
    response = toupper(response); // ensures input is uppercase
    if (response != 'Y') { close(clientSocket); exit(0); } // if anything but 'Y', close client socket and exit
    write(clientSocket, &response, sizeof(response)); // write response to clientSocket, server then reads given data.
    memset(buffer, 0, BUFFER_SIZE); // reset buffer to 0
    for (int i = 0; i < 5; i++) {
        memset(buffer, 0, BUFFER_SIZE); // reset buffer to 0
        // read from client socket, if error occurs, print error
        if (read(clientSocket, buffer, BUFFER_SIZE) < 0) printError("Error reading from socket");
        printf("%s", buffer); // print out buffer to client console
        memset(buffer, 0, BUFFER_SIZE); // reset buffer to 0
        scanf("%s", buffer); // take in answer as string
        write(clientSocket, buffer, strlen(buffer)); // write input to client socket
        memset(buffer, 0, BUFFER_SIZE); // reset buffer to 0
    }
    memset(buffer, 0, BUFFER_SIZE); // reset buffer to 0

    // read final score sent by server to client, if error occurs, print out perror
    if (read(clientSocket, buffer, BUFFER_SIZE) < 0) printError("Error reading from socket");

    printf("%s", buffer); // print out final score to console

    close(clientSocket); // close client socket
    return 0;
}
