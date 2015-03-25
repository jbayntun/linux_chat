#include "network.h"

using namespace std;

int createListenSocket(int port)
{
    int sd, arg;
    struct	sockaddr_in server;
    // Create a stream socket
        if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror ("Can't create a socket");
            return -1;
        }

        // Bind an address to the socket
        bzero((char *)&server, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

        if (setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
        {
                        printf("setsockopt error");
                        return -1;
        }

        if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            perror("Can't bind name to socket");
            return -1;
        }

        // Listen for connections

        // queue up to 5 connect requests
        listen(sd, 5);

        return sd;

}

void sendToAll(int client, const char* message)
{
    char toSend[SEND_SIZE];
    char*actuallySending;
    map<string, CLIENT*>::iterator it;

    if(client == user_char)
    {
        actuallySending = (char*)message;
    }
    else
    {
        string client_name = getClientByIncoming(client)->name;
        for(int i = 0; i < client_name.length(); i++)
        {
            toSend[i] = client_name[i];
        }
        toSend[client_name.length()] = ':';
        toSend[client_name.length() + 1] = ' ';
        strcpy(&(toSend[client_name.length() + 2]), message);
        actuallySending = toSend;
    }

    for(it= clients.begin(); it != clients.end(); it++)
    {
        send(it->second->outgoing, actuallySending, SEND_SIZE, 0);
    }
}

bool acceptConnection(struct pollfd* sockets)
{
    int new_sd;
    struct	sockaddr_in client;
    int client_len;
    string ip;

    client_len= sizeof(client);
    if ((new_sd = accept (sockets[0].fd, (struct sockaddr *)&client, (socklen_t*)&client_len)) == -1)
    {
        fprintf(stderr, "Can't accept client\n");
        return false;
    }
    ip.assign(inet_ntoa(client.sin_addr));

    map<string, CLIENT*>::iterator it = clients.find(ip);
    // new client, this is clients outgoing socket
    if( it == clients.end() )
    {
        CLIENT* temp = new CLIENT;
        temp->outgoing = new_sd;
        clients.insert({ip, temp});
        printf(" Added outgoing socket for:  %s\n", inet_ntoa(client.sin_addr));
    }
    else // need to add outgoing to this client
    {
        it->second->incoming = new_sd;
        printf(" Added incoming socket for:  %s\n", inet_ntoa(client.sin_addr));
    }
    return addNewConnection(sockets, new_sd);
}

bool addNewConnection(struct pollfd* sockets, int s)
{
    for (int i = 1; i < MAX_CONNECTIONS + 1; i++)
    {
        if(sockets[i].fd < 0)
        {
            sockets[i].fd = s;
            sockets[i].events = POLLIN;
            return true;
        }
    }
    return false;
}

void removeClient(int sd)
{
    map<string, CLIENT*>::iterator it;
    for(it= clients.begin(); it != clients.end(); it++)
    {
        if(it->second->incoming == sd || it->second->outgoing == sd)
            clients.erase(it);
    }
    sendUsers();
}

CLIENT* getClientByIncoming(int incoming)
{
    map<string, CLIENT*>::iterator it;
    for(it= clients.begin(); it != clients.end(); it++)
    {
        if(it->second->incoming == incoming)
            return it->second;
    }
    return NULL;
}

CLIENT* getClientByOutgoing(int outgoing)
{
    map<string, CLIENT*>::iterator it;
    for(it= clients.begin(); it != clients.end(); it++)
    {
        if(it->second->outgoing == outgoing)
            return it->second;
    }
    return NULL;
}

int closeAllSockets(struct pollfd* sockets)
{
    for(int i = 0; i < MAX_CONNECTIONS + 1; i++)
    {
        if(sockets[i].fd > 0)
            close(sockets[i].fd);
    }
    printf("closing program\n");
    return 1;
}

void addUser(int sd, char* userName)
{
    CLIENT* c = getClientByIncoming(sd);
    c->name.assign(userName);

    sendUsers();

}

void sendUsers()
{
    string message(1, user_char);

    map<string, CLIENT*>::iterator it;
    for(it= clients.begin(); it != clients.end(); it++)
    {
        message += it->second->name;
        message += user_char;
    }

    sendToAll(user_char, message.c_str());
}


void checkForEvents(struct pollfd* sockets)
{
    for (int i = 1; i < MAX_CONNECTIONS + 1; i++)
    {
        /* were there any events for this socket? */
        if (!sockets[i].revents)
            continue;


        // check for hang ups
        if(sockets[i].revents & POLLRDHUP)
        {
            sockets[i].fd = -1;
            // need to print a message that this connection is closed. need a map.

            printf("connection closed");
            continue;
        }

        // check for hang ups
        if(sockets[i].revents & POLLNVAL)
        {
            sockets[i].fd = -1;
            // need to print a message that this connection is closed. need a map.

            printf("connection closed");
            continue;
        }

        /* is there incoming data on the socket? */
        if (sockets[i].revents & POLLIN)
        {
            int n = 0;
            int to_read = RECEIVE_SIZE;
            char* message = new char[RECEIVE_SIZE + 1];
            /* call recv() on the socket and decide what to do from there */
            while ((n = recv (sockets[i].fd, message, to_read, 0)) < to_read)
            {
                if(n == 0)
                {
                    removeClient(sockets[i].fd);
                    sockets[i].fd = -1;
                    // need to print a message that this connection is closed. need a map.

                    printf("connection closed\n");
                    delete [] message;
                    return;
                }
                message += n;
                to_read -= n;
            }
            message[RECEIVE_SIZE] = '\0';
            if(message[0] == user_char)
            {
                addUser(sockets[i].fd, message+1);
            }
            else
            {
                printf ("sending:%s\n", message);
                sendToAll(sockets[i].fd, message);
            }
            delete [] message;

        }



    }

}
