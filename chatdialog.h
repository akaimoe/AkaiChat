#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatDialog; }
QT_END_NAMESPACE

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private slots:
    void sendMessage();
    void receiveMessage();
    void saveHistory(const QString &message);

private:
    Ui::ChatDialog *ui;
    QUdpSocket *udpSocket;
    QString nickname;
    quint16 port;
};

#endif // CHATDIALOG_H
