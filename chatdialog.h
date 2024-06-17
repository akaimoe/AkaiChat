#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include "ui_chatdialog.h"

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private slots:
    void sendMessage();
    void receiveMessage();
    void onConnected();
    void onDisconnected();
    void connectBotton();
    void disConnectBotton();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    Ui::ChatDialog *ui;
    QTcpSocket *tcpSocket;
    QString nickname;
    QString serverIP;
    quint16 serverPort;
    QString targetNickname;

    void saveHistory(const QString &message);
};

#endif // CHATDIALOG_H
