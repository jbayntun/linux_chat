#ifndef NETWORK
#define NETWORK

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <map>
#include <string>

#define TCP_LISTEN_PORT 8896
#define MAX_CONNECTIONS 12

#define RECEIVE_SIZE	255
#define SEND_SIZE 1023
const static char user_char =  '^';
const static char remove_user = '%';

typedef struct _client
{
    int incoming;
    int outgoing;
    std::string name;
} CLIENT;

static std::map<std::string, CLIENT*> clients;

int createListenSocket(int port);
void sendToAll(int client, const char* message);
bool acceptConnection(struct pollfd* sockets);
bool addNewConnection(struct pollfd* sockets, int s);
int closeAllSockets(struct pollfd* sockets);
void checkForEvents(struct pollfd* sockets);

CLIENT* getClientByOutgoing(int outgoing);
CLIENT* getClientByIncoming(int incoming);
void removeClient(int sd);
void sendUsers();

#endif // NETWORK

