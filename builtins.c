#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    char input[BUFFER_SIZE];
    char* arguments[BUFFER_SIZE / 2 + 1];

    while (1) {
        // Display the prompt and wait for user input
        printf("$ ");
        fflush(stdout);

        if (!fgets(input, BUFFER_SIZE, stdin)) {
            break;
        }

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = 0;

        // Split the input into words
        char* token = strtok(input, " ");
        int i = 0;
        while (token) {
            arguments[i++] = token;
            token = strtok(NULL, " ");
        }
        arguments[i] = NULL;

        // Try to execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (execvp(arguments[0], arguments) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            wait(NULL);
        }
    }

    return 0;
}
