#include "Network.h"

Network::Network(QObject *parent) : QObject(parent)
{
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Network::connectSocket(const QString &name, const qint32 port)
{
	socket->connectToHost(name, port);
	socket->waitForConnected(5000);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Network::disconnectSocket()
{
	socket->abort();
	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	emit disconnectAvailable();
}

bool Network::isSocketConnected()
{
	return (socket->state() == QTcpSocket::ConnectedState);
}

QString Network::readSocketData()
{
	QString data; // @TODO
	if(isSocketConnected())
	{
		data = socket->readAll();
	}
	return data;
}

void Network::writeSocketData(const QString &data)
{
	if(isSocketConnected())
	{
		QString line = data;
		line.append("\r\n");
		socket->write(line.toUtf8());
	}
}

void Network::disconnected()
{
	emit disconnectAvailable();
}

void Network::readyRead()
{
	emit readyReadAvailable();
}
