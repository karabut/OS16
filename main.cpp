#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define ERROR -1
#define ERR_ISATTY 0

char answerTheQuestion(char *question) {

    printf("%s (enter - S if Spanish / enter - F if French)\n", question);

    char answer;

    struct termios termReserve, termInformation;

    if(fflush(stdout) == EOF) {
        perror("fflush error");
        return ERROR;
    }

    //Получить характеристики терминала (структура termios)
    if (tcgetattr(STDIN_FILENO, &termInformation) == ERROR) {
        perror("Error in getting terminal characteristics");
        return ERROR;
    }

    termReserve = termInformation;

    int count = 1;
    termInformation.c_lflag &= ~(ISIG | ICANON);
    termInformation.c_cc[VMIN] = count;

    //Изменить характеристики терминала (структура termios)
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termInformation) == ERROR) {
        perror(" Error in changing terminal characteristics");
        return ERROR;
    }

    if (read(STDIN_FILENO, &answer, count) == ERROR) {
        perror("Error in read");
        return ERROR;
    }

    //Изменить характеристики терминала (структура termios)
    if (tcsetattr(STDIN_FILENO, TCSANOW, &termReserve) == ERROR) {
        perror("Error in changing terminal characteristics");
        return ERROR;
    }

    return answer;
}

int main(){
    char answer;

    int fd = 0;

    if(isatty(fd) == ERR_ISATTY){
        perror("Issatty error");
        exit(EXIT_FAILURE);
    }

    answer = answerTheQuestion("Are you a Spanish or French learner?");
    printf("\n");

    if (answer == ERROR) {
        exit(EXIT_FAILURE);
    }

    switch (answer) {
        case 'S':
            printf("Hola\n");
            break;
        case 'F':
            printf("Bonjour\n");
            break;
        default:
            printf("Invalid answerTheQuestion\n");
            break;
    }
    exit(EXIT_SUCCESS);
}
