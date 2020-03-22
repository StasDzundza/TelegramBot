#include <QCoreApplication>
#include <iostream>
#include "bot.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Bot b;
    b.start();
    return a.exec();
}
