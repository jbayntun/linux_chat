#include "network.h"

int rcv_socket;
int send_socket;

bool connectReceiveSocket(std::string& host, int port)
{
    if( (rcv_socket = connectTCP(host, port) ) == -1 )
    {
        return false;
    }

   // shutdown(rcv_socket, SHUT_WR);
    return true;
}

bool connectSendSocket(std::string& host, int port)
{
    if( (send_socket = connectTCP(host, port) ) == -1 )
    {
        return false;
    }

   // shutdown(send_socket, SHUT_RD);
    return true;
}

std::string initChat(std::string& host, int port, std::string& user)
{
    if( !connectReceiveSocket(host, port) )
        return "";

    //connect send socket
    if( !connectSendSocket(host, port) )
        return "";


    //send username
    std::string message(1, user_char);
    message += user;
    sendMessage(message);

    //receive userlist
    std::string users = receiveMessage();
    return users;
}



int connectTCP(std::string& host, int port)
{
        int sd;
        struct hostent	*hp;
        struct sockaddr_in server;

    // Create the socket
        if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Cannot create socket");
            return -1;
        }
        bzero((char *)&server, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        if ((hp = gethostbyname(host.c_str())) == NULL)
        {
            fprintf(stderr, "Unknown server address\n");
            return -1;
        }
        bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

        // Connecting to the server
        if (connect (sd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            fprintf(stderr, "Can't connect to server\n");
            perror("connect");
            return -1;
        }

        return sd;
}

void sendMessage(std::string& message)
{
    unsigned int sent = 0;
    char buf[SEND_SIZE];
    char* temp;

    while( sent < message.length() )
    {
        temp = (char*)message.substr(0, SEND_SIZE).c_str();
        memset(buf, 0, SEND_SIZE);
        strcpy(buf, temp);
        sent += send(send_socket, buf, SEND_SIZE, 0);
    }
}

std::string receiveMessage()
{
    std::string message;
    int n = 0;
    int bytes_to_read = RECEIVE_SIZE;
    char buf[RECEIVE_SIZE];
    char* bp = buf;

    printf("network rcv entered\n");
    fflush(stdout);

    while (n < bytes_to_read)
        {
            n = recv(rcv_socket, bp, bytes_to_read, 0);
            printf("network rcv while\n");
            fflush(stdout);
            if( n < 1 )
            {
                printf("network got empty", bp);
                fflush(stdout);
                message.assign("");
                return message;
            }
            bp += n;
            bytes_to_read -= n;
        }

    printf("network after: %s\n", bp);
    fflush(stdout);

    message.assign(buf);
    return message;
}

void closeChat()
{
    close(send_socket);
    close(rcv_socket);
}


