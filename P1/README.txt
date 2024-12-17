SMTP Email Sender in C

Overview
This project implements an email-sending program in C using SMTP over network sockets. 
The program connects to an SMTP server, reads an email template from a file, and sends it to a
specified recipient.

Files
	1.	email_sender.c - Main C source code for the program.
	2.	Makefile - Allows for easy compilation of the project.
	3.	P1.pdf - Assignment description and guidelines.

How to Run

Compilation

To compile the program, use the provided Makefile: Make

Run the program using the following syntax:
./email_sender <SMTP_ADDR IPv4> <DEST_EMAIL_ADDR> <EMAIL_FILENAME>

	•	<SMTP_ADDR IPv4>: IPv4 address of the SMTP server (e.g., 128.123.88.221).
	•	<DEST_EMAIL_ADDR>: Recipient’s email address.
	•	<EMAIL_FILENAME>: Path to the text file containing the email content.

    Features
	•	Connects to an SMTP server using network sockets.
	•	Reads email content from a specified file.
	•	Sends emails following SMTP protocol (RFC 2821).
	•	Handles errors such as:
	•	Invalid command-line arguments
	•	Invalid email/IP addresses
	•	Socket creation/connection failures
	•	SMTP server rejection or multi-line responses.

    Notes
	•	Do not spoof sender emails. Use your official email address.
	•	Tested on OpenSUSE Leap 15.6 (NMSU lab computers).

Author

Davery Valdez
New Mexico State University
Course: CS 484/504 - Computer Networks I