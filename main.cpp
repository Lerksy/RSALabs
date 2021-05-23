#include "dialog.h"

#include <QApplication>
#include <Qca-qt5/QtCrypto/QtCrypto>

int main(int argc, char *argv[])
{
    QCA::Initializer init;
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}
