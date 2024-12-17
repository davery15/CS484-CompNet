/*
 * Name: Davery Valdez
 * Assignment: CS 484 Programming Assignment 1
 * Purpose: This program connects to an SMTP server using network sockets
 *          and sends an email to a specified recipient. The email content
 *          is read from a file provided via command-line arguments.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SMTP_PORT 25  // SMTP runs on port 25
#define BUFFER_SIZE 1024

// Function prototypes
void smtp_send(int socket, const char *msg);
void smtp_receive(int socket, char *buffer, size_t size);
void send_email(int socket, const char *from, const char *to, const char *message);

int main(int argc, char *argv[]) {
    // Check if the number of arguments is correct (3 args)
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <SMTP ADDR IPv4> <DEST Email ADDR> <Email Filename>\n", argv[0]);
        exit(EXIT_FAILURE); // Exit the program if the argument count is wrong
    }

    // Extract arguments
    const char *smtp_server_ip = argv[1]; // The IP address of the SMTP server
    const char *dest_email = argv[2]; // The destination email address
    const char *email_filename = argv[3]; // The filename containing the email content

    // Step 1: Create a socket
    int smtp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (smtp_socket == -1) {
        // If socket creation fails, print an error and exit
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Set up the server address struct
    struct sockaddr_in smtp_server;
    smtp_server.sin_family = AF_INET; // Use IPv4 addresses
    smtp_server.sin_port = htons(SMTP_PORT); // Set port number, converting it to network byte order (big-endian)

    // Convert the IP address to binary form and store it in 'smtp_server.sin_addr
    if (inet_pton(AF_INET, smtp_server_ip, &smtp_server.sin_addr) <= 0) {
        perror("Invalid IP address"); //If IP conversion fails
        exit(EXIT_FAILURE);
    }

    // Step 3: Connect to the SMTP server
    if (connect(smtp_socket, (struct sockaddr *)&smtp_server, sizeof(smtp_server)) < 0) {
        perror("Connection to SMTP server failed"); // If connection fials
        exit(EXIT_FAILURE);
    }

    // Step 4: Read the email file for reading
    FILE *email_file = fopen(email_filename, "r");
    if (!email_file) { // If the file cannot be opened, print an error and exit
        perror("Failed to open email file");
        exit(EXIT_FAILURE);
    }

    // Buffer to store the email content
    char email_content[BUFFER_SIZE] = {0};
    fread(email_content, sizeof(char), BUFFER_SIZE, email_file);
    fclose(email_file);

    // Step 5: SMTP Communication (HELO, MAIL FROM, RCPT TO, DATA)
    char buffer[BUFFER_SIZE];

    smtp_receive(smtp_socket, buffer, sizeof(buffer));  // Welcome message
    smtp_send(smtp_socket, "HELO localhost\r\n");       // HELO command
    smtp_receive(smtp_socket, buffer, sizeof(buffer));

    // MAIL FROM
    char mail_from_cmd[BUFFER_SIZE];
    snprintf(mail_from_cmd, sizeof(mail_from_cmd), "MAIL FROM:<davery15@nmsu.edu>\r\n");
    smtp_send(smtp_socket, mail_from_cmd);
    smtp_receive(smtp_socket, buffer, sizeof(buffer));

    // RCPT TO
    char rcpt_to_cmd[BUFFER_SIZE];
    snprintf(rcpt_to_cmd, sizeof(rcpt_to_cmd), "RCPT TO:<%s>\r\n", dest_email);
    smtp_send(smtp_socket, rcpt_to_cmd);
    smtp_receive(smtp_socket, buffer, sizeof(buffer));

    // Send the DATA command (begin sending email data)
    smtp_send(smtp_socket, "DATA\r\n");
    smtp_receive(smtp_socket, buffer, sizeof(buffer));

    // Send email content followed by "." to end the email
    smtp_send(smtp_socket, email_content);
    smtp_send(smtp_socket, "\r\n.\r\n");
    smtp_receive(smtp_socket, buffer, sizeof(buffer));

    // Send the QUIT command (end the SMTP session)
    smtp_send(smtp_socket, "QUIT\r\n");
    smtp_receive(smtp_socket, buffer, sizeof(buffer));

    // Step 6: Close the socket
    close(smtp_socket);

    return 0; // exit
}

/*
 * Function: smtp_send
 * Purpose: Send a message to the SMTP server over the given socket.
 * Inputs:
 *   - socket (int): The socket descriptor connected to the SMTP server.
 *   - msg (const char*): The message to be sent to the server.
 * Outputs: None (This function does not return a value, but it sends the message to the socket).
 */
void smtp_send(int socket, const char *msg) {
    send(socket, msg, strlen(msg), 0);
    printf("C: %s", msg);  // Print client-side communication
}

/*
 * Function: smtp_receive
 * Purpose: Receive a response from the SMTP server and store it in the buffer.
 * Inputs:
 *   - socket (int): The socket descriptor connected to the SMTP server.
 *   - buffer (char*): The buffer to store the server's response.
 *   - size (size_t): The size of the buffer.
 * Outputs: None (This function does not return a value, but it populates the buffer with server data).
 */
void smtp_receive(int socket, char *buffer, size_t size) {
    int len = recv(socket, buffer, size - 1, 0);
    if (len > 0) {
        buffer[len] = '\0';
        printf("S: %s\n", buffer);  // Print server-side communication
    } else {
        perror("Error reading from socket");
        exit(EXIT_FAILURE);
    }
}