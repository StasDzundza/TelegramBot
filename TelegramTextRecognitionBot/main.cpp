#include <QCoreApplication>
#include <iostream>
#include "bot.h"
#include "baseapi.h"
#include "allheaders.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Bot b;
    b.start();
    //Translater t(nullptr,0);
    //t.translateText("hello","en","es");
    return a.exec();
}
