# Among Ruz

## Instructions for executing the program
There is a Makefile for compiling the server and client files. To run the program through the terminal, go to the main folder, run the make command, and then type:

$ ./server -i <ip_address> -p <tcp_port>  
$ ./client -i <ip_address> -p <tcp_port>  

where ip_address corresponds to the IP address of the server in human-readable format and tcp_port corresponds to the port through which new connections will be received.
This work was done by testing the IP address 0.0.0.0 and port 8080.


## Description of the packages used in the communication between client and server
To create the packages, the same format as seen in the assistantship was used: ID, PayLoadSize and PayLoad. The ID dictates what the message is for:
- 1: message issued by the server
- 2 - 9: message from a specific player
- 10 - 17: private message from a specific player via whisper
- 18 - 25: message from a specific ghost player
- 255: disconnection

## What were the main design decisions to build the program
- The ghost chat is only for expelled, not eliminated.

## What are the main functions of the program
### Client - communication.h
- void client_send_message (int client_socket, int pkg_id, char * message): receives as arguments an int that represents the client socket, another int that represents the packet id and a pointer to a char of the message. Returns empty.
- int client_receive_id (int client_socket): receives as an argument an int that represents the client's socket. Returns an int that represents the ID of the message.
- char * client_receive_payload (int client_socket): receives as an argument an int that represents the client's socket. Returns a pointer to a char that represents the payload.

### Client - connection.h
- int prepare_socket (char * IP, int PORT): receives as arguments a pointer to a char of the IP address and an int that represents the port. Returns an int that represents the client socket.

### Client - main.c
- void * recv_msg_handler (void * arguments): receives a pointer to a void as an argument. Returns an empty pointer.
- void * send_msg_handler (void * arguments): receives a pointer to a void as an argument. Returns an empty pointer.
- void str_trim_lf (char * arr, int length): receives as arguments a pointer to an array of chars and an int. Returns empty.
- int main (int argc, char * argv []): function that handles the client's main program.

### Server - communication.h
- int server_receive_id (int client_socket): receives an int from the client socket as an argument. Returns an int that represents the ID of the message.
- char * server_receive_payload (int client_socket): receives as an argument an int that represents the client's socket. Returns a pointer to a char that represents the payload.
- void server_send_message (int client_socket, int pkg_id, char * message): receives as arguments an int that represents the client socket, another int that represents the packet id and a pointer to a char of the message. Returns empty.

### Server - connection.h
- void * prepare_sockets_and_get_clients (void * arguments): receives a pointer to a void as an argument. Returns an empty pointer.
- void message_initial (int player_number, struct arg_struct * arguments, char colors [8] [9]): receives as arguments an int that represents the player's number, a pointer to a struct arg_struct and a vector of chars. Returns empty.

### Server - main.c
- void * recv_msg_handler (void * arguments): receives a pointer to a void as an argument. Returns an empty pointer.
- void message_handler (char * message, int socket_number, struct arg_struct * arg_struct): receives as arguments a pointer to a message char, an int that represents the socket number and a pointer to a struct of type arg_struct. Returns empty.
- int players_connected (struct arg_struct * arg_struct): receives as an argument a pointer to a struct arg_struct. Returns an int that represents the number of connected players.
- int * random_numbers (int lower, int upper, int count): receives an int lower, an int upper and an int count as an argument. Returns a pointer to an int that represents an array of integers of the number of the player that will be the imposter.
- int check_game (struct arg_struct * arg_struct): receives as an argument a pointer to a struct of type arg_struct. Returns an int 0 or 1, depending on whether the game continues or not respectively.
- void check_votation (struct arg_struct * arg_struct): receives as an argument a pointer to a struct of type arg_struct. Returns empty.
- int main (int argc, char * argv []): function that manages the main program of the server.

## What additional assumptions did they occupy

## Any information they consider necessary to facilitate the correction of their task
- Whisper works even if the game is not initialized.
- Whisper reaches any player, whether it is in state 0 (disconnected) or state> 0 (connected: its socket number).
