#include "ParserInput.h"

ParserInput::ParserInput()
{
	serverName.clear();
	nickName.clear();
}

void ParserInput::setNames(const QString &server, const QString &nick)
{
	serverName = server;
	nickName = nick;
}

QStringList ParserInput::splitLine(const QString &line)
{
	QStringList list = line.split(QRegExp("[\\s\\n\\r]"));
	return list;
}

bool ParserInput::isQuit(const QString &params)
{
	if(params.contains("QUIT"))
	{
		return true;
	}
	return false;
}

bool ParserInput::isChannelMessage(const QStringList &params)
{
	if(params.at(0).contains("PRIVMSG") && params.at(1).contains('#') && params.at(2).contains(':'))
	{
		return true;
	}
	return false;
}

bool ParserInput::isPrivateMessage(const QStringList &params)
{
	if(params.at(0).contains("PRIVMSG") && !params.at(1).contains('#') && params.at(2).contains(':'))
	{
		return true;
	}
	return false;
}

QString ParserInput::getChannelMessageChannelName(const QStringList &params)
{
	QString data = params.at(1);
	return data;
}

QString ParserInput::getPrivateMessageChannelName(const QStringList &params)
{
	QString data = params.at(1);
	return data;
}
