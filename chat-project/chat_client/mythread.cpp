#include "mythread.h"
#include "network.h"
#include <iostream>

MyThread::MyThread(QObject* parent, bool b) : QThread(parent), Stop(b)
{

}


void MyThread::run()
{
    std::string incoming;
    QString qstr;
    while(1)
    {
        std::cout << "waiting for sem\n";
        fflush(stdout);
        sem->acquire();
        std::cout << "sem acquired\n";
        fflush(stdout);

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

       std::cout << "got empty message";
    }
}

void MyThread::initThread(QSemaphore* s)
{
    sem = s;
}

