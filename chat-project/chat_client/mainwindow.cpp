/*--------------------------------------------------------------------------
    PROGRAM:        chat_client

    File:           mainwindow.cpp

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      explicit MainWindow(QWidget *parent = 0);
                    ~MainWindow();
                    void gogoThread();
                    void connectClicked();
                    void sendClicked();
                    void messageRecieved(QString message);
                    void peerChanged(QString peer);
                    void saveToFile();
                    void abortConnection();
                    void connectionDropped()

    SIGNALS:
                    void emitMessage(std::string message);
                    void killThread();

    NOTES:          contains gui information and logic for chat client
                    does not have any network calls
----------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "helpers.h"


using namespace std;

/*--------------------------------------------------------------------------
    FUNCTION:       MainWindow

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      MainWindow::MainWindow(QWidget *parent)
                    parent: parent of this widget

    RETURNS:        constructor

    NOTES:          sets initial UI values, connects button slots/signals
----------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->host_port->setText("8896");
    ui->host_name->setText("127.0.0.1");
    connected = false;


    connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(connectClicked()));
    connect(ui->send_button, SIGNAL(clicked()), this, SLOT(sendClicked()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(saveToFile()));
}
/*--------------------------------------------------------------------------
    FUNCTION:       ~MainWindow

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      ~MainWindow()


    RETURNS:        destructor

    NOTES:          terminates thread, closes sockets
----------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    emit(killThread());
    wait();
    closeChat();
    delete ui;
}
/*--------------------------------------------------------------------------
    FUNCTION:       connectClicked

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      connectClicked()


    RETURNS:        void

    NOTES:          slot for when connect button is clicked.  attempts to
                    connect to server, if cannot displays error message.
                    Else, establishes basic connection information like
                    other connected users and enables message typing/sending
                    and creates receive thread.

                    if already connected, button reads abort.  in this case,
                    closes sockets, terminates receive thread and disables
                    message typing.
----------------------------------------------------------------------------*/
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
        ui->conversation->setText("Must enter all of: HOST, PORT, USERNAME");
        return;
    }

    string users;
    string host = ui->host_name->text().toStdString();
    int port = ui->host_port->text().toInt();
    userName = ui->user_name->text().toStdString();

    if( (users = initChat(host, port, userName)) == "")
    {
        ui->conversation->setText("Unable to connect to server!");
        return;
    }

    ui->send_button->setEnabled(true);
    ui->save->setEnabled(true);
    ui->message->setEnabled(true);
    ui->conversation->clear();
    ui->connect_button->setText("Abort Connection");
    gogoThread();
    connected = true;

    QString qstr = QString::fromStdString(users);
    peerChanged(qstr);
}
/*--------------------------------------------------------------------------
    FUNCTION:       gogoThread

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      gogoThread()


    RETURNS:        void

    NOTES:          starts receive thread and establishes signals/slots
                    for communication between main and receive thread
----------------------------------------------------------------------------*/
void MainWindow::gogoThread()
{
    rcv_thread = new MyThread();
    connect(this, SIGNAL(killThread()), rcv_thread, SLOT(terminate()));

    connect(rcv_thread, SIGNAL( userChanged(QString) ), this, SLOT( peerChanged(QString) ));
    connect(rcv_thread, SIGNAL( connectionDropped() ), this, SLOT( connectionDropped() ));
    connect(rcv_thread, SIGNAL( valueChanged(QString) ), this, SLOT( messageRecieved(QString) ));
    rcv_thread->start();
}
/*--------------------------------------------------------------------------
    FUNCTION:       connectionDropped

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      connectionDropped()


    RETURNS:        void

    NOTES:          slot for when a connection is lost from the server.
                    aborts the connection and displays an error message.
----------------------------------------------------------------------------*/
void MainWindow::connectionDropped()
{
    abortConnection();
    ui->conversation->setText("Connection Dropped By Server!");
}
/*--------------------------------------------------------------------------
    FUNCTION:       sendClicked

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      sendClicked()


    RETURNS:        void

    NOTES:          slot for send button.  if there is a message in the
                    message box, sends it to the server.
----------------------------------------------------------------------------*/
void MainWindow::sendClicked()
{
    string message = ui->message->toPlainText().toStdString();
    if(message.size() < 1)
        return;
    if(!sendMessage(message))
        connectionDropped();
    ui->message->clear();
}
/*--------------------------------------------------------------------------
    FUNCTION:       messageRecieved

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      messageRecieved(QString message)
                    message: received message

    RETURNS:        void

    NOTES:          slot to get a received message from the receive thread.
                    displays it in the conversation window
----------------------------------------------------------------------------*/
void MainWindow::messageRecieved(QString message)
{
    q_messages += message + "\n";

    ui->conversation->setText(q_messages);
}
/*--------------------------------------------------------------------------
    FUNCTION:       peerChanged

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      peerChanged(QString peer)


    RETURNS:        void

    NOTES:          slot to display an updated userlist. clears current list
                    and adds all received users.
----------------------------------------------------------------------------*/
void MainWindow::peerChanged(QString peer)
{

    ui->user_list->clear();
    string users = peer.toStdString();

    vector<string> user_v;
    users.erase(users.begin());
    Split(users, user_v, user_char);
    for(string s: user_v)
    {
        QListWidgetItem* i = new QListWidgetItem();
        QString qstr = QString::fromStdString(s);
        i->setText(qstr);
        ui->user_list->addItem(i);
    }

}
/*--------------------------------------------------------------------------
    FUNCTION:       saveToFile

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      saveToFile()


    RETURNS:        void

    NOTES:          slot for save to file button.  saves current messages
                    to a file, using the current date/time as the file
                    name.
----------------------------------------------------------------------------*/
void MainWindow::saveToFile()
{
    std::cout << "saving to file\n" <<  q_messages.toStdString() ;
    fflush(stdout);
    std::string fname = QTime::currentTime().toString().toStdString();

    std::ofstream file_out;
    file_out.open("messages_" + fname);
    if(!file_out.is_open())
    {
        std::cout << "badfile\n";
        fflush(stdout);
        return;
    }
    file_out << q_messages.toStdString();
    file_out.close();
}
/*--------------------------------------------------------------------------
    FUNCTION:       abortConnection

    DATE:           March 20, 2015

    REVISIONS:      (Date and Description)

    DESIGNER:       Jeff Bayntun

    PROGRAMMER:     Jeff Bayntun

    INTERFACE:      abortConnection()


    RETURNS:        void

    NOTES:          cleans things up after connection is lost for whatever
                    reason.  terminates thread, disables message sending
                    gui elements and clears message variables.
----------------------------------------------------------------------------*/
void MainWindow::abortConnection()
{
    emit(killThread());
    closeChat();
    connected = false;
    ui->conversation->setText("");
    ui->message->setEnabled(false);
    ui->message->clear();
    ui->connect_button->setText("Connect");
    ui->send_button->setEnabled(false);
    ui->save->setEnabled(false);
    ui->user_list->clear();
    q_messages.clear();
}
