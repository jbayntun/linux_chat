/*--------------------------------------------------------------------------
    PROGRAM:        chat_client

    File:           mythread.cpp

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      explicit MyThread(QObject* parent = 0, bool b = false);
                    void run();

    SIGNALS:        void valueChanged(QString message);
                    void userChanged(QString message);
                    void connectionDropped();

    NOTES:          thread to wait for received data.
----------------------------------------------------------------------------*/

#include "mythread.h"
#include "network.h"
#include <iostream>
/*--------------------------------------------------------------------------
    FUNCTION:       MyThread

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      MyThread

    RETURNS:        constructor

    NOTES:          calls parent constructor
----------------------------------------------------------------------------*/
MyThread::MyThread(QObject* parent, bool b) : QThread(parent), Stop(b)
{

}

/*--------------------------------------------------------------------------
    FUNCTION:       run

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      run()

    RETURNS:        void

    NOTES:          waits for incoming messages, when it gets one,
                    if it starts with user_char ^ assumes it is
                    usernames and emits userChangeds signal, else
                    emits valueChanged signal with received message.

                    on error, it emits connectionDropped signal
----------------------------------------------------------------------------*/
void MyThread::run()
{
    std::string incoming;
    QString qstr;
    while(1)
    {
       std::string incoming;
       QString qstr;
       while(1)
       {
           incoming.assign(receiveMessage());
           if(incoming.compare("") == 0)
           {
               break;
           }
           qstr = QString::fromStdString(incoming);

           std::cout << "thread " << incoming << std::endl;
           fflush(stdout);

           if(incoming.empty())
               continue;
           else if(incoming[0] == user_char)
           {
               emit(userChanged(qstr));
               continue;
           }
           emit(valueChanged(qstr));
       }

       emit(connectionDropped());
    }
}

