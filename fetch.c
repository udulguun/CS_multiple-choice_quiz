#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "quiz.h"

#define BUFFER_SIZE 1024

char *fetch(char *url)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return NULL;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return NULL;
    }

    if (pid == 0)
    {
        // Child process redirects stdout to the write end of the pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Execute curl
        char *args[] = {"curl", "-s", url, NULL};
        execvp("curl", args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process reads from the pipe
        close(pipefd[1]);
        char *response = malloc(BUFFER_SIZE);
        if (!response)
            return NULL;

        ssize_t bytesRead = read(pipefd[0], response, BUFFER_SIZE - 1);
        if (bytesRead < 0)
        {
            free(response);
            return NULL;
        }
        response[bytesRead] = '\0';
        close(pipefd[0]);

        int status;
        // Wait for child process to finish
        waitpid(pid, &status, 0);

        // Check if the child process finished without errors
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            return strdup(response);
        }
        else
        {
            fprintf(stderr, "child process fail");
            return NULL;
        }
    }
}
