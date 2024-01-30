#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd[2];

    // Set default directory to current directory if no argument provided
    char *directory = argc > 1 ? argv[1] : ".";

    if (pipe(fd) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // child process
        close(fd[1]); // close write end
        dup2(fd[0], 0); // redirect pipe input to stdin
        close(fd[0]); // close read end

        // Execute wc command
        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    } else { // parent process
        close(fd[0]); // close read end
        dup2(fd[1], 1); // redirect stdout to pipe
        close(fd[1]); // close write end

        // Execute ls command
        execlp("ls", "ls", "-1a", directory, NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    return 0;
}
