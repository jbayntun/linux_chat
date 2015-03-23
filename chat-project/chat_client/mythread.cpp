#include "mythread.h"
#include "network.h"

MyThread::MyThread(QObject* parent, bool b) : QThread(parent), Stop(b)
{

}


void MyThread::run()
{
    std::string incoming;
    QString qstr;
    while(1)
    {
        incoming.assign(receiveMessage());
        qstr = QString::fromStdString(incoming);
        if(incoming.empty())
            continue;
        else if(incoming[0] == user_char)
        {
            emit(userChanged(qstr));
            continue;
        }



        emit(valueChanged(qstr));
    }
}

