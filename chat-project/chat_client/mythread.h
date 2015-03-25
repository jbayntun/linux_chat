#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QString>
#include <QSemaphore>
#include <string>
/**
 * @brief The MyThread class
 * based on code from
 * http://www.bogotobogo.com/Qt/Qt5_QThreads_GuiThread.php
 */

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject* parent = 0, bool b = false);
    void run();
public slots:
    void initThread(QSemaphore*);
signals:
    void valueChanged(QString message);
    void userChanged(QString message);
private:
        bool Stop;
        bool running;
        QSemaphore* sem;



};

#endif
