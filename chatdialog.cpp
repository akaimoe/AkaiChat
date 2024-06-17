#include "chatdialog.h"
#include <QHostAddress>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog)
    , tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    ui->messageEdit->setReadOnly(true);
    ui->chatHistory->setReadOnly(true);

    connect(ui->sendButton, &QPushButton::clicked, this, &ChatDialog::sendMessage);
    connect(ui->connectButton, &QPushButton::clicked, this, &ChatDialog::connectBotton);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &ChatDialog::disConnectBotton);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ChatDialog::receiveMessage);
    connect(tcpSocket, &QTcpSocket::connected, this, &ChatDialog::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ChatDialog::onDisconnected);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &ChatDialog::onErrorOccurred);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::sendMessage()
{
    QString message = ui->messageEdit->text();
    serverIP = ui->ipEdit->text();
    serverPort = ui->portEdit->text().toUShort();
    nickname = ui->nicknameEdit->text();
    targetNickname = ui->targetNicknameEdit->text();

    if (tcpSocket->state() != QAbstractSocket::ConnectedState) {
        ui->chatHistory->append("Errer, you should connect at first!");
//        rerturn;
    }

    else if (!message.isEmpty() && !targetNickname.isEmpty() && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        QString dataToSend = QString("%1:%2:%3").arg(targetNickname).arg(nickname).arg(message);
        QByteArray data = dataToSend.toUtf8();
        tcpSocket->write(data);
        saveHistory(QString("Me (%1): %2").arg(QDateTime::currentDateTime().toString()).arg(message));
        ui->messageEdit->clear();
    }
}

void ChatDialog::receiveMessage()
{
    while (tcpSocket->bytesAvailable()) {
        QByteArray data = tcpSocket->readAll();
        QString message = QString::fromUtf8(data);
        ui->chatHistory->append(QString("%1").arg(message));
        saveHistory(QString("%1").arg(message));
    }
}

void ChatDialog::onConnected()
{
    tcpSocket->setProperty("nickname", nickname);
    qDebug() << tcpSocket->property("nickname").toString();
    tcpSocket->write(nickname.toUtf8());
    tcpSocket->flush();
    ui->chatHistory->append("Connected to server. nickname is ");
    ui->chatHistory->append(nickname);
    ui->nicknameEdit->setReadOnly(true);
    ui->ipEdit->setReadOnly(true);
    ui->portEdit->setReadOnly(true);
    ui->messageEdit->setReadOnly(false);
}

void ChatDialog::onDisconnected()
{
    ui->chatHistory->append("Disconnected from server.");
    ui->nicknameEdit->setReadOnly(false);
    ui->ipEdit->setReadOnly(false);
    ui->portEdit->setReadOnly(false);
    ui->messageEdit->setReadOnly(true);
}

void ChatDialog::connectBotton()
{
    serverIP = ui->ipEdit->text();
    serverPort = ui->portEdit->text().toUShort();
    nickname = ui->nicknameEdit->text();

    if(!nickname.isEmpty())
    {
        tcpSocket->connectToHost(QHostAddress(serverIP), serverPort);
    }
    else
    {
        ui->chatHistory->append("take the info into the nickname");
    }
}

void ChatDialog::disConnectBotton()
{
        tcpSocket->disconnectFromHost();

}

void ChatDialog::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    ui->chatHistory->append("Error: " + tcpSocket->errorString());
}

void ChatDialog::saveHistory(const QString &message)
{
    QFile file("history.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << message << "\n";
    }
}
