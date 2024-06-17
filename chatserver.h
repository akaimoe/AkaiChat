#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

class ChatServer : public QTcpServer
{
    Q_OBJECT

public:
    ChatServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    void restartServer();

private slots:
    void receiveNickname();
    void receiveMessage();
    void clientDisconnected();

private:
    QList<QTcpSocket *> clients;

    void sendMessageToClient(const QString &message, QTcpSocket *clientSocket);

    QHash<QTcpSocket*, QString> socketToNickname;  // 存储客户端套接字及其对应的昵称
    QHash<QString, QTcpSocket*> nicknameToSocket;  // 存储昵称及其对应的客户端套接字
};

#endif // CHATSERVER_H
