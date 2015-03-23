#include "network.h"
#include <iostream>

using namespace std;

// should make a map for ip and socket numbers

int main()
{
    struct pollfd sockets[MAX_CONNECTIONS + 1];
    int result;

    // set up poll structures
    sockets[0].fd = createListenSocket(TCP_LISTEN_PORT);
    sockets[0].events = POLLIN;

    for(int i = 1; i <= MAX_CONNECTIONS; i++ )
    {
        sockets[i].fd = -1;
    }

    // handle poll events
    while(1)
    {
        result = poll(sockets, MAX_CONNECTIONS + 1, -1);

        if(result < 1)
        {
            perror("problem with poll");
            return closeAllSockets(sockets);
        }

        // check for accept events on listen socket
        if( sockets[0].revents & POLLIN )
        {
            //do accept
            acceptConnection(sockets);
            continue;
        }

        // check for other socket events
        checkForEvents(sockets);
    }

    return 0;
}








