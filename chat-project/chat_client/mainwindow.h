#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include <QThread>
#include <QTime>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "mythread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void gogoThread();
    ~MainWindow();

signals:
    void emitMessage(std::string message);
    void killThread();


public slots:
    void connectClicked();
    void sendClicked();
    void messageRecieved(QString message);
    void peerChanged(QString peer);
    void saveToFile();
    void abortConnection();
    void connectionDropped();

private:
    Ui::MainWindow *ui;
    bool connected;
    std::string userName;
    QString q_messages;
    MyThread* rcv_thread;};


#endif // MAINWINDOW_H
