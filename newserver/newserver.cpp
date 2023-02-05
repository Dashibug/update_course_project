#include <newserver.h>

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "start";
    }else
    {
        qDebug() << "error";
    }
    nextBlockSize = 0;
}
void Server::incomingConnection(qintptr socketDescriptor)//обработка подключений
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;
}

void Server::slotReadyRead()//чтение сообщ
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "ok!";
//        QString s;
//        QTime time;
//        in >> time >> s;
//        qDebug() << s;
//        SendToClient(s);
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                qDebug() << "nextBlockSize = 0";
                if(socket->bytesAvailable() < 2)
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
                qDebug() << "nextBlockSize = " << nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data not full, break";
                break;
            }
            QTime time;
            QString s;
            in >> time >> s;
            nextBlockSize = 0;
            SendToClient(s);
            break;
        }
    }
    else
    {
        qDebug() << "error!";
    }
}

void Server::SendToClient(QString s){
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0)<< QTime::currentTime() << s;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    out << QTime::currentTime() << s;
    //socket->write(Data);
    for(int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}
