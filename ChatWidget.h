#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>
#include <QTime>

#include "Globals.h"

class ChatWidget : public QTableWidget
{
	public:
		ChatWidget(QString const &name, const qint32 type, QWidget *parent = 0);
		QString getName() { return chatName; }
		qint32 getType() { return chatType; }
		bool isServer() { return chatType == TYPE_SERVER; }
		bool isChannel() { return chatType == TYPE_CHANNEL; }
		bool isUser() { return chatType == TYPE_USER; }
		void addText(QString const &text);

	private:
		QString chatName;
		qint32 chatType;
};

#endif // CHATWIDGET_H
