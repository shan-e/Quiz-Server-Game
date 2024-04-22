#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#include "QuizDB.h" // quiz database

#define MAX_QUESTIONS 5 // maximum number of questions
#define BUFFER_SIZE 10000 // buffer size to hold text outputs

// swap array elements by creating temporary variable and swapping
void swap(char **array1, char **array2) { char *temp = *array1; *array1 = *array2; *array2 = temp; }
// error handling, take string and print as perror and exit
void printError(const char *msg) { perror(msg); exit(1); }
// if in terminal args number does not match required amount
void printServerUsage() { fprintf(stderr, "Usage: ./server <IPv4 address> <port number>\n"); exit(1); }
// ensures questions given are randomised using Fisher-Yates algorithm
void randomiseQuestions(char *questions[], char *answers[], int numOfQuestions) { // questionsDB, answersDB, num of questions in DB
    srand(time(NULL)); // random seed with epoch time
    // shuffle using Fisher Yates algorithm found online
    for (int i = numOfQuestions - 1; i > 0; i--) {// start from last element, i > 0 because we don't need to run for first element
        int j = rand() % (i + 1); //j is random num from 1 to i
        swap(&questions[i], &questions[j]);// swap questions at index i and j
        swap(&answers[i], &answers[j]); // swap answers at index i and j
    }
}
// send questions and prompts to client and receive answers
void toClient(int clientSocket) { // take in client socket file descriptor
    char buffer[BUFFER_SIZE]; // console output
    int score = 0; // player final score

    // assign string to buffer and send to client using write
    snprintf(buffer, BUFFER_SIZE, "Welcome to Unix Programming Quiz!\n");
    write(clientSocket, buffer, strlen(buffer));

    // if client inputs anything except 'Y' or if error occurs reading, close the client socket and exit while loop
    if (read(clientSocket, buffer, BUFFER_SIZE) < 0 || buffer[0] != 'Y') { close(clientSocket); return; }

    // randomise quizDB using Fisher-Yates algorithm
    randomiseQuestions(QuizQ, QuizA, sizeof(QuizQ) / sizeof(QuizQ[0]));

    // Loop to handle quiz questions
    for (int currentQuestion = 0; currentQuestion < MAX_QUESTIONS; ++currentQuestion) {
        // assign string to buffer and send to client via clientSocket
        snprintf(buffer, BUFFER_SIZE, "Question %d: %s\n", currentQuestion + 1, QuizQ[currentQuestion]);
        write(clientSocket, buffer, strlen(buffer));

        memset(buffer, 0, BUFFER_SIZE); // clear buffer with 0s via memset
        // if error occurs reading, close the client socket and exit while loop
        if (read(clientSocket, buffer, BUFFER_SIZE) < 0) { close(clientSocket); return; }

        // if correct answer, buffer saved with message and score is incremented
        if (strcmp(buffer, QuizA[currentQuestion]) == 0) { snprintf(buffer, BUFFER_SIZE, "Right Answer.\n"); score++; }
        // otherwise, saved buffer to write expected answer
        else snprintf(buffer, BUFFER_SIZE, "Wrong Answer. Right answer is %s.\n", QuizA[currentQuestion]);
        // write buffer to client socket
        write(clientSocket, buffer, strlen(buffer));
    }

    // format and write final score message to client socket
    snprintf(buffer, BUFFER_SIZE, "Your quiz score is %d/5. Goodbye!\n", score);
    write(clientSocket, buffer, strlen(buffer));

    close(clientSocket); // close client socket
}

int main(int argc, char *argv[]) {
    const char *ipAddress = argv[1]; // ip address is second argument given
    int portNumber = atoi(argv[2]); // port number is third argument given and converted to int
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);// creating sequential ipv4 server socket
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

    if (argc != 3) printServerUsage(); // print usage if under 3 arguments given
    if (serverSocket < 0) printError("Error opening socket"); // if error creating server socket, print issue

    // ensure server info is formatted properly but initialising address to zero
    memset((char *) &serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; // ensure ipv4 address
    serverAddress.sin_addr.s_addr = inet_addr(ipAddress); // convert ip string to binary
    serverAddress.sin_port = htons(portNumber); // sets port number using htons converting

    // bind socket server to given address, if error occurs binding, print issue
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) printError("Error on binding");
    listen(serverSocket, 0); // socket listens for incoming connections
    printf("Listening on ip: %s port: %d\n", ipAddress, portNumber); // print connection info to console

    for (;;) { // keep server open and running for client to connect
        socklen_t clientSize = sizeof(clientAddress); // length of client address
        // file descriptor accepts connections from given client socket to server socket
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientSize);
        // if there is error connecting client socket, print issue
        if (clientSocket < 0) printError("Error accepting connection");
        else printf("Client connected!!\n"); // otherwise print established
        toClient(clientSocket); // read and write data to client via client socket
    }

    close(serverSocket); // close server socket
    return 0;
}