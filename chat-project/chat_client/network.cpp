/*--------------------------------------------------------------------------
    PROGRAM:        chat_client

    File:           network.cpp

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      std::string initChat(std::string& host, int port, std::string& user);
                    void closeChat();
                    bool connectReceiveSocket(std::string& host, int port);
                    bool connectSendSocket(std::string& host, int port);
                    int connectTCP(std::string& host, int port);
                    void addUser(int sd, char* userName);
                    bool sendMessage(std::string& message);

    NOTES:          c/c++ linux file with blocking networking calls
----------------------------------------------------------------------------*/

#include "network.h"

int rcv_socket;
int send_socket;
/*--------------------------------------------------------------------------
    FUNCTION:       connectReceiveSocket

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      bool connectReceiveSocket(std::string& host, int port)
                    host: ip/hostname to connect to
                    port: host port to connect to

    RETURNS:        false on failure

    NOTES:          tries to connect the send to a given host/port
----------------------------------------------------------------------------*/
bool connectReceiveSocket(std::string& host, int port)
{
    if( (rcv_socket = connectTCP(host, port) ) == -1 )
    {
        return false;
    }

   // shutdown(rcv_socket, SHUT_WR);
    return true;
}
/*--------------------------------------------------------------------------
    FUNCTION:       connectSendSocket

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      bool connectSendSocket(std::string& host, int port)
                    host: ip/hostname to connect to
                    port: host port to connect to

    RETURNS:        false on failure

    NOTES:          tries to connect the send socket to a given host/port
----------------------------------------------------------------------------*/
bool connectSendSocket(std::string& host, int port)
{
    if( (send_socket = connectTCP(host, port) ) == -1 )
    {
        return false;
    }

   // shutdown(send_socket, SHUT_RD);
    return true;
}
/*--------------------------------------------------------------------------
    FUNCTION:       initChat

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      bool initChat(std::string& host, int port, std::string& user)
                    host: ip/hostname to connect to
                    port: host port to connect to
                    user: reference to string that will be populated with connected users

    RETURNS:        string with usernames on success, empty string on failure

    NOTES:          connects the send and receive ports, sends our username
                    to the server and gets list of other users
----------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------
    FUNCTION:       connectTCP

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      connectTCP(std::string& host, int port)
                    host: host to connect to
                    port: port to connect to

    RETURNS:        socket after connection, -1 on failure

    NOTES:          creates socket and connects to host
----------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------
    FUNCTION:       sendMessage

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      sendMessage(std::string& message)
                    message to send

    RETURNS:        false on failure

    NOTES:          sends message to server
----------------------------------------------------------------------------*/
bool sendMessage(std::string& message)
{
    unsigned int sent = 0;
    unsigned int result;
    char buf[SEND_SIZE];
    char* temp;

    while( sent < message.length() )
    {
        temp = (char*)message.substr(0, SEND_SIZE).c_str();
        memset(buf, 0, SEND_SIZE);
        strcpy(buf, temp);
        result = send(send_socket, buf, SEND_SIZE, 0);
        if(result < 1)
        {
            return false;
        }
        sent += result;
    }
    return true;
}
/*--------------------------------------------------------------------------
    FUNCTION:       receiveMessage

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      receiveMessage()

    RETURNS:        string containing message received

    NOTES:          waits for a message until one is received
----------------------------------------------------------------------------*/
std::string receiveMessage()
{
    std::string message;
    int n = 0;
    int bytes_to_read = RECEIVE_SIZE;
    char buf[RECEIVE_SIZE];
    char* bp = buf;

    while (n < bytes_to_read)
        {
            n = recv(rcv_socket, bp, bytes_to_read, 0);
            if( n < 1 )
            {
                message.assign("");
                return message;
            }
            bp += n;
            bytes_to_read -= n;
        }
    message.assign(buf);
    return message;
}
/*--------------------------------------------------------------------------
    FUNCTION:       closeChat

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      closeChat()

    RETURNS:        void

    NOTES:          closes send and recieve sockets
----------------------------------------------------------------------------*/
void closeChat()
{
    close(send_socket);
    close(rcv_socket);
}


