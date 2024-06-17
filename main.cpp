#include <QCoreApplication>
#include "chatserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ChatServer server;
//    if (!server.listen(QHostAddress::Any, 1920)) {
//        qFatal("Failed to start server");
//    } else {
//        qDebug() << "Server started on port 1920";
//    }

    return a.exec();
}
