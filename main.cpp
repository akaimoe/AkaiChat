#include <QApplication>
#include "chatdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatDialog w;
    w.show();
    return a.exec();
}
