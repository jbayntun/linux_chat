#ifndef NETWORK
#define NETWORK

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>

#define SEND_SIZE	255
#define RECEIVE_SIZE 1023
const static char user_char = '^';
const static char remove_user = '%';

std::string initChat(std::string& host, int port, std::string& user);
void closeChat();
bool connectReceiveSocket(std::string& host, int port);
bool connectSendSocket(std::string& host, int port);
int connectTCP(std::string& host, int port);
void addUser(int sd, char* userName);

void sendMessage(std::string& message);
std::string receiveMessage();

#endif // NETWORK

