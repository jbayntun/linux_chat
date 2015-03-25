#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "helpers.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->host_port->setText("8896");
    ui->host_name->setText("127.0.0.1");
    connected = false;

    sem = new QSemaphore(0);

    connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(connectClicked()));
    connect(ui->send_button, SIGNAL(clicked()), this, SLOT(sendClicked()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(saveToFile()));
}

MainWindow::~MainWindow()
{
    emit(killThread());
    wait();
    delete ui;
    delete sem;
}

void MainWindow::connectClicked()
{
        //close connection if already connected(button says abort...)
    if(connected)
    {
        abortConnection();
        return;
    }

    // if not all fields are filled in, wait
    if( ui->host_name->text() == ""
            || ui->host_port->text() == ""
            || ui->user_name->text() == "")
    {
        return;
    }

    string users;
    string host = ui->host_name->text().toStdString();
    int port = ui->host_port->text().toInt();
    userName = ui->user_name->text().toStdString();

    if( (users = initChat(host, port, userName)) == "")
    {
        return;
    }

    ui->send_button->setEnabled(true);
    ui->save->setEnabled(true);
    ui->message->setEnabled(true);
    ui->connect_button->setText("Abort Connection");
    gogoThread();
    connected = true;




    users.erase(users.begin());
    QString qstr = QString::fromStdString(users);
    peerChanged(qstr);

    std::cout << "sem released\n";
    fflush(stdout);
    sem->release();
}

void MainWindow::gogoThread()
{
    rcv_thread = new MyThread();
    connect(this, SIGNAL(killThread()), rcv_thread, SLOT(terminate()));
    connect(this, SIGNAL(startThread(QSemaphore*)), rcv_thread, SLOT(initThread(QSemaphore*)));

    connect(rcv_thread, SIGNAL( userChanged(QString) ), this, SLOT( peerChanged(QString) ));
    connect(rcv_thread, SIGNAL( valueChanged(QString) ), this, SLOT( messageRecieved(QString) ));
    emit(startThread(sem));
    rcv_thread->start();
}

void MainWindow::sendClicked()
{
    string message = ui->message->toPlainText().toStdString();
    sendMessage(message);
    ui->message->clear();
}

void MainWindow::messageRecieved(QString message)
{
    q_messages += message + "\n";

    ui->conversation->setText(q_messages);
}

void MainWindow::peerChanged(QString peer)
{

    ui->user_list->clear();
    string users = peer.toStdString();

    vector<string> user_v;
    Split(users, user_v, user_char);
    for(string s: user_v)
    {
        QListWidgetItem* i = new QListWidgetItem();
        QString qstr = QString::fromStdString(s);
        i->setText(qstr);
        ui->user_list->addItem(i);
    }

}

void MainWindow::saveToFile()
{

}

void MainWindow::abortConnection()
{
    closeChat();
    connected = false;
    ui->conversation->setText("");
   // ui->user_list->
    ui->message->setEnabled(false);
    ui->message->clear();
    ui->connect_button->setText("Connect");
    ui->send_button->setEnabled(false);
    ui->save->setEnabled(false);
    ui->user_list->clear();
    q_messages.clear();
    // sets boolean so thread pauses
    emit(killThread());
}
