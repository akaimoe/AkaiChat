#include <QApplication>
#include "chatdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatDialog chatDialog;
    chatDialog.show();

    return a.exec();
}
