#ifndef HEADER_SERVER_H
#define HEADER_SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDataStream>
#include <QTime>


class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;

private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;//то что между клиентом и сервером
    void SendToClient(QString s);
    quint16 nextBlockSize;

public slots:
    void incomingConnection(qintptr socketDesctiptor);//qintptr - тип данных целое+
    void slotReadyRead();
};
#endif // HEADER_SERVER_H
