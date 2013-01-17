#include "Client.h"

/*
 *@TODO Liste
 *
 * - PRIVMSG von an andere User im PRIVMSG Chat anzeigen
 * - Channel PRIVMSG im CHannel anzeigen
 */

Client::Client(QWidget *parent) : QMainWindow(parent)
{
	// Interface
	setupUi(this);
	setView(VIEW_SERVER_DEFAULT);

	// Connections
	connect(actionConnect, SIGNAL(triggered()), this, SLOT(slotConnect()));
	connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(slotDisconnect()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
	connect(actionShowConnection, SIGNAL(triggered()), this, SLOT(slotShowConnection()));
	connect(actionShowUser, SIGNAL(triggered()), this, SLOT(slotShowUser()));
	connect(actionHelp, SIGNAL(triggered()), this, SLOT(slotHelp()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
	connect(listWidgetConnection, SIGNAL(currentRowChanged(int)), this, SLOT(slotChangeConnectionItem(int)));
	connect(listWidgetUser, SIGNAL(currentRowChanged(int)), this, SLOT(slotChangeUserItem(int)));
	connect(lineEditMessage, SIGNAL(returnPressed()), pushButtonSend, SLOT(click()));
	connect(pushButtonSend, SIGNAL(clicked()), this, SLOT(slotWriteStream()));

	// Network
	networkHandler = new Network(this);
	connect(networkHandler, SIGNAL(disconnectAvailable()), this, SLOT(slotDisconnected()));
	connect(networkHandler, SIGNAL(readyReadAvailable()), this, SLOT(slotReadStream()));
}

void Client::closeEvent(QCloseEvent *)
{
	networkHandler->disconnectSocket();
	setView(VIEW_SERVER_DEFAULT);
}

void Client::slotConnect()
{
	if(!networkHandler->isSocketConnected()) // Not connected - we connect now
	{
		ConnectDialog *dialog = new ConnectDialog(this);
		if(dialog->exec() == QDialog::Accepted)
		{
			QString server = dialog->getServerName();
			qint32 port = dialog->getServerPort();
			QString nick = dialog->getNickName();

			addChatWidget(server, TYPE_SERVER);
			parserOutputHandler.setNames(server, nick);
			parserInputHandler.setNames(server, nick);
			networkHandler->connectSocket(server, port);
			networkHandler->writeSocketData("NICK " + nick.toUtf8());
			networkHandler->writeSocketData("USER User Client Client : Realname");
		}
	}
	else
	{
		QMessageBox::warning(this, "Warning", "You are already connected to a server. Please discconect from the current server and then you can connect to another server");
	}
}

void Client::slotDisconnect()
{
	networkHandler->disconnectSocket();
	setView(VIEW_SERVER_DEFAULT);
}

void Client::slotDisconnected()
{
	setView(VIEW_SERVER_DEFAULT);
}

void Client::slotExit()
{
	exit(EXIT_SUCCESS);
}

void Client::slotShowConnection()
{
	if(actionShowConnection->isChecked())
	{
		dockWidgetConnection->show();
	}
	else
	{
		dockWidgetConnection->hide();
	}
}

void Client::slotShowUser()
{
	if(actionShowUser->isChecked())
	{
		dockWidgetUser->show();
	}
	else
	{
		dockWidgetUser->hide();
	}
}

void Client::slotHelp()
{
	QMessageBox::information(this, "Help", "Help for this program\n\nConnect: Connect to a server\nDisconnect: Leave the current server\nShow elements: Enable/Disable display elements");
}

void Client::slotAbout()
{
	QMessageBox::information(this, "About", "Karma IRC Client v0.0.1\n\nAuthor: Simon Waechter\nContact: waechter.simon@gmail.com\nDate: January 2013\nLanguage: C++ and Qt");
}

void Client::slotChangeConnectionItem(const int index)
{
	if(index != LIST_WIDGET_NO_CURRENT_ITEM)
	{
		ChatWidget *widget = getChatWidgetPointerById(index);

		stackedWidget->setCurrentIndex(index);
		listWidgetConnection->setCurrentRow(index);
		listWidgetUser->clear();

		if(widget->isChannel())
		{
			QString line = "NAMES " + widget->getName();
			networkHandler->writeSocketData(line);
		}
	}
}

void Client::slotChangeUserItem(const int index)
{
	if(index != LIST_WIDGET_NO_CURRENT_ITEM)
	{
		QListWidgetItem *item = listWidgetUser->item(index);
		QString name = item->text();
		addChatWidget(name, TYPE_USER);
	}
}

void Client::slotReadStream()
{
	QString data = networkHandler->readSocketData();
	QStringList lines = data.split('\n');
	for(qint32 i = 0; i < lines.count(); i++)
	{
		QString line = lines.at(i);
		if(!line.isEmpty())
		{
			QStringList params = parserOutputHandler.splitLine(line);
			if(parserOutputHandler.isPing(params))
			{
				networkHandler->writeSocketData("PONG");
			}
			else if(parserOutputHandler.isJoin(params))
			{
				QString channel = parserOutputHandler.getJoinChannelName(params);
				addChatWidget(channel, TYPE_CHANNEL);
			}
			else if(parserOutputHandler.isOtherJoin(params))
			{
				QString channel = parserOutputHandler.getJoinChannelName(params);
				addChatWidgetText(channel, line);
			}
			else if(parserOutputHandler.isPart(params))
			{
				QString channel = parserOutputHandler.getPartChannelName(params);
				removeChatWidget(channel);
			}
			else if(parserOutputHandler.isOtherPart(params))
			{
				QString channel = parserOutputHandler.getPartChannelName(params);
				addChatWidgetText(channel, line);
			}
			else if(parserOutputHandler.isChannelMessage(params))
			{
				QString channel = parserOutputHandler.getChannelMessageChannelName(params);
				addChatWidgetText(channel, line);
			}
			else if(parserOutputHandler.isPrivateMessage(params))
			{
				QString channel = parserOutputHandler.getPrivateMessageChannelName(params);
				addChatWidget(channel, TYPE_USER);
				addChatWidgetText(channel, line);
			}
			else if(parserOutputHandler.isNames(params))
			{
				QStringList users = parserOutputHandler.getNamesChannelName(params);
				updateUserList(params.at(4), users);
			}
			else if(parserOutputHandler.isChannelCode(params))
			{
				QString channel = parserOutputHandler.getChannelCodeChannelName(params);
				// @TODO addChatWidgetText(channel, line) - maybe to a log, but don't spam a channel or the server chat
			}
			else
			{
				ChatWidget *widget = getChatWidgetPointerById(CHAT_WIDGET_SERVER);
				addChatWidgetText(widget->getName(), line);
			}
		}
	}
}

void Client::slotWriteStream()
{
	QString line = lineEditMessage->text();
	lineEditMessage->clear();
	if(!line.isEmpty())
	{
		if(networkHandler->isSocketConnected())
		{
			QStringList params = parserInputHandler.splitLine(line);
			if(parserInputHandler.isQuit(line))
			{
				networkHandler->writeSocketData(line);
				networkHandler->disconnectSocket();
			}
			else if(parserInputHandler.isChannelMessage(params))
			{
				QString channel = parserInputHandler.getChannelMessageChannelName(params);
				addChatWidgetText(channel, line);
				networkHandler->writeSocketData(line);
			}
			else if(parserInputHandler.isPrivateMessage(params))
			{
				QString user = parserInputHandler.getPrivateMessageChannelName(params);
				addChatWidgetText(user, line);
				networkHandler->writeSocketData(line);
			}
			else
			{
				networkHandler->writeSocketData(line);
			}
		}
	}
}

void Client::setView(const qint32 view)
{
	switch(view)
	{
		case VIEW_SERVER_DEFAULT:
		{
			for(qint32 index = 0; index < listChatWidget.count();)
			{
				ChatWidget *widget = getChatWidgetPointerById(index);
				removeChatWidget(widget->getName());
			}

			lineEditMessage->clear();
			lineEditMessage->setReadOnly(true);
			pushButtonSend->setEnabled(false);
			listWidgetConnection->clear();
			listWidgetUser->clear();
			listChatWidget.clear();

			break;
		}
		case VIEW_SERVER_OPEN:
		{
			lineEditMessage->setReadOnly(false);
			pushButtonSend->setEnabled(true);

			break;
		}
		default:
		{
			break;
		}
	}
}

void Client::addChatWidget(const QString &name, const qint32 type)
{
	qint32 index = getChatWidgetIdByName(name);
	if(index == CHAT_WIDGET_NOT_EXISTING)
	{
		ChatWidget *widget = new ChatWidget(name, type, stackedWidget);

		stackedWidget->addWidget(widget);
		listChatWidget.append(widget);
		listWidgetConnection->addItem(name);

		if(widget->isServer())
		{
			setView(VIEW_SERVER_OPEN);
			listWidgetConnection->setCurrentRow(0);
		}
	}
}

void Client::removeChatWidget(const QString &name)
{
	qint32 index = getChatWidgetIdByName(name);
	if(index != CHAT_WIDGET_NOT_EXISTING)
	{
		ChatWidget *remove = getChatWidgetPointerById(index);

		stackedWidget->removeWidget(remove);
		listChatWidget.removeAt(index);
		listWidgetUser->clear();
		listWidgetConnection->clear();

		for(qint32 index = 0; index < listChatWidget.count(); index++)
		{
			ChatWidget *widget = listChatWidget.at(index);
			listWidgetConnection->addItem(widget->getName());
		}
	}
}

void Client::addChatWidgetText(const QString &name, const QString &text)
{
	qint32 index = getChatWidgetIdByName(name);
	if(index != CHAT_WIDGET_NOT_EXISTING)
	{
		ChatWidget *widget = getChatWidgetPointerById(index);
		widget->addText(text);
	}
}

ChatWidget *Client::getChatWidgetPointerById(qint32 index)
{
	ChatWidget *widget = listChatWidget.at(index);
	return widget;
}

qint32 Client::getChatWidgetIdByName(const QString &name)
{
	for(qint32 index = 0; index < listChatWidget.count(); index++)
	{
		ChatWidget *widget = getChatWidgetPointerById(index);

		if(name.compare(widget->getName()) == 0)
		{
			return index;
		}
	}
	return CHAT_WIDGET_NOT_EXISTING;
}

void Client::updateUserList(const QString &name, const QStringList &users)
{
	qint32 index = stackedWidget->currentIndex();
	ChatWidget *widget = getChatWidgetPointerById(index);

	if(name.compare(widget->getName()) == 0)
	{
		listWidgetUser->addItems(users);
	}
}
