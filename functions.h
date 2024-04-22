////
//// Created by Shane Easo on 19/04/2024.
////
//#include <time.h>
//#include "QuizDB.h"
//#ifndef COMP20200A3_FUNCTIONS_H
//#define COMP20200A3_FUNCTIONS_H
//
//#define MAX_QUESTIONS 5 // maximum number of questions
//#define BUFFER_SIZE 10000 // buffer size to hold text outputs
//
//// if in terminal args number does not match required amount
//void printClientUsage() { fprintf(stderr, "Usage: ./client <IPv4 address> <port number>\n"); exit(1); }
//// swap array elements by creating temporary variable and swapping
//void swap(char **array1, char **array2) { char *temp = *array1; *array1 = *array2; *array2 = temp; }
//// error handling, take string and print as perror and exit
//void printError(const char *msg) { perror(msg); exit(1); }
//// if in terminal args number does not match required amount
//void printServerUsage() { fprintf(stderr, "Usage: ./server <IPv4 address> <port number>\n"); exit(1); }
//// ensures questions given are randomised
//void randomiseQuestions(char *questions[], char *answers[], int numOfQuestions) { // questionsDB, answersDB, num of questions in DB
//    srand(time(NULL)); // random seed with epoch time
//    // shuffle using Fisher Yates algorithm found online
//    for (int i = numOfQuestions - 1; i > 0; i--) {// start from last element, i > 0 because we don't need to run for first element
//        int j = rand() % (i + 1); //j is random num from 1 to i
//        swap(&questions[i], &questions[j]);// swap questions at index i and j
//        swap(&answers[i], &answers[j]); // swap answers at index i and j
//    }
//}
//// Function to serve a connected client
//void toClient(int clientSocket) {
//    char buffer[BUFFER_SIZE];
//    int score = 0;
//
//    // Send quiz preamble to client
//    snprintf(buffer, BUFFER_SIZE, "Welcome to Unix Programming Quiz!\n");
//    write(clientSocket, buffer, strlen(buffer));
//
//    // Wait for client response to start the quiz
//    if (read(clientSocket, buffer, BUFFER_SIZE) <= 0 || buffer[0] != 'Y') {
//        close(clientSocket);
//        return;
//    }
//
//    randomiseQuestions(QuizQ, QuizA, sizeof(QuizQ) / sizeof(QuizQ[0]));
//
//    // Loop to handle quiz questions
//    for (int currentQuestion = 0; currentQuestion < MAX_QUESTIONS; ++currentQuestion) {
//        // Send the question to the client
//        memset(buffer, 0, sizeof(buffer));
//        snprintf(buffer, BUFFER_SIZE, "Question %d: %s\n", currentQuestion + 1, QuizQ[currentQuestion]);
//        write(clientSocket, buffer, strlen(buffer));
//
//        // Receive answer from client
//        memset(buffer, 0, BUFFER_SIZE);
//        if (read(clientSocket, buffer, BUFFER_SIZE) <= 0) {
//            close(clientSocket);
//            return;
//        }
//
//        // if correct, send message and increment score otherwise send message with correct answer
//        if (strcmp(buffer, QuizA[currentQuestion]) == 0) {
//            snprintf(buffer, BUFFER_SIZE, "Right Answer.\n");
//            score++;
//        } else
//            snprintf(buffer, BUFFER_SIZE, "Wrong Answer. Right answer is %s.\n", QuizA[currentQuestion]);
//        write(clientSocket, buffer, strlen(buffer));
//        memset(buffer, 0, sizeof(buffer));
//    }
//
//    // send final score message
//    snprintf(buffer, BUFFER_SIZE, "Your quiz score is %d/5. Goodbye!\n", score);
//    write(clientSocket, buffer, strlen(buffer));
//
//    close(clientSocket); // close socket
//}
//
//#endif //COMP20200A3_FUNCTIONS_H
