#include <stdio.h>
#include <stdlib.h>
#include "QuizDB.h" // quiz data base

int main(int argc, char **argv) {
    // get number of questions by dividing byte size of question array by single question
    int numberOfQuestions = sizeof(QuizQ) / sizeof(QuizQ[0]);
    // loop through all questions to print its question and answer
    for (int i = 0; i < numberOfQuestions; i++) { printf("Q. %s\n", QuizQ[i]); printf("A. %s\n", QuizA[i]); }
    exit(0);
}