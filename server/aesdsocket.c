#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 9000
#define DATA_FILE "/var/tmp/aesdsocketdata"
#define MAX_PACKET_SIZE 1024

volatile sig_atomic_t flag = 0;

void sig_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        syslog(LOG_INFO, "Caught signal, exiting");
        flag = 1;
    }
}

int main(int argc, char *argv[]) {
    int daemon_mode = 0;

    // Check if -d argument is provided
    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        daemon_mode = 1;
    }

    // Signal handling
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    // Daemonization if in daemon mode
    if (daemon_mode) {
        pid_t pid = fork();
        if (pid < 0) {
            syslog(LOG_ERR, "Fork failed: %s", strerror(errno));
            
            return -1;
        }
        if (pid > 0) {
            // Parent process
            return 0;
        }
        
        // Child process continues execution
        umask(0);
        if (setsid() < 0) {
            syslog(LOG_ERR, "setsid failed: %s", strerror(errno));
            return -1;
        }
        if (chdir("/") < 0) {
            syslog(LOG_ERR, "chdir failed: %s", strerror(errno));
            return -1;
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }

    // Open syslog for logging
    openlog("aesdsocket", LOG_PID, LOG_DAEMON);

    // Create a socket
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        syslog(LOG_ERR, "Socket failed: %s", strerror(errno));
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9000);

    // Bind the socket
    int bind_ret = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_ret < 0) {
        if (errno == EADDRINUSE) {
            syslog(LOG_ERR, "Port 9000 is already in use");
        } else {
            syslog(LOG_ERR, "Bind failed: %s", strerror(errno));
        }
        return -1;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        syslog(LOG_ERR, "Listen failed: %s", strerror(errno));
        return -1;
    }

    while (!flag) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            syslog(LOG_ERR, "Accept failed: %s", strerror(errno));
            return -1;
        }

        // Log accepted connection
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        syslog(LOG_INFO, "Accepted connection from %s", client_ip);

        // Receive data and append to file
        char buffer[1024];
        int valread;
        FILE *file = fopen("/var/tmp/aesdsocketdata", "a");
        if (file == NULL) {
            syslog(LOG_ERR, "File open failed: %s", strerror(errno));
            return -1;
        }

        while ((valread = recv(new_socket, buffer, sizeof(buffer), 0)) > 0) {
            fwrite(buffer, 1, valread, file);
            // Check for newline to consider packet completion
            if (memchr(buffer, '\n', valread) != NULL) {
                break;
            }
        }
        fclose(file);

        // Read the contents of the file
        file = fopen("/var/tmp/aesdsocketdata", "r");
        if (file == NULL) {
            syslog(LOG_ERR, "File open failed: %s", strerror(errno));
            return -1;
        }

        // Calculate the file size
        fseek(file, 0L, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        // Allocate memory to hold file contents
        char *file_contents = malloc(file_size);
        if (file_contents == NULL) {
            syslog(LOG_ERR, "Memory allocation failed: %s", strerror(errno));
            return -1;
        }

        // Read file contents
        size_t read_bytes = fread(file_contents, 1, file_size, file);
        fclose(file);

        // Send the file contents back to the client
        if (send(new_socket, file_contents, read_bytes, 0) != read_bytes) {
            syslog(LOG_ERR, "Send failed: %s", strerror(errno));
            return -1;
        }

        free(file_contents); // Free allocated memory

        // Log closed connection
        syslog(LOG_INFO, "Closed connection from %s", client_ip);
        close(new_socket);
    }
    close(server_fd);
    syslog(LOG_INFO, "Caught signal, exiting");
    closelog();
    unlink(DATA_FILE);
    return 0;
}
