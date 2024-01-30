#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s output_file command [args...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the output file
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Redirect stdout to the output file descriptor
    if (dup2(fd, 1) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    // Close the file descriptor as it's no longer needed
    close(fd);

    // Execute the command with its arguments
    execvp(argv[2], &argv[2]);
    perror("execvp");
    exit(EXIT_FAILURE);

    return 0;
}
