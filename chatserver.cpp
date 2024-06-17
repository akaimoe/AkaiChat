#include "chatserver.h"
#include <QDebug>
#include <QDateTime>
#include <iostream>

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
    if (!listen(QHostAddress::Any, 1920)) {
        qDebug() << "Failed to start server.";
        restartServer();
    } else {
        qDebug() << "Server started.";
    }
}

void ChatServer::restartServer()
{
    close(); // 关闭当前监听
    if (!listen(QHostAddress::Any, 1920)) {
        qDebug() << "Failed to restart server.";
    } else {
        qDebug() << "Server restarted.";
    }
}


void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::receiveNickname);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::clientDisconnected);

    clients.append(clientSocket);
    qDebug() << "Client connected. " << clientSocket->property("nickname").toString();
}

void ChatServer::receiveNickname()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket)
        return;

    QByteArray data = clientSocket->readAll();
    QString nickname = QString::fromUtf8(data).trimmed();
    clientSocket->setProperty("nickname", nickname);
    qDebug() << "Received nickname:" << nickname;

    // Change the connection to read messages after setting the nickname
    disconnect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::receiveNickname);
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::receiveMessage);
}

void ChatServer::receiveMessage()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if (!senderSocket)
        return;

    while (senderSocket->bytesAvailable()) {
        QByteArray data = senderSocket->readAll();
        QString message = QString::fromUtf8(data);

        QStringList messageParts = message.split(":");
        if (messageParts.size() == 3) {
            QString targetNickname = messageParts.at(0);
            QString senderNickname = messageParts.at(1);
            QString content = messageParts.at(2);

            // 查找目标客户端
            int num = 0;
            for (QTcpSocket *client : qAsConst(clients)) {
                num += 1;
                if (client != senderSocket) {
                    // 如果找到目标客户端，发送消息
                    qDebug() << targetNickname << ' ' << client->property("nickname").toString() << ' ' << client->property("serverIP").toString();
                    if (senderNickname == targetNickname) {
                        QString errorMessage = QString("You can't send the message to yourself!");
                        sendMessageToClient(errorMessage, senderSocket);
                        return;
                    }
                    if (client->property("nickname").toString() == targetNickname) {
                        QString messageToSend = QString("%1: %2").arg(senderNickname).arg(content);
                        sendMessageToClient(messageToSend, client);
                        return;
                    }
                }

            }
            std::cout << num << std::endl;
            // 如果未找到目标客户端，发送消息给发送者
            QString errorMessage = QString("Target client '%1' not found.").arg(targetNickname);
            sendMessageToClient(errorMessage, senderSocket);
        }
    }
}

void ChatServer::clientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket)
        return;

    clients.removeOne(clientSocket);
    clientSocket->deleteLater();

    qDebug() << "Client disconnected." << clients;
}

void ChatServer::sendMessageToClient(const QString &message, QTcpSocket *clientSocket)
{
    QByteArray data = message.toUtf8();
    clientSocket->write(data);
    clientSocket->flush();
}
