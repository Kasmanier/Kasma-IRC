#include "ParserOutput.h"

ParserOutput::ParserOutput()
{
	serverName.clear();
	nickName.clear();
}

void ParserOutput::setNames(const QString &server, const QString &nick)
{
	serverName = server;
	nickName = nick;
}

QStringList ParserOutput::splitLine(const QString &line)
{
	QStringList list = line.split(QRegExp("[\\s\\n\\r]"));
	return list;
}

bool ParserOutput::isPing(const QStringList &params)
{
	if(params.contains("PING"))
	{
		return true;
	}
	return false;
}

bool ParserOutput::isJoin(const QStringList &params)
{
	if(params.at(0).contains(nickName) && params.at(1).contains("JOIN") && params.at(2).contains(":#"))
	{
		return true;
	}
	return false;
}

bool ParserOutput::isOtherJoin(const QStringList &params)
{
	if(!params.at(0).contains(nickName) && params.at(1).contains("JOIN") && params.at(2).contains(":#"))
	{
		return true;
	}
	return false;
}

bool ParserOutput::isPart(const QStringList &params)
{
	if(params.at(0).contains(nickName) && params.at(1).contains("PART") && params.at(2).contains('#'))
	{
		return true;
	}
	return false;
}

bool ParserOutput::isOtherPart(const QStringList &params)
{
	if(!params.at(0).contains(nickName) && params.at(1).contains("PART") && params.at(2).contains('#'))
	{
		return true;
	}
	return false;
}

bool ParserOutput::isChannelMessage(const QStringList &params)
{
	if(params.at(1).contains("PRIVMSG") && params.at(2).contains('#'))
	{
		return true;
	}
	return false;
}

bool ParserOutput::isPrivateMessage(const QStringList &params)
{
	if(params.at(1).contains("PRIVMSG") && params.at(2).contains(nickName) && params.at(3).contains(':'))
	{
		return true;
	}
	return false;
}

bool ParserOutput::isNames(const QStringList &params)
{
	bool ok = false;
	QString item = params.at(1);
	int code = item.toInt(&ok, 10);
	if(ok && code == 353)
	{
		return true;
	}
	return false;
}

bool ParserOutput::isChannelCode(const QStringList &params)
{
	bool ok = false;
	QString item = params.at(1);
	int code = item.toInt(&ok, 10);
	if(ok && (code == 353 || code == 366))
	{
		return true;
	}
	return false;
}

QString ParserOutput::getJoinChannelName(const QStringList &params)
{
	QString channel = params.at(2);
	channel.remove(0, 1); // Remove leading :
	return channel;
}

QString ParserOutput::getPartChannelName(const QStringList &params)
{
	QString channel = params.at(2);
	return channel;
}

QString ParserOutput::getChannelMessageChannelName(const QStringList &params)
{
	QString channel = params.at(2);
	return channel;
}

QString ParserOutput::getPrivateMessageChannelName(const QStringList &params)
{
	QStringList item = params.at(0).split("!~");
	QString channel = item.at(0);
	channel.remove(0, 1); // Remove leading :
	return channel;
}

QStringList ParserOutput::getNamesChannelName(const QStringList &params)
{
	QStringList users = splitNickNameList(params);
	return users;
}

QString ParserOutput::getChannelCodeChannelName(const QStringList &params)
{
	QString channel;
	if(params.at(2).contains(nickName) && params.at(3).contains('=') && params.at(4).contains('#'))
	{
		channel = params.at(4);
		return channel;
	}
	else if(params.at(2).contains(nickName) && params.at(3).contains('#'))
	{
		channel = params.at(3);
		return channel;
	}
	else // Backup
	{
		return serverName;
	}
}

QStringList ParserOutput::splitNickNameList(const QStringList &params)
{
	QStringList users = params;
	users.removeAt(0);
	users.removeAt(0);
	users.removeAt(0);
	users.removeAt(0);
	users.removeAt(0);

	QString first = users.at(0);
	first.remove(0, 1); // The first nick name has a leading :
	users.removeAt(0); // Remove item
	users.insert(0, first); // Add the right one

	for(int index = 0; index < users.count(); index++)
	{
		if(users.at(index).contains('@'))
		{
			QString user = users.at(index);
			user.remove(0, 1); // The nick name has a leading @
			users.removeAt(index); // Remove item
			users.insert(index, user); // Add the right one
		}
		if(users.at(index).isEmpty())
		{
			users.removeAt(index);
			index--;
		}
	}
	return users;
}
