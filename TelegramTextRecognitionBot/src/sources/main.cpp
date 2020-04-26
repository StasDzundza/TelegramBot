#include <QCoreApplication>
#include <iostream>
#include "bot.h"
//#include "tesseractocr.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Bot b;
    b.start();
    //TesseractOCR ocr;
    //QString res = ocr.recognizeImage("E:\\Studying\\0_whatsappweb1_censored.jpg","eng");
    //std::cout << res.toStdString() <<std::endl;
    return a.exec();
}
