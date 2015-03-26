/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPlainTextEdit *message;
    QTextBrowser *conversation;
    QPushButton *send_button;
    QLineEdit *user_name;
    QLineEdit *host_name;
    QLineEdit *host_port;
    QPushButton *connect_button;
    QFrame *line;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *save;
    QListWidget *user_list;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(874, 485);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        message = new QPlainTextEdit(centralWidget);
        message->setObjectName(QStringLiteral("message"));
        message->setEnabled(false);
        message->setGeometry(QRect(220, 340, 491, 78));
        sizePolicy.setHeightForWidth(message->sizePolicy().hasHeightForWidth());
        message->setSizePolicy(sizePolicy);
        conversation = new QTextBrowser(centralWidget);
        conversation->setObjectName(QStringLiteral("conversation"));
        conversation->setEnabled(true);
        conversation->setGeometry(QRect(220, 10, 491, 311));
        send_button = new QPushButton(centralWidget);
        send_button->setObjectName(QStringLiteral("send_button"));
        send_button->setEnabled(false);
        send_button->setGeometry(QRect(760, 380, 71, 27));
        user_name = new QLineEdit(centralWidget);
        user_name->setObjectName(QStringLiteral("user_name"));
        user_name->setGeometry(QRect(50, 60, 113, 27));
        host_name = new QLineEdit(centralWidget);
        host_name->setObjectName(QStringLiteral("host_name"));
        host_name->setGeometry(QRect(50, 120, 113, 27));
        host_port = new QLineEdit(centralWidget);
        host_port->setObjectName(QStringLiteral("host_port"));
        host_port->setGeometry(QRect(50, 180, 113, 27));
        connect_button = new QPushButton(centralWidget);
        connect_button->setObjectName(QStringLiteral("connect_button"));
        connect_button->setGeometry(QRect(40, 230, 151, 27));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(200, 10, 20, 401));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 30, 91, 17));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 90, 91, 17));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 160, 67, 17));
        save = new QPushButton(centralWidget);
        save->setObjectName(QStringLiteral("save"));
        save->setEnabled(false);
        save->setGeometry(QRect(38, 270, 151, 27));
        user_list = new QListWidget(centralWidget);
        user_list->setObjectName(QStringLiteral("user_list"));
        user_list->setGeometry(QRect(720, 10, 151, 331));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 874, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        send_button->setText(QApplication::translate("MainWindow", "Send", 0));
        user_name->setText(QApplication::translate("MainWindow", "MilkDuds", 0));
        host_name->setText(QApplication::translate("MainWindow", "127.0.0.1", 0));
        host_port->setText(QApplication::translate("MainWindow", "8896", 0));
        connect_button->setText(QApplication::translate("MainWindow", "Connect", 0));
        label->setText(QApplication::translate("MainWindow", "User Name:", 0));
        label_2->setText(QApplication::translate("MainWindow", "Host Name:", 0));
        label_3->setText(QApplication::translate("MainWindow", "Host Port:", 0));
        save->setText(QApplication::translate("MainWindow", "Save to File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
