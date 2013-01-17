#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>

class Network : public QObject
{
		Q_OBJECT

	public:
		Network(QObject *parent);
		void connectSocket(QString const &name, const qint32 port);
		void disconnectSocket();
		bool isSocketConnected();
		QString readSocketData();
		void writeSocketData(QString const &data);

	public slots:
		void disconnected();
		void readyRead();

	private:
		QTcpSocket *socket;

	signals:
		void disconnectAvailable();
		void readyReadAvailable();
};

#endif // NETWORK_H
