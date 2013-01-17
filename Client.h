#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QList>

#include "ui_Client.h"
#include "Globals.h"
#include "Network.h"
#include "ParserOutput.h"
#include "ParserInput.h"
#include "ChatWidget.h"
#include "ConnectDialog.h"

class Client : public QMainWindow, public Ui_Client
{
		Q_OBJECT

	public:
		Client(QWidget *parent = 0);

	protected:
		void closeEvent(QCloseEvent *);

	private:
		Ui::Client *ui;

		Network *networkHandler;
		ParserOutput parserOutputHandler;
		ParserInput parserInputHandler;

		QList <ChatWidget *> listChatWidget;

		void setView(const qint32 view);
		void addChatWidget(QString const &name, const qint32 type);
		void removeChatWidget(QString const &name);
		void addChatWidgetText(QString const &name, QString const &text);
		ChatWidget *getChatWidgetPointerById(qint32 index);
		qint32 getChatWidgetIdByName(QString const &name);
		void updateUserList(QString const &name, QStringList const &users);

	private slots:
		void slotConnect();
		void slotDisconnect();
		void slotDisconnected();
		void slotExit();
		void slotShowConnection();
		void slotShowUser();
		void slotHelp();
		void slotAbout();
		void slotChangeConnectionItem(const int index);
		void slotChangeUserItem(const int index);
		void slotReadStream();
		void slotWriteStream();
};

#endif // CLIENT_H
