#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QHostAddress>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog)
    , udpSocket(new QUdpSocket(this))
{
    ui->setupUi(this);

    connect(ui->sendButton, &QPushButton::clicked, this, &ChatDialog::sendMessage);
    connect(udpSocket, &QUdpSocket::readyRead, this, &ChatDialog::receiveMessage);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::sendMessage()
{
    QString message = ui->messageEdit->text();
    QString ip = ui->ipEdit->text();
    quint16 port = ui->portEdit->text().toUInt();
    nickname = ui->nicknameEdit->text();

    if (!message.isEmpty() && !ip.isEmpty() && port > 0) {
        QByteArray data = QString("%1: %2").arg(nickname).arg(message).toUtf8();
        udpSocket->writeDatagram(data, QHostAddress(ip), port);
        saveHistory(QString("Me (%1): %2").arg(QDateTime::currentDateTime().toString()).arg(message));
        ui->messageEdit->clear();
    }
}

void ChatDialog::receiveMessage()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QString message = QString::fromUtf8(datagram);
        ui->chatHistory->append(QString("%1 (%2:%3): %4").arg(QDateTime::currentDateTime().toString()).arg(sender.toString()).arg(senderPort).arg(message));
        saveHistory(QString("%1 (%2:%3): %4").arg(QDateTime::currentDateTime().toString()).arg(sender.toString()).arg(senderPort).arg(message));
    }
}

void ChatDialog::saveHistory(const QString &message)
{
    QFile file("history.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << message << "\n";
    }
}
