#include <QCoreApplication>
#include <newserver.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server q;
    return a.exec();
}
