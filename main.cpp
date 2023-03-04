#include <QCoreApplication>
#include <QDebug>

#include "appchecker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    AppChecker appChecker;
    appChecker.checkAppSingleton(QCoreApplication::applicationName());
    return a.exec();
}
